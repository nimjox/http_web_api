#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
#include <syslog.h>
#include <utility>
#include <forward_list>
#include <csignal>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include <errors.h>

/*** Program global settings ***/
#define RECV_SOCKET_IP "127.0.0.1"
#define RECV_SOCKET_PORT 45000

static bool shutdown_requested = false;
/*** End Program global settings ***/


/*** Aliases ***/
typedef std::pair<std::string, std::string> STRING_PAIR;
typedef std::forward_list<STRING_PAIR>      STRING_PAIR_LINKED_LIST;
/*** End Aliases ***/


/*** Signal Helpers ***/
void signalHandler (int signum)
{
    if ((signum == SIGINT) || (signum == SIGTERM))
        shutdown_requested = true;
}

void setupSignalHandlers ()
{
    struct sigaction sa;

    sa.sa_handler = signalHandler;
    sa.sa_flags = 0;
    sigemptyset (&sa.sa_mask);

    if (sigaction (SIGINT, &sa, NULL) < 0) {
        fprintf (stderr, SD_ERR "Failed to install SIGINT handler: %m\n");
        exit (EXIT_EBUSY);
    }

    if (sigaction (SIGTERM, &sa, NULL) < 0) {
        fprintf (stderr, SD_ERR "Failed to install SIGTERM handler: %m\n");
        exit (EXIT_EBUSY);
    }
}
/*** End Signal Helpers ***/


/*** Socket Helpers ***/
int shutdownTcpSocket(const int& fd)
{
    fprintf (stderr, SD_DEBUG "Calling 'close' on fd %d\n", fd);

    send(fd, "\n", 1, 0);
    shutdown(fd, SHUT_WR);
    close(fd);
    return 0;
}

int createTcpSocket (const char *addr, int port)
{
    int fd;
    struct sockaddr_in my_addr;

    fprintf (stderr, SD_DEBUG "Creating Tcp Socket Addr:%s Port:%i\n", addr, port);

    std::memset (&my_addr, 0, sizeof(struct sockaddr_in));
    my_addr.sin_family = AF_INET;
    if (addr == NULL) {
        my_addr.sin_addr.s_addr = htonl (INADDR_ANY);
        fprintf (stderr, SD_DEBUG "Creating Tcp Socket addr: INADDR_ANY\n");
    }
    else {
        struct in_addr tmp;
        if (inet_aton (addr, &tmp)) {
            my_addr.sin_addr = tmp;
            fprintf (stderr, SD_DEBUG "addr: %s", inet_ntoa(tmp));
        }
        else {
            fprintf (stderr, SD_ERR "Invalid address: %s\n", addr);
            exit (EXIT_EADDRNOTAVAIL);
        }
    }

    my_addr.sin_port = (in_port_t)htons (port);

    fd = socket (AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        fprintf (stderr, SD_ERR "socket creation failed: %s\n", strerror (errno));

        exit (EXIT_ESOCKTNOSUPPORT);
    }

    //Allow address reuse
    int optval = 1;
    setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (bind(fd, (struct sockaddr *) &my_addr, sizeof (struct sockaddr_in)) == -1) {
        fprintf (stderr, SD_ERR "socket bind failed: %s\n", strerror(errno));
        close (fd);

        exit (EXIT_ESOCKTNOSUPPORT);
    }

    return fd;
}
/*** End Socket Helpers ***/


/*** Parsers ***/
int parseRequestQuery (const std::string& query_string, STRING_PAIR_LINKED_LIST& params)
{
    /* Example: "GET /api/get?param1=hello&param2=128&_=1512519152783 HTTP/1.1";
     * protocol: HTTP/1.1
     * method  : GET
     * url     : /index.asp
     * param   : param1 = hello
     * param   : param2 = 128
     */

    // In case really odd/unexpected query_string catch parsing corner cases
    try {
        fprintf (stderr, SD_INFO "http query rcv'd:%s\n", query_string.c_str());

        std::istringstream iss (query_string);

        std::string method, query, protocol;

        if(!(iss >> method >> query >> protocol)) {
            fprintf (stderr, SD_ERR "ERROR: parsing request\n");
            return EXIT_ENOEXEC;
        }

        // reset the std::istringstream with the query string
        iss.clear ();
        iss.str (query);

        // remove the URL part
        std::string url;
        if(!std::getline (iss, url, '?')) {
            fprintf (stderr, SD_ERR "ERROR: query malformated\n");
            return EXIT_ENOEXEC;
        }

        // store query key/value pairs in a list, ie. we have to itr on all of them O(n)
        // split by '&' delimiter
        std::string keyval, key, val;
        while(std::getline (iss, keyval, '&')) {
            std::istringstream iss_sub (keyval);

            // split key/value pairs
            if(std::getline (std::getline (iss_sub, key, '='), val)) {
                // Don't save cache timestamp if it's was passed in
                if (key == "_")
                    continue;

                STRING_PAIR key_value;
                key_value.first = key;
                key_value.second = val;
                params.push_front (key_value);
            }
        }

        fprintf (stderr, SD_INFO "protocol:%s\n", protocol.c_str ());
        fprintf (stderr, SD_INFO "method:%s\n", method.c_str ());
        fprintf (stderr, SD_INFO "url:%s\n", url.c_str ());

        for(auto const& param: params)
            fprintf (stderr, SD_INFO "param - key:%s, value:%s\n",
                     param.first.c_str(), param.second.c_str());
    }
    catch (...) {
        fprintf (stderr, SD_ERR "ERROR: http string malformated\n");
        return EXIT_EINVAL;
    }

    return EXIT_SUCCESS;
}
/*** End Parsers ***/


