#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <string>
#include <vector>

#include "Types.hpp"


namespace Sim {

    void init();
    void compute();

    enum Simtypes {
        VertThrow1,

    };


    // TODO: change the Vec3 types!
    typedef struct TimeStep
    {
        int i;
        double t;
        Vec3 pos;
        Vec3 vel;
        Vec3 acc;
    } TimeStep;




    class Simulator {
        
        public:
        
            Simulator() {};
            
            std::string name;

            std::string simSaveDirectory = "/home/anders/dev/Physimos/resources/simulation/";
            std::string saveFileName = "";
            std::string delim = ", ";

            std::vector<TimeStep> SimData;
            
            Vec3 pos;
            Vec3 pos_0;
            Vec3 vel;
            Vec3 vel_0;
            Vec3 acc;

            Vec3 f;

            double mass;

            double t0;
            double tf;
            double dt;

            Simtypes simtype;

            TimeStep* nextTimeStep();



            void compute();
            
    };



    Simulator* getSim1Pointer();

}

#endif