#ifndef PLIB_STRING_HH
#define PLIB_STRING_HH

#include <string>
#include <sstream>
#include <vector>

typedef std::string pstring;


namespace plib {


std::vector<pstring>    pstring_split(pstring str, char delimiter);
std::vector<pstring>    pstring_split_and_trim(pstring str, char delimiter);
void                    pstring_trim(pstring& str);

bool cstr_is_empty_or_null(char* cstr);

}



namespace plib::std_string {

std::vector<std::string>    split(std::string str, char delimiter);
void                        trim(std::string& str);

}

#endif