/*** Supporting Functions ***/
void processRequest (const int& fd)
{
    char buf[2048] = {};
    int len = sizeof(buf) - 1;
    long bytesRead;
    std::string request_string;

    int wait = 500;	// in milliseconds
    struct pollfd fds[1];

    fds->fd = fd;
    fds->events = POLLIN;

    while (true) {
        int ready = poll (fds, 1, wait);
        if (ready < 0) {
            fprintf (stderr, SD_ERR "poll() failed: %m\n");
            break;
        }
        else if (ready == 0) {
            fprintf (stderr, SD_ERR "poll() timed out: revents=0x%X\n", fds->revents);
            break;
        }
        else if (fds->revents & POLLIN) {
            memset(buf, 0, sizeof (buf));
            bytesRead = recv (fd, buf, len, 0);

            if (bytesRead < 0) {
                fprintf (stderr, SD_ERR "recv() failed: %m\n");
                break;
            }
            else {
                buf[len] = '\0';

                // Accumulate the bytes read from the socket, and break once we
                // see '\n' as our end-of-message delimiter.
                request_string += buf;
                if (request_string.find ('\n') != std::string::npos)
                    break;

                // If we collect more than 2048 bytes of data, truncate & just run with it
                // The key/value parsing is designed to catch malformated params
                if (request_string.length () > 2048) {
                    fprintf (stderr, SD_WARNING "http query too long: turncating\n");
                    break;
                }
            }
        }
    }

    // Get all key/value pairs from the http query string
    STRING_PAIR_LINKED_LIST params;
    if (parseRequestQuery (request_string, params))
        //sendReplyError ();
        fprintf (stderr, SD_ERR "parse error 12\n");


    // Link supporting library here to support get/set calling functionality
    // TODO: Add function ptr map based on path, if parsed, to allow diff lib's to be used based on
    // URL tag
    //std::string result;
    //if (getData(params, result))
    //    sendReplyError ();

    //std::string result;
    //process_request_helper(get, request_string, result);

    //Send the doctype XML header and the body of the XML response.
    //std::string doctype =
    //"HTTP/1.1 200 OK\nContent-Type: text\ncharset=UTF-8\nConnection: close\r\n\r\n";
    //send(fd, doctype.c_str(), doctype.length(), 0);
    //send(fd, result.c_str(), result.size(), 0);

    shutdownTcpSocket (fd);
}
/*** End Supporting Functions ***/


/*** MAIN ***/
int apiMainLoop ()
{
    // Start a server type socket
    int sockfd = createTcpSocket (RECV_SOCKET_IP, RECV_SOCKET_PORT);
    if (sockfd < 0) {
        fprintf (stderr, SD_ERR "failed to get socket lock\n");
        exit (EXIT_EADDRINUSE);
    }

    // The backlog argument provides a hint to the implementation which the implementation shall
    // use to limit the number of outstanding connections in the socket's listen queue. Current
    // versions of the Linux kernel round it up to the next highest power of two, with a minimum
    // of 16. https://stackoverflow.com/questions/5111040/listen-ignores-the-backlog-argument
    if (listen (sockfd, 16) != 0) {
        fprintf (stderr, SD_ERR "socket listen failed: %s\n", strerror (errno));
        close (sockfd);
        exit (EXIT_EOPNOTSUPP);
    }

    fprintf (stderr, SD_INFO "APId is ready!\n");

    // Main Loop - Blocks until signal recv'd
    while (!shutdown_requested) {
        struct sockaddr_in remote_addr;
        socklen_t addr_len;

        addr_len = sizeof (struct sockaddr_in);
        const int newfd = accept (sockfd, (struct sockaddr *) &remote_addr, &addr_len);
        if (newfd >= 0)
            // TODO: Spin off thread
            processRequest (newfd);
        else if (errno != EINTR) {
            //This shouldn't happen
            fprintf (stderr, SD_ERR "socket accept failed: %s\n", strerror (errno));
            close (sockfd);
            return EXIT_ESOCKTNOSUPPORT;
        }
    }

    fprintf (stderr, SD_NOTICE "API daemon shutdown requested...\n");

    close (sockfd);
    return EXIT_SUCCESS;
}

/**
 * @brief The main application init
 *
 * @return  0 - If the application completed successfully.
 *          255 - If there was an error executing the application.
 */
int main ()
{
    fprintf (stderr, SD_NOTICE "Starting APId\n");

    setupSignalHandlers ();

    return apiMainLoop ();
}
/*** End MAIN ***/
