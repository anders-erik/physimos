#ifndef PSO_LOADER_HH
#define PSO_LOADER_HH

#include <string>

#include "resources.hh"

namespace res {

    class PsoLoader {
        public:
            PsoLoader() {};
            ~PsoLoader() {};

            ModelVertStucture vertStructure;
            std::vector<float> vertexFloatBuffer;
            int vertexCount = 0;
    };

    PsoLoader* loadPsoModel(std::string _path);

}

#endif