#ifndef PLIB_STRING_HH
#define PLIB_STRING_HH

#include <string>
#include <sstream>
#include <vector>

typedef std::string pstring;


namespace plib {

bool cstr_is_empty_or_null(char* cstr);

}



namespace plib::std_string {

std::vector<std::string>    split(std::string str, char delimiter);
void                        trim(std::string& str);

}

#endif