#pragma once


typedef unsigned int OID;




struct TagO 
{
    enum Type {
        Base,
        Quad,
        None,
    };

    OID oid = 0;
    Type type = None;


    TagO() = default;
    // TagO(OID oid)               : oid {oid}                 {};
    // TagO(Type type)             : type {type}               {};
    TagO(OID oid, Type type)    : oid {oid}, type {type}    {};
    // TagO(const TagO& rhs) : oid {rhs.oid}, type {rhs.type}  {};

    // TagO& operator=(const TagO& rhs) {oid = rhs.oid; type = rhs.type; return *this; };

    bool operator!=(const TagO& rhs) { return !(*this == rhs); }
    bool operator==(const TagO& rhs)
    {
        return  oid      == rhs.oid      &&
                type    == rhs.type;
    }

    bool is_quad() {return type == Quad ? true : false;}
    bool is_base() {return type == Base ? true : false;}

};

