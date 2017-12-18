#include <example.h>
#include <helpers/aliases.h>

#define HEADER "HTTP/1.1 200 OK\nContent-Type: text\ncharset=UTF-8\nConnection: close\r\n\r\n"

enum KEYS {
    DOG = 0,
    CAT,
    PIG,
    MONKEY,
    COW
};

typedef int (*callback)(DFU_STATES last_state, DFU_STATES& final_state, DFU_STATUS& status);

/**
 * @brief The main application init
 *
 * @return  0 - If the application completed successfully.
 *          255 - If there was an error executing the application.
 */
const std::unordered_map<std::string, KEYS> control_map = {
    {"Dog" , },
    {"Cat" , DISABLE_NON_CONFIG_ALL_CMDS},
    {"Pig" , DISABLE_NON_CONFIG_ALL_CMDS},
    {"Monkey" , WIFI_CONFIG},
    {"Cow" , WIFI_CONFIG}
};


int doApiWork (const std::string& key, const std::string& val)
{

}

//std::string result;
//if (getData(params, result))
//    sendReplyError ();

//std::string result;
//process_request_helper(get, request_string, result);

//Send the doctype XML header and the body of the XML response.
//std::string result = "<test>asdfsfd</test>";
//std::string doctype =
//"HTTP/1.1 200 OK\nContent-Type: text\ncharset=UTF-8\nConnection: close\r\n\r\n";
//send(fd, doctype.c_str(), doctype.length(), 0);
//send(fd, result.c_str(), result.size(), 0);
