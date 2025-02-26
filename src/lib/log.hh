#ifndef PLIB_LOG_HH
#define PLIB_LOG_HH

#include <string>

namespace plib {

std::string get_current_timestring();
std::string getCurrentDate();
std::string getCurrentTime();

enum LogLevel {
    INFO,
    DEBUG,
    WARN,
    ERROR,
};

enum LogScope {
    MAIN,
    IMAGE,
    LIB,
    SCENE,
    UI,
    TEST,
};

enum LogEvent {
    NONE,
    ENTERED_MAIN,
};

enum LogError {
    OBJ_PARSING,
};

#define plog plog_info

std::string get_plog_level_string(LogLevel logLevel);
std::string get_plog_scope_string(LogScope logScope);
std::string get_plog_event_string(LogEvent logEvent);
std::string get_plog_error_string(LogError logErrors);


void plog_info(LogScope logScope, std::string message);
void plog_info(LogScope logScope, LogEvent logEvent, std::string message);

void plog_error(LogScope logScope, LogError logError, std::string message);
void plog_error(std::string scope, std::string error_type, std::string message);

}

#endif