
#pragma once

#include "lib/defs.hh"

#include <vector>

#include "lib/str.hh"


#include "scene/physics.hh"

#include "scene/tag_phy.hh"

#include "lib/pair.hh"

struct ManagerPhysics
{
    IDPhy phy_id_count = 1;

    std::vector<Pair<IDPhy, Physics>> data_dyn;
    std::vector<Pair<IDPhy, Physics>> data_stat;

    IDPhy       new_id();
    TagPhy      new_tagp();

    IDPhy       push_physics(Physics& physics);
    Physics*    find_physics(IDPhy physics_id);


    void respond_dyn_dyn(Physics& D, Physics& S, f32 dt_s)
    {
    }

    void respond_dyn_stat(Physics& D, Physics& S, f32 dt_s)
    {
        float dt10 = dt_s * 0.1f;

        int back_i=1;
        int backed_max = 11;

        // Unit vector from static to dynamic body
        f3 diff_normalized = (D.p - S.p).unit();


        // Also assumes Y is not moved by user.
        for(; back_i<backed_max; back_i++)
        {
            D.p -= D.v * dt_s;
            // D.p += diff_normalized*0.01f;

            D.update_isector(D.p);
            if(Physics::isect( D, S ))
                continue;
            break;
        }
        
        // reflect velocity of dynamic body
        Physics::reflect_dynamic_static(D, S);

        // Return body same number of time steps as it was deep in intersection
        D.p += D.v * dt10 * (float)back_i;

        // Move dynamic along diff vector if still intersecting
        backed_max = 11;
        while(Physics::isect( D, S ) && backed_max-- > 0)
        {
            // D.p += D.v * dt10;
            // D.update_isector(D.p);
            D.p += diff_normalized * 0.01f;
            D.update_isector(D.p);
        }

        // Energy loss
        D.v *= 0.9f;
    }

    void update(f32 dt_s)
    {
        if(data_dyn.empty())
            return;

        // update dynamic
        for(size_t i=0; i< data_dyn.size(); i++)
            data_dyn[i].YY.update_dynamic(dt_s);
        
        // collide dynamic-dynamic
        for(size_t i=0; i< data_dyn.size()-1; i++)
        {
            for(size_t j=i+1; j<data_dyn.size(); j++)
            {
                Physics& phy   = data_dyn[i].YY;
                Physics& other = data_dyn[j].YY;

                if(Physics::isect(phy, other))
                    respond_dyn_dyn(phy, other, dt_s);
            }
        }

        // collide dynamic-static
        for(size_t i=0; i< data_dyn.size(); i++)
        {
            for(size_t j=0; j<data_stat.size(); j++)
            {
                Physics& phy  = data_dyn[i].YY;
                Physics& stat = data_stat[j].YY;

                if(Physics::isect(phy, stat))
                    respond_dyn_stat(phy, stat, dt_s);
            }
        }

    }
};




