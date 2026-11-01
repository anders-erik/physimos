#pragma once

#include "str.hh"
#include "arr.hh"

struct StrUtil
{

    static Str ptr_to_hex(void* ptr)
    {
        char string[20] {0};
        sprintf(string, "%p", (void *) ptr);
        return Str{string};
    }

    /** Split into array at each matching char. */
    static Arr<Str> split(Str str, char ch)
    {
        Arr<Str> arr;
        SizeStr last_match_i = 0;

        for(SizeStr i=0; i<str.size(); i++)
        {
            if(str[i] == ch)
            {
                arr.push_back(str.substr(last_match_i, i - last_match_i));
                i++;
                last_match_i = i;
            }
        }

        // if(last_match_i != str.size()-1)
        arr.push_back(str.substr(last_match_i, str.size() - last_match_i));

        return arr;
    }

    static Str to_str(bool b)
    {
        return b ? "true" : "false";
    }
};
