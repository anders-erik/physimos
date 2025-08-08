
#pragma once

/** Property ID */
typedef unsigned int    IDPhy;

struct TagPhy
{
    enum Type
    {
        Null,
    };

    IDPhy  id      = 0;
    Type    type    = Null;

    TagPhy() = default;
    TagPhy(IDPhy id)
        : id    {id}
        , type  {Null}
    {}
    TagPhy(IDPhy id, Type type)
        : id    {id}
        , type  {type}
    {}


    bool is_null() { return type == Null;}
};
