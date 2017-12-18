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


/*** Errors & logging info ***/
#define SD_EMERG   "<0>"  /* system is unusable */
#define SD_ALERT   "<1>"  /* action must be taken immediately */
#define SD_CRIT    "<2>"  /* critical conditions */
#define SD_ERR     "<3>"  /* error conditions */
#define SD_WARNING "<4>"  /* warning conditions */
#define SD_NOTICE  "<5>"  /* normal but significant condition */
#define SD_INFO    "<6>"  /* informational */
#define SD_DEBUG   "<7>"  /* debug-level messages */

#define EXIT_SUCCESS          0   /* No Error */
#define EXIT_EPERM            1   /* Operation not permitted */
#define EXIT_ENOENT           2   /* No such file or directory */
#define EXIT_ESRCH            3   /* No such process */
#define EXIT_EINTR            4   /* Interrupted system call */
#define EXIT_EIO              5   /* I/O error */
#define EXIT_ENXIO            6   /* No such device or address */
#define EXIT_E2BIG            7   /* Argument list too long */
#define EXIT_ENOEXEC          8   /* Exec format error */
#define EXIT_EBADF            9   /* Bad file number */
#define EXIT_ECHILD           10  /* No child processes */
#define EXIT_EAGAIN           11  /* Try again */
#define EXIT_ENOMEM           12  /* Out of memory */
#define EXIT_EACCES           13  /* Permission denied */
#define EXIT_EFAULT           14  /* Bad address */
#define EXIT_ENOTBLK          15  /* Block device required */
#define EXIT_EBUSY            16  /* Device or resource busy */
#define EXIT_EEXIST           17  /* File exists */
#define EXIT_EXDEV            18  /* Cross-device link */
#define EXIT_ENODEV           19  /* No such device */
#define EXIT_ENOTDIR          20  /* Not a directory */
#define EXIT_EISDIR           21  /* Is a directory */
#define EXIT_EINVAL           22  /* Invalid argument */
#define EXIT_ENFILE           23  /* File table overflow */
#define EXIT_EMFILE           24  /* Too many open files */
#define EXIT_ENOTTY           25  /* Not a typewriter */
#define EXIT_ETXTBSY          26  /* Text file busy */
#define EXIT_EFBIG            27  /* File too large */
#define EXIT_ENOSPC           28  /* No space left on device */
#define EXIT_ESPIPE           29  /* Illegal seek */
#define EXIT_EROFS            30  /* Read-only file system */
#define EXIT_EMLINK           31  /* Too many links */
#define EXIT_EPIPE            32  /* Broken pipe */
#define EXIT_EDOM             33  /* Math argument out of domain of func */
#define EXIT_ERANGE           34  /* Math result not representable */
#define EXIT_EDEADLK          35  /* Resource deadlock would occur */
#define EXIT_ENAMETOOLONG     36  /* File name too long */
#define EXIT_ENOLCK           37  /* No record locks available */
#define EXIT_ENOSYS           38  /* Function not implemented */
#define EXIT_ENOTEMPTY        39  /* Directory not empty */
#define EXIT_ELOOP            40  /* Too many symbolic links encountered */
#define EXIT_ENOMSG           42  /* No message of desired type */
#define EXIT_EIDRM            43  /* Identifier removed */
#define EXIT_ECHRNG           44  /* Channel number out of range */
#define EXIT_EL2NSYNC         45  /* Level 2 not synchronized */
#define EXIT_EL3HLT           46  /* Level 3 halted */
#define EXIT_EL3RST           47  /* Level 3 reset */
#define EXIT_ELNRNG           48  /* Link number out of range */
#define EXIT_EUNATCH          49  /* Protocol driver not attached */
#define EXIT_ENOCSI           50  /* No CSI structure available */
#define EXIT_EL2HLT           51  /* Level 2 halted */
#define EXIT_EBADE            52  /* Invalid exchange */
#define EXIT_EBADR            53  /* Invalid request descriptor */
#define EXIT_EXFULL           54  /* Exchange full */
#define EXIT_ENOANO           55  /* No anode */
#define EXIT_EBADRQC          56  /* Invalid request code */
#define EXIT_EBADSLT          57  /* Invalid slot */
#define EXIT_EBFONT           59  /* Bad font file format */
#define EXIT_ENOSTR           60  /* Device not a stream */
#define EXIT_ENODATA          61  /* No data available */
#define EXIT_ETIME            62  /* Timer expired */
#define EXIT_ENOSR            63  /* Out of streams resources */
#define EXIT_ENONET           64  /* Machine is not on the network */
#define EXIT_ENOPKG           65  /* Package not installed */
#define EXIT_EREMOTE          66  /* Object is remote */
#define EXIT_ENOLINK          67  /* Link has been severed */
#define EXIT_EADV             68  /* Advertise error */
#define EXIT_ESRMNT           69  /* Srmount error */
#define EXIT_ECOMM            70  /* Communication error on send */
#define EXIT_EPROTO           71  /* Protocol error */
#define EXIT_EMULTIHOP        72  /* Multihop attempted */
#define EXIT_EDOTDOT          73  /* RFS specific error */
#define EXIT_EBADMSG          74  /* Not a data message */
#define EXIT_EOVERFLOW        75  /* Value too large for defined data type */
#define EXIT_ENOTUNIQ         76  /* Name not unique on network */
#define EXIT_EBADFD           77  /* File descriptor in bad state */
#define EXIT_EREMCHG          78  /* Remote address changed */
#define EXIT_ELIBACC          79  /* Can not access a needed shared library */
#define EXIT_ELIBBAD          80  /* Accessing a corrupted shared library */
#define EXIT_ELIBSCN          81  /* .lib section in a.out corrupted */
#define EXIT_ELIBMAX          82  /* Attempting to link in too many shared libraries */
#define EXIT_ELIBEXEC         83  /* Cannot exec a shared library directly */
#define EXIT_EILSEQ           84  /* Illegal byte sequence */
#define EXIT_ERESTART         85  /* Interrupted system call should be restarted */
#define EXIT_ESTRPIPE         86  /* Streams pipe error */
#define EXIT_EUSERS           87  /* Too many users */
#define EXIT_ENOTSOCK         88  /* Socket operation on non-socket */
#define EXIT_EDESTADDRREQ     89  /* Destination address required */
#define EXIT_EMSGSIZE         90  /* Message too long */
#define EXIT_EPROTOTYPE       91  /* Protocol wrong type for socket */
#define EXIT_ENOPROTOOPT      92  /* Protocol not available */
#define EXIT_EPROTONOSUPPORT  93  /* Protocol not supported */
#define EXIT_ESOCKTNOSUPPORT  94  /* Socket type not supported */
#define EXIT_EOPNOTSUPP       95  /* Operation not supported on transport endpoint */
#define EXIT_EPFNOSUPPORT     96  /* Protocol family not supported */
#define EXIT_EAFNOSUPPORT     97  /* Address family not supported by protocol */
#define EXIT_EADDRINUSE       98  /* Address already in use */
#define EXIT_EADDRNOTAVAIL    99  /* Cannot assign requested address */
#define EXIT_ENETDOWN         100 /* Network is down */
#define EXIT_ENETUNREACH      101 /* Network is unreachable */
#define EXIT_ENETRESET        102 /* Network dropped connection because of reset */
#define EXIT_ECONNABORTED     103 /* Software caused connection abort */
#define EXIT_ECONNRESET       104 /* Connection reset by peer */
#define EXIT_ENOBUFS          105 /* No buffer space available */
#define EXIT_EISCONN          106 /* Transport endpoint is already connected */
#define EXIT_ENOTCONN         107 /* Transport endpoint is not connected */
#define EXIT_ESHUTDOWN        108 /* Cannot send after transport endpoint shutdown */
#define EXIT_ETOOMANYREFS     109 /* Too many references: cannot splice */
#define EXIT_ETIMEDOUT        110 /* Connection timed out */
#define EXIT_ECONNREFUSED     111 /* Connection refused */
#define EXIT_EHOSTDOWN        112 /* Host is down */
#define EXIT_EHOSTUNREACH     113 /* No route to host */
#define EXIT_EALREADY         114 /* Operation already in progress */
#define EXIT_EINPROGRESS      115 /* Operation now in progress */
#define EXIT_ESTALE           116 /* Stale NFS file handle */
#define EXIT_EUCLEAN          117 /* Structure needs cleaning */
#define EXIT_ENOTNAM          118 /* Not a XENIX named type file */
#define EXIT_ENAVAIL          119 /* No XENIX semaphores available */
#define EXIT_EISNAM           120 /* Is a named type file */
#define EXIT_EREMOTEIO        121 /* Remote I/O error */
#define EXIT_EDQUOT           122 /* Quota exceeded */
#define EXIT_ENOMEDIUM        123 /* No medium found */
#define EXIT_EMEDIUMTYPE      124 /* Wrong medium type */
#define EXIT_ECANCELED        125 /* Operation Canceled */
#define EXIT_ENOKEY           126 /* Required key not available */
#define EXIT_EKEYEXPIRED      127 /* Key has expired */
#define EXIT_EKEYREVOKED      128 /* Key has been revoked */
#define EXIT_EKEYREJECTED     129 /* Key was rejected by service */
#define EXIT_EOWNERDEAD       130 /* Owner died */
#define EXIT_ENOTRECOVERABLE  131 /* State not recoverable */
/*** End Errors & logging info ***/


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
            fprintf (stderr, SD_INFO "param - key:%s, value:%s\n", param.first.c_str(), param.second.c_str());
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
    //std::string doctype = "HTTP/1.1 200 OK\nContent-Type: text/xml\ncharset=UTF-8\nConnection: close\r\n\r\n";
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
