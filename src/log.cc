#include <string>
#include <iostream>

// PLIB
#include "lib/fs.hh"

#include "log.hh"
#include "process_info.hpp"


extern std::string physimos_root_dir;

namespace plib {


#define LOGLEVEL_INFO_STR  "[INFO]  "
#define LOGLEVEL_DEBUG_STR "[DEBUG] "
#define LOGLEVEL_WARN_STR  "[WARN]  "
#define LOGLEVEL_ERROR_STR "[ERROR] "

#define LOGSCOPE_MAIN_STR  "_MAIN_  "
#define LOGSCOPE_IMAGE_STR "_IMAGE_ "
#define LOGSCOPE_LIB_STR   "_LIB_   "
#define LOGSCOPE_SCENE_STR "_SCENE_ "
#define LOGSCOPE_UI_STR    "_UI_    "
#define LOGSCOPE_TEST_STR  "_TEST_  "

#define LOGEVENT_NONE_STR           "_               "
#define LOGEVENT_ENTERED_MAIN_STR   "_ENTERED_MAIN_  "


#define LOGERROR_OBJ_PARSING_STR    "_OBJ_PARSING_   "


std::string get_plog_level_string(LogLevel logLevel){
    std::string level_string;

    switch (logLevel){
            
        case LogLevel::INFO:
            return LOGLEVEL_INFO_STR;   break;
        
        case LogLevel::DEBUG:
            return LOGLEVEL_DEBUG_STR;  break;

        case LogLevel::WARN:
            return LOGLEVEL_WARN_STR;   break;

        case LogLevel::ERROR:
            return LOGLEVEL_ERROR_STR;  break;
    }

    return "";
}

std::string get_plog_scope_string(LogScope logScope) {
    std::string level_string;

    switch (logScope) {

        case LogScope::MAIN:
            return LOGSCOPE_MAIN_STR;       break;

        case LogScope::IMAGE:
            return LOGSCOPE_IMAGE_STR;      break;

        case LogScope::LIB:
            return LOGSCOPE_LIB_STR;        break;

        case LogScope::SCENE:
            return LOGSCOPE_SCENE_STR;      break;

        case LogScope::UI:
            return LOGSCOPE_UI_STR;         break;

        case LogScope::TEST:
            return LOGSCOPE_TEST_STR;       break;

    }

    return "";
}

std::string get_plog_event_string(LogEvent logEvent){

    switch (logEvent)
    {
        case LogEvent::NONE:
            return LOGEVENT_NONE_STR;           break;

        case LogEvent::ENTERED_MAIN:
            return LOGEVENT_ENTERED_MAIN_STR;   break;

    }

    return "";
}


std::string get_plog_error_string(LogError logErrors){

    switch (logErrors){

        case LogError::OBJ_PARSING:
            return LOGERROR_OBJ_PARSING_STR;        break;

    }

    return "";
}


void plog_error(LogScope logScope, LogError logError, std::string message){

    std::string main_log_spath = "logs/main.log";
    std::string full_log_path = physimos_root_dir + "/" + main_log_spath;

    std::string level_string = get_plog_level_string(LogLevel::ERROR);
    std::string time_string = "2025-02-16T23:02:55 ";
    std::string scope_string = get_plog_scope_string(logScope);
    std::string error_string = get_plog_error_string(logError);
    std::string full_log_string = level_string + time_string + scope_string + error_string + "\"" + message + "\"";


    plib::fs_echo_append(full_log_path, full_log_string);

}




void plog_info(LogScope logScope, std::string message) {

    std::string main_log_spath = "logs/main.log";
    
    std::string level_string = get_plog_level_string(LogLevel::INFO);
    std::string time_string = "2025-02-16T23:02:55 ";
    std::string scope_string = get_plog_scope_string(logScope);
    std::string event_string = get_plog_event_string(LogEvent::NONE);
    std::string full_log_string = level_string + time_string + scope_string + event_string + "\"" + message + "\"";

    // ALWAYS LOG MAIN
    plib::fs_echo_append(physimos_root_dir + "/" + main_log_spath, full_log_string);
    
}

void plog_info(LogScope logScope, LogEvent logEvent, std::string message) {

    std::string main_log_spath = "logs/main.log";
    std::string full_log_path = physimos_root_dir + "/" + main_log_spath;

    std::string level_string = get_plog_level_string(LogLevel::INFO);
    std::string time_string = "2025-02-16T23:02:55 ";
    std::string scope_string = get_plog_scope_string(logScope);
    std::string event_string = get_plog_event_string(logEvent);
    std::string full_log_string = level_string + time_string + scope_string + event_string + "\"" + message + "\"";


    plib::fs_echo_append(full_log_path, full_log_string);
}


}