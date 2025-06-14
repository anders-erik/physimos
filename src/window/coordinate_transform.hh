
#pragma once


#include "math/vecmat.hh"



struct PWCoordinatesInput{
    f2 window_size_sc;
    f2 content_scale;
    f2 monitor_size_mm; 
    f2 monitor_size_px;
};

// Coordinate Transformation Matrices -- All values are relative to window origins!
struct PWCoordinateTransformer {

    /** All available Physimos window units. */
    enum class PWUnit {
        SC,     /** (virtual) Sceen Coordinates  */
        PX,     /** Real Pixels */
        N,      /** Normalized */
        MM,     /** Millimeters */
    };
    /** All but the glfw-input system share an origin and have parallel basis axes: bottom left of window; y is up.  */
    enum class PWCoordinateSystems {
        INPUT,          /** GLFW input coordinate in content area coordinate. PWUnit : SC */
        SANE,           /** PWUnit : SC */
        PIXELS,         /** PWUnit : PX */
        NORMALIZED,     /** PWUnit : N */
        MILLIMETERS,    /** PWUnit : MM */
    };

    void set_constants(PWCoordinatesInput input);

    /** Input to sane */
    f2 i_s(f2 i){
        return M_i_s.mult_vec(i);
    }
     /** Sane to normalized */
    f2 s_n(f2 s){
        return M_s_n.mult_vec(s);
    }
     /** Sane to pixels */
    f2 s_p(f2 s){
        return M_s_p.mult_vec(s);
    }
    /** Sane to millimeters */
    f2 s_m(f2 s){
        return M_p_m.mult_vec(s_p(s));
    }

// private:

    m3f3 M_i_s;   /** From glfw input to sane : ( (1 0 0) (0 -1 win_h) (0 0 1) ) */
    m2f2 M_s_n;   /** From sane to Normalized : ( (1/win_w 0) (0 1/win_h) ) */
    m2f2 M_s_p;   /** From sane to pixels : ( (CSx 0) (0 CSy) ) */
    m2f2 M_p_m;   /** From pixels to Millimeters : ( (dp_dm_x 0) (0 dp_dm_y) ) */

    f2 cs;      /** Content scale */
    f2 win_sc;  /** Window size, screen coordinated */
    f2 mon_px;  /** Monitor size, pixels */
    f2 mon_mm;  /** Monitor size, millimeters */
    f2 dm_dp;   /** pixels per millimeters : wm_px / wm_mm */

};