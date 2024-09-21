
#include "Simulator.hpp"

#include <iostream>
#include <fstream>




namespace Sim {

    Simulator* sim_1_ptr = nullptr;
    Simulator sim_1;

    void init(){
        std::cout << "HELLO FROM SIM INIT! " << std::endl;
        
        sim_1_ptr = new Simulator();
        // sim_1_ptr = &Simulator_1;
        std::cout << "Sim::sim_1_ptr = " << sim_1_ptr << std::endl;
        
        // Temporary reference object for local convenience
        Simulator& simulator_1 = *sim_1_ptr;

        std::cout << "Sim::sim_1_ptr = " << sim_1_ptr << std::endl;
        

        simulator_1.name = "vertithrow_1";

        simulator_1.saveFileName = "vertithrow_1.psos";

        simulator_1.t0 = 0.0;
        simulator_1.tf = 1.0;
        simulator_1.dt = 0.1;

        simulator_1.pos_0 = {0.0, 0.0, 10.0};
        simulator_1.vel_0 = { 0.0, 0.0, 0.0 };

        simulator_1.acc = {0.0, 0.0, -9.8};

        // simulator_1.mass = 10;
    }


    void compute(){
        // std::cout << "sim_1_ptr = " << sim_1_ptr << std::endl;
        // Simulator& _simulator = *sim_1_ptr;
        Sim::sim_1_ptr->runSimulation();
        // Sim::sim_1_ptr->name;
        // std::cout << "Sim::sim_1_ptr->t0 = " << Sim::sim_1_ptr->t0 << std::endl;
        // std::cout << "_simulator.t0 = " << _simulator.t0 << std::endl;
        // _simulator.runSimulation();
    }



    void Simulator::runSimulation(){
        
        std::cout << " Setting up Simulation " << name << std::endl;

        int steps = (int)((tf-t0)/dt);

        // First step
        TimeStep& timestep = SimData.emplace_back();
        timestep.i = 0;
        timestep.t = t0;
        timestep.pos = pos_0;
        timestep.vel = vel_0;
        timestep.acc = {0.0, 0.0, 0.0};
        // timestep.f = f;

        std::cout << " Starting Simulation " << std::endl;

        for(int i = 1; i < steps; i++){
            double i_d = (double) i;
            // std::cout << "i_d = " << i_d << std::endl;
            TimeStep& _timestep = SimData.emplace_back();
            // _timestep = 
            _timestep.i = i;
            _timestep.t = t0 + dt * i_d;
            _timestep.acc = acc;
            _timestep.vel = { 0.0, 0.0, SimData[i - 1].vel.z + acc.z };
            _timestep.pos = { 0.0, 0.0, SimData[i - 1].pos.z + SimData[i - 1].vel.z };
            // std::cout << "timestep.t = " << _timestep.t << std::endl;
        }

        std::cout << " Finished Simulation " << std::endl;
        
        std::ofstream file(simSaveDirectory + saveFileName);

        if (file.is_open()) {
            file << "i" << delim << "t" << delim;
            file << "pos.x" << delim << "pos.y" << delim << "pos.z" << delim;
            file << "vel.x" << delim << "vel.y" << delim << "vel.z" << delim;
            file << "acc.x" << delim << "acc.y" << delim << "acc.z" << "\n";
            
            for (TimeStep& _timeStep : SimData) {
                // std::cout << "_timeStep.t" << _timeStep.t << std::endl;
                file << _timeStep.i << delim << _timeStep.t << delim;
                file << _timeStep.pos.x << delim << _timeStep.pos.y << delim << _timeStep.pos.z;
                file << _timeStep.vel.x << delim << _timeStep.vel.y << delim << _timeStep.vel.z;
                file << _timeStep.acc.x << delim << _timeStep.acc.y << delim << _timeStep.acc.z << "\n";
                
                // file << _timeStep.acc.z << "\n";
            }
            // for (const auto& str : vec) {
                 
            // }
           
            file.close();
        }
        else {
            std::cerr << "Unable to open file";
        }
        

        std::cout << " Simulation Output written to : " << saveFileName << std::endl;

    }

}