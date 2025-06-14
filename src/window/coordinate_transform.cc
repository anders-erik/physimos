
#include "coordinate_transform.hh"


void PWCoordinateTransformer::set_constants(PWCoordinatesInput input)
{
        win_sc.x = (float) input.window_size_sc.x;
        win_sc.y = (float) input.window_size_sc.y;

        // win_sc.print("win_sc: ");

        cs = input.content_scale;

        mon_px = input.monitor_size_px;
        mon_mm = input.monitor_size_mm;

        dm_dp.x = mon_mm.x / mon_px.x;
        dm_dp.y = mon_mm.y / mon_px.y;

        M_i_s.set_to_identity();
        M_i_s.y.y = -1.0f;
        M_i_s.y.z = win_sc.y;

        M_s_n.set_to_identity();
        M_s_n.x.x = 1 / win_sc.x;
        M_s_n.y.y = 1 / win_sc.y;

        M_s_p.set_to_identity();
        M_s_p.x.x = cs.x; // TODO: make sure its not 1 / cs
        M_s_p.y.y = cs.y; // TODO: make sure its not 1 / cs
        
        M_p_m.set_to_identity();
        M_p_m.x.x = dm_dp.x;
        M_p_m.y.y = dm_dp.y;
    }