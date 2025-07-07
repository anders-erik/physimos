#pragma once

#include "math/const.hh"
#include "math/transform.hh"
#include "math/quarternion.hh"

#include "scene/tago.hh"
#include "scene/object.hh"

struct CameraPerspective
{
    m4f4 matrix; // Full perspective matrix. Set by calling 'update_matrix()'

    float fov = 0.785f;
    float width = 600.0f;
    float height = 400.0f;
    float zn = 0.1f;
    float zf = 100.0f;

    void set_fov(int new_width, int new_height);
    float AR();
    
    m4f4& update_matrix();
};



struct OrbitalView
{
    f3 rotational_center;   // the point about which the camera is orbiting
    m4f4 matrix;            // Full view matrix. Set by calling 'update_matrix()'

    float rho   =  8.0f; // Radius
    float theta =  4.5f; // x-y plane
    float phi   =  1.0f; // z-axis
    // float rho   =  3.0f; // Radius
    // float theta =  -PIHf; // x-y plane
    // float phi   =  0.0f; // z-axis

    float rho_min       = 0.5f;
    float rho_max       = 40.0f;
    float phi_min       = 0.01f;
    float phi_max       = 3.13f;


    float rho_clamp(float _rho);
    float phi_clamp(float _phi);

    void rho_change(float delta);
    void theta_change(float delta);
    void phi_change(float delta);

    m4f4& update_matrix();
};





struct CameraOrbital
{
    CameraPerspective perspective;
    OrbitalView view;

    /** Makes view and perspective matrices reflect most recent values. */
    void update();

    f3 get_pos();
    f3 get_forward();
    f3 get_right();
    f3 get_up();

    void forward(float delta);
    void backward(float delta);
    void left(float delta);
    void right(float delta);
    void up(float delta);
    void down(float delta);

    void print();
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


struct CameraObject
{
    Object              object;
    CameraView          view;
    CameraPerspective   perspective;

    CameraState         state;

    void set_free();
    void set_orbit_center();
    void set_orbit_tag(TagO new_tag);

    void update_matrices();
};
