
#include "io/json/json.hh"
#include "json_types.hh"

#include "json_lexer.hh"
#include "json_parser.hh"
#include "json_serialize.hh"



OptPtr<Json> Json::
object_find(j_string str_to_match)
{
    if(type != json_type::object)
            throw std::runtime_error("Error: tried to get kv from non object variant.");

    auto search = get_object().find(str_to_match);
    if(search->first == str_to_match)
        return {&(search->second)};
    else
        return {};
}



void Json::object_push(const j_kv & kv)
{
    get_object().insert(kv);
    // get_object().emplace(kv);
}

void Json::try_parse(const Str& json_source)
{
    var = nullptr;

    JsonLexer lexer;
    lexer.lex(json_source);

    JsonParser parser (json_source, lexer.tokens); 
    *this = parser.parse();
}

Str Json::try_serialize()
{
    JsonSerializer serializer;
    // serializer.set_config({});

    return {serializer.serialize(*this)};
}



ResMove<Json> Json::
parse(const Str& json_source)
{
    try
    {
        Json j_var;
        j_var.try_parse(json_source);
        return {std::move(j_var)};
    }
    catch(const std::exception& e)
    {
        Err err;
        err.message = e.what();
        return {MV(err)};
    }
}

ResMove<Str> Json::
serialize(const Json& json_var)
{
    try
    {
        JsonSerializer serializer;
        return {MV(serializer.serialize(json_var))};
    }
    catch(const std::exception& e)
    {
        Err err;
        err.message = e.what();
        return {MV(err)};
    }
}

