#pragma once

#include "math/const.hh"
#include "math/transform.hh"
#include "math/quarternion.hh"

#include "scene/tago.hh"
#include "scene/object.hh"


/** Three projection types: finite perspective, infinite perspective, and Othographic.
    Matrices are equal to GLTF specs. */
struct Projection
{
    struct Finite
    {
        float AR;
        float fov;
        float zn;
        float zf;
    };
    struct Infinite
    {
        float AR;
        float fov;
        float zn;
    };
    struct Ortho
    {
        float xmax;
        float ymax;
        float zn;
        float zf;
    };

    constexpr Finite    default_finite()    { return {1.5f, 0.785f, 0.1f, 100.0f};  }
    constexpr Infinite  default_infinite()  { return {1.5f, 0.785f, 0.1f};          }
    constexpr Ortho     default_ortho()     { return {30.0f, 20.0f, 0.1f, 100.0f};  }

    union Base
    {
        Finite      finite;
        Infinite    infinite;
        Ortho       ortho;
    };

    enum class Tag
    {
        Finite,
        Infinite,
        Ortho,
    };

    void set_viewport(int new_width, int new_height)
    {
        float ar = (float) new_width / (float) new_height;

        if(tag == Tag::Finite)
            base.finite.AR = ar;
        else if(tag == Tag::Infinite)
            base.infinite.AR = ar;
        else if(tag == Tag::Ortho)
            base.ortho.xmax = base.ortho.ymax * ar;
        else 
            throw "Updated camera viewport without a matching tag.";
        
        update_matrix();
    }

    float AR()
    {
        if(tag == Tag::Finite)
            return base.finite.AR;
        else if(tag == Tag::Infinite)
            return base.infinite.AR;
        else if(tag == Tag::Ortho)
            return base.ortho.xmax / base.ortho.ymax;
        else 
            throw "Get camera AR without a matching tag.";
    }

    bool is_perspective() { return tag == Tag::Finite || tag == Tag::Infinite; }

    void set_finite(const Finite& finite)
    {
        base.finite = finite;
        tag = Tag::Finite;
        update_matrix();
    }
    void set_finite()
    {
        base.finite = default_finite();
        tag = Tag::Finite;
        update_matrix();
    }
    void set_infinite(const Infinite& infinite)
    {
        base.infinite = infinite;
        tag = Tag::Infinite;
        update_matrix();
    }
    void set_infinite()
    {
        base.infinite = default_infinite();
        tag = Tag::Infinite;
        update_matrix();
    }
    void set_ortho(const Ortho& ortho)
    {
        base.ortho = ortho;
        tag = Tag::Ortho;
        update_matrix();
    }
    void set_ortho()
    {
        base.ortho = default_ortho();
        tag = Tag::Ortho;
        update_matrix();
    }

    m4f4& update_matrix();

    Base    base;
    Tag     tag;
    m4f4    matrix; // Full perspective matrix. Set by calling 'update_matrix()'
};




struct OrbitalContext
{

    // float rho   =  10.0f; // Radius
    // float theta =  0.0f; // x-y plane
    // float phi   =  PIHf; // z-axis

    float rho   =  8.0f; // Radius
    float theta =  4.5f; // x-y plane
    float phi   =  1.0f; // z-axis
    // float rho   =  3.0f; // Radius
    // float theta =  -PIHf; // x-y plane
    // float phi   =  0.0f; // z-axis

    float rho_min       = 0.3f;
    float rho_max       = 60.0f;
    float phi_min       = 0.01f;
    float phi_max       = 3.13f;

    void set_rho(float new_rho);
    void set_theta(float new_theta);
    void set_phi(float new_phi);

    float rho_clamp(float _rho);
    float phi_clamp(float _phi);

    void  rho_scale(float factor);
    void  theta_add(float delta);
    void  phi_add(float delta);
};

struct CameraView
{
    m4f4 matrix;

    OrbitalContext orbit_ctx;

    m4f4& calc_matrix(f3 pos, Quarternion rot);
};

struct CameraState
{
    using Bits = uint;
    static constexpr Bits FREE          = 0x0000;
    static constexpr Bits ORBIT         = 0x0001;
    static constexpr Bits FOLLOW        = 0x0002;
    static constexpr Bits RB            = 0x0004;

    static constexpr Bits ORB_CENTER    = ORBIT | 0x0100;
    static constexpr Bits ORB_TAG       = ORBIT | 0x0200;

    Bits bits = FREE;
    TagO tag  = TagO{}; // Tag to follow or orbit

    bool is_free()          {return bits == FREE;   }
    bool is_orbit()         {return bits &  ORBIT;  }
    bool is_follow()        {return bits &  FOLLOW; }
    bool is_rigid_body()    {return bits &  RB;     }

    bool is_orbit_center()    {return bits == ORB_CENTER; }
    bool is_orbit_tag()       {return bits == ORB_TAG; }

    bool has_tag() {return tag.not_null(); }
};


struct Camera
{
    Object      object;
    CameraView  view;
    Projection  projection;

    CameraState state;

    void set_free();
    void set_orbit_center();
    void set_orbit_tag(TagO new_tag);

    bool is_perspective()   { return projection.is_perspective(); }
    bool is_finite()        { return projection.tag == Projection::Tag::Finite;     }
    bool is_infinite()      { return projection.tag == Projection::Tag::Infinite;   }
    bool is_ortho()         { return projection.tag == Projection::Tag::Ortho;      }

    void update_matrices();
};
