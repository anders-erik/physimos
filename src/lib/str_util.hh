#pragma once

#include "lib/str.hh"
#include "lib/arr.hh"

struct StrUtil
{

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

        arr.push_back(str.substr(last_match_i, str.size() - last_match_i));

        return arr;
    }
};
