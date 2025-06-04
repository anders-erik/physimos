#pragma once

#ifdef PH_LINUX

struct Str;

namespace OS {

bool file_exists(const char* path);
bool file_echo_overwrite_first_strsize_chars(const char* path, Str& str);
Str file_cat_as_str(const char* path);

}

#endif
