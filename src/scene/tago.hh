#pragma once


typedef unsigned int OID;




struct TagO 
{
    enum Type {
        Base,
        Quad,
        Lamp,
        Camera,
        Null,
    };

    OID oid = 0;
    Type type = Null;


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

    bool is_quad() const  {return type == Quad;}
    bool is_base() const  {return type == Base;}
    bool is_lamp() const  {return type == Lamp;}
    bool is_camera() const  {return type == Camera;}
    bool is_null() const  {return type == Null;}
    bool not_null() const  {return type != Null;}

};

