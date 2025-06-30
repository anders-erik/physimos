
#include "io/json/json.hh"
#include "json_types.hh"



OptPtr<JsonVar> JsonVar::
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



void JsonVar::object_push(const j_kv & kv)
{
    get_object().insert(kv);
}

void JsonVar::parse(const Str& json_source)
{
    Str str = json_source;
    std::string json_src_string = str.to_c_str();

    variant_ = nullptr;

    JsonLexer lexer;
    lexer.lex(json_src_string);

    JsonParser parser (json_src_string, lexer.tokens); 
    *this = parser.parse();

}

ResMove<Str> JsonVar::serialize()
{
    JsonSerializer serializer;
    serializer.set_config({});

    return {serializer.serialize(*this).c_str()};
}