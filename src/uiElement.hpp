#include <vector>
#include <string>

#include "Types.hpp"

class UiElement {
    public:
        UiElement() {};
    
        int vertexCount = 6;
        // float vertices[6*6];
        // float color[3] = {1.0f, 1.0f, 1.0f};
        Vec3 color = { 1.0f, 1.0f, 1.0f };
        int activated = 0;
        std::string name;
        std::string type;
        int height;
        int width;
        int x;
        int y;
        std::vector<float> squareVertices = {
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // top-right
            0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // top-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // top-right
            0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
            1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        };


};

