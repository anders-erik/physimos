#ifndef LOGGER_HH
#define LOGGER_HH

#include <string>

namespace logger {

    typedef enum Logtype {
        Models,
        Textures,
    } Logtype;

    void init();

    void log(Logtype logtype, std::string logString);
}


#endif