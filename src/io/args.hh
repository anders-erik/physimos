#pragma once


#include "lib/str.hh"
#include "lib/str_util.hh"

#include "lib/arr.hh"

struct Arg 
{
    Str key;
    Str value;
};

/** Parse command line arguments into key-value pairs */
Arr<Arg> parse_args(int argc, char** argv)
{
    Arr<Arg> args;
    for(int i = 0; i < argc; i++)
    {
        Str raw_arg_str = Str(argv[i]);
        
        Arr<Str> KV_pairs = StrUtil::split(raw_arg_str, ' ');

        for(uint k = 0; k < KV_pairs.count(); k++)
        {
            Str kv_str = KV_pairs[k];
            Arr<Str> kv_split = StrUtil::split(kv_str, '=');

            if(kv_split.count() == 2)
            {
                Str key = kv_split[0];
                Str value = kv_split[1];

                Arg arg;
                arg.key = key.trim();
                arg.key.cut_to_substr(1, arg.key.size()); // remove leading '-'
                arg.value = value.trim();

                args.push_back(arg);
            }
            else
            {
                Str throw_str = Str("Error parsing arg: ") + raw_arg_str.to_c_str();
                throw std::runtime_error(throw_str.to_c_str());
            }

        }

    }
    return args;
}



