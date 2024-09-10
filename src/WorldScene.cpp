#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Simulation.hpp"
#include "Camera.hpp"

#include "WorldObject.hpp"
#include "obj_loader.hpp"

#include "WorldRenderer.hpp"

extern Shader worldShader;


#include "WorldScene.hpp"

#include "bmp_loader.hpp"

#include "Timing.hpp"

// WORLD OBJECTS

std::vector<WorldObject> worldObjects;

WorldObject worldGround1;
WorldObject* worldGround_pointer;

WorldObject worldCube1;
WorldObject worldCube_spin;
WorldObject cube_3_gravity;
WorldObject worldCube4_obj;

WorldObject worldTriangle1Texture;
WorldObject worldTriangle2_bounce;

WorldObject* worldTriangle2_simobj_pointer;


// TEXTURES
unsigned int mountainTexture;
unsigned int grayTexture;

// Old Black and white generated texture
// Generate a black and white test 'image'
int blackWhiteWidth = 100;
int blackWhiteHeight = 100;
// Generate black and white texture : 3 * blackWhiteWidth * blackWhiteHeight
unsigned char blackWhiteImageBuffer[30000]; // = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, };

#define GROUND_Z_0 0.0f


void ws_init(){

    wr_init();

    ws_loadTextures();

    ws_createWorldObjects();

    cam_init();

}


void ws_update() {

    // SIMULATION
    updateSimulation();
    // USE NEW WORLD OBJECTS
    // for (WorldObject& _worldObject : worldObjects) {
    //     if (_worldObject.name == "worldTriangle2_bounce")
    //         updateSimulation(_worldObject);

    // }

    ws_physics();

    // Update Camera matrices (view/persp.)
    cam_UpdateCam();


    // Render all world objects (world renderer)
    wr_render(worldObjects);

}



void ws_physics(){
    WorldObject& worldGround = *worldGround_pointer;


    for (WorldObject& _worldObject : worldObjects){
        // std::cout << "_worldObject.name =  " << _worldObject.name << std::endl;
        // std::cout << "_worldObject.rotation.x =  " << _worldObject.rotation.x << std::endl;
       
        if (_worldObject.name == "worldCube_spin")
            _worldObject.Rotate({ 0.03f, 0.01f, 0.0f });
        
        // GRAVITY
        if (_worldObject.gravityOn == 1){
            _worldObject.velocity.z += -9.8 * 0.0133; // dt = 0.0133s
            _worldObject.position.z += _worldObject.velocity.z * 0.0133;

            // Check ground bounce
            // worldObjectsCollide_aabb_z(worldGround, _worldObject);
            // if (_worldObject.position.z < (worldGround.position.z + _worldObject.offsetToBottom)) {
            if ( worldObjectCollidingWithGround_aabb_z(worldGround, _worldObject) == 1) {
                // std::cout << "COLLISION" << std::endl;
                
                _worldObject.position.z -= _worldObject.velocity.z * 0.0133;
                _worldObject.velocity.z = -_worldObject.velocity.z * 0.8;
            }
            // Vec3 deltaPos = {};
            // _worldObject.Translate({ 0.0f, 0.0f, -0.01f });
        }

        _worldObject.position.x += _worldObject.velocity.x * 0.0133;
        _worldObject.position.y += _worldObject.velocity.y * 0.0133;
        
    }
}


int worldObjectCollidingWithGround_aabb_z(WorldObject& ground, WorldObject& wo2) {

    // vertical test
    // if (wo2.position.z < (ground.position.z + wo2.offsetToBottom)) {
    if ( (wo2.position.z + wo2.boundingBox.z_min) < ground.boundingBox.z_max) {
        // if(wo)
        // std::cout << "wo2.boundingBox.z_min   " << wo2.boundingBox.z_min << std::endl;
        // std::cout << "ground.boundingBox.z_max   " << ground.boundingBox.z_max << std::endl;
        
        // x test
        // std::cout << "" << var << std::endl;
        // std::cout << "wo2.position.x + wo2.boundingBox.x_min = " << wo2.position.x + wo2.boundingBox.x_min << std::endl;
        // std::cout << "ground.boundingBox.x_max               = " << ground.boundingBox.x_max << std::endl;
        
        
        if ((wo2.position.x + wo2.boundingBox.x_min < ground.boundingBox.x_max) && (wo2.position.x + wo2.boundingBox.x_min > ground.boundingBox.x_min)){
            // std::cout << "ddddddddddddddddddd" << std::endl;
            
            // y test
            if ((wo2.position.y + wo2.boundingBox.y_min < ground.boundingBox.y_max) && (wo2.position.y + wo2.boundingBox.y_max > ground.boundingBox.y_min)) {
                return 1;
            }
        }
        
    }

    
    return 0;
    
}



void ws_loadTextures(){


    // MOUNTAIN TEXTURE
    glGenTextures(1, &mountainTexture);
    glBindTexture(GL_TEXTURE_2D, mountainTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    bmp_loader_loadBMPFile("media/mountain.bmp");
    // bmp_loader.prettyPrint();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp_getWidth(), bmp_getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp_getImageDataBuffer().data());
    glGenerateMipmap(GL_TEXTURE_2D);


    // GRAY TEXTURE
    glGenTextures(1, &grayTexture);
    glBindTexture(GL_TEXTURE_2D, grayTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // bmp_loader_loadBMPFile("media/mountain.bmp");
    // bmp_loader.prettyPrint();

    float gray_f = 0.5;
    unsigned char gray_i = (unsigned char)(gray_f * 255);
    const unsigned char grayData[3] = { gray_i, gray_i, gray_i };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &grayData);
    glGenerateMipmap(GL_TEXTURE_2D);



    glBindTexture(GL_TEXTURE_2D, 0);
}




void ws_createWorldObjects(){

    // TEMP
    Cube cube_bounding_1;
    cube_bounding_1.name = "cube_bounding_1";
    // worldObjects.push_back(cube_bounding_1);

    // worldCube1.scale = {2.0, 2.0, 2.0};
    cube_bounding_1.scale = { 0.5, 0.5, 0.5 };
    cube_bounding_1.position = { -10.0f, -5.0f, 20.0f };
    // worldCube1.printVertices();

    cube_bounding_1.setVaoVbo330();
    cube_bounding_1.setShaderProgram(&worldShader);
    worldObjects.push_back(cube_bounding_1);
    // std::cout << "cube_1.cubeString = " << cube_1.cubeString << std::endl;
    


    // GROUND 1

    // WorldObject worldCube1("src/models/cube.pso");
    worldGround1.name = "worldGround1";
    worldGround1.LoadWorldObject("src/models/ground_1.pso");

    // worldCube1.scale = {2.0, 2.0, 2.0};
    float groundScale = 50.0f;
    worldGround1.scale = { groundScale, groundScale, 1.0f };
    worldGround1.position = { 0.0f, 0.0f, GROUND_Z_0 };
    // worldCube1.printVertices();
    worldGround1.boundingBox.x_min = worldGround1.position.x - groundScale;
    worldGround1.boundingBox.x_max = worldGround1.position.x + groundScale;
    worldGround1.boundingBox.y_min = worldGround1.position.y - groundScale;
    worldGround1.boundingBox.y_max = worldGround1.position.y + groundScale;
    worldGround1.boundingBox.z_min = worldGround1.position.z + GROUND_Z_0;
    worldGround1.boundingBox.z_max = worldGround1.position.z + GROUND_Z_0;

    worldGround1.setVaoVbo330();
    worldGround1.setShaderProgram(&worldShader);
    worldObjects.push_back(worldGround1);


    // CUBE 1

    // WorldObject worldCube1("src/models/cube.pso");
    worldCube1.name = "worldCube1";
    worldCube1.LoadWorldObject("src/models/cube.pso");

    // worldCube1.scale = {2.0, 2.0, 2.0};
    worldCube1.scale = { 0.5, 0.5, 0.5 };
    worldCube1.position = { 20.0f, 0.0f, 0.0f };
    // worldCube1.printVertices();

    worldCube1.setVaoVbo330();
    worldCube1.setShaderProgram(&worldShader);
    worldObjects.push_back(worldCube1);



    // CUBE 2 - SPIN

    // WorldObject worldCube1("src/models/cube.pso");
    worldCube_spin.name = "worldCube_spin";
    worldCube_spin.LoadWorldObject("src/models/cube.pso");

    // worldCube1.scale = {2.0, 2.0, 2.0};
    float cubeSpinScale = 1.0f;
    worldCube_spin.scale = { cubeSpinScale, cubeSpinScale, cubeSpinScale };
    worldCube_spin.position = { 5.0f, 0.0f, 3.0f };
    // worldCube1.printVertices();

    worldCube_spin.setVaoVbo330();
    worldCube_spin.setShaderProgram(&worldShader);
    worldObjects.push_back(worldCube_spin);





    // CUBE 3 - GRAVITY

    // WorldObject worldCube1("src/models/cube.pso");
    cube_3_gravity.name = "cube_3_gravity";
    cube_3_gravity.LoadWorldObject("src/models/cube.pso");


    // GRAVITY ENABLED CUBES
    float cube_3_scale = 2.0;
    cube_3_gravity.scale = { cube_3_scale, cube_3_scale, cube_3_scale };
    cube_3_gravity.rotation = { 40.0f, -10.0f, 5.0f };

    cube_3_gravity.position_0 = { -5.0f, 0.0f, 6.0f };
    cube_3_gravity.position = cube_3_gravity.position_0;
    cube_3_gravity.velocity_0 = { 0.0f, 0.0f, 0.0f };
    cube_3_gravity.velocity = cube_3_gravity.velocity_0;
    // worldCube1.printVertices();
    cube_3_gravity.gravityOn = 1;

    cube_3_gravity.offsetToBottom = cube_3_scale;

    cube_3_gravity.boundingBox.x_min = - cube_3_scale;
    cube_3_gravity.boundingBox.x_max =   cube_3_scale;
    cube_3_gravity.boundingBox.y_min = - cube_3_scale;
    cube_3_gravity.boundingBox.y_max =   cube_3_scale;
    cube_3_gravity.boundingBox.z_min =  -cube_3_scale;
    cube_3_gravity.boundingBox.z_max =   cube_3_scale;

    cube_3_gravity.setVaoVbo330();
    cube_3_gravity.setShaderProgram(&worldShader);

    worldObjects.push_back(cube_3_gravity);

    // Add more cubes
    for (int i = 0; i < 0; i++) {
        cube_3_gravity.position_0 = { 40.0f, -10.0f + 5.0f * (float)i, 30.0f };
        cube_3_gravity.position = cube_3_gravity.position_0;
        
    }




    // CUBE 4 - OBJ !

    // CAVEATS:
    // - exported as triangulated obj file from blender
    //      - ./resources/models/blend-file.obj
    // - ignores mtl-file / data
    // - ignores v.w & vt.w in obj-file
    // - Render using already defined grayTexture 
    // - reads vertex normals, but does not consider that data when rendering
    //      - the vertex normals are put in the color layout location in the vbo/vao
    //      - because the cube is rendered using texture, the color data is ignored in the shader

    worldCube4_obj.name = "worldCube4_obj";
    // worldCube1.LoadWorldObject("src/models/cube.pso");
    obj_loadFromFile("./resources/models/blend-cube.obj");
    float * Kd_temp = obj_loadKdFromFile("./resources/models/blend-cube.mtl");
    worldCube4_obj.Kd[0] = Kd_temp[0];
    worldCube4_obj.Kd[1] = Kd_temp[1];
    worldCube4_obj.Kd[2] = Kd_temp[2];

    worldCube4_obj.vertices = obj_getVertexBuffer_v_vt_vn();
    std::cout << "worldCube4_obj.vertices.size() = " << worldCube4_obj.vertices.size() << std::endl;
    worldCube4_obj.vertexCount = worldCube4_obj.vertices.size() / 8;

    // worldCube1.scale = {2.0, 2.0, 2.0};
    worldCube4_obj.scale = { 2.0, 2.0, 2.0 };
    worldCube4_obj.position = { 20.0f, -10.0f, 10.0f };
    // worldCube1.printVertices();

    worldCube4_obj.setVaoVbo_obj();
    worldCube4_obj.setShaderProgram(&worldShader);

    worldCube4_obj.hasTexture = 1.0;
    worldCube4_obj.glTexture = grayTexture;

    worldObjects.push_back(worldCube4_obj);





    // TRIANGLE 2 - BOUNCE

    // WorldObject worldCube1("src/models/cube.pso");
    worldTriangle2_bounce.name = "worldTriangle2_bounce";
    worldTriangle2_bounce.LoadWorldObject("src/models/firstSimTriangle.pso");

    // worldCube1.scale = {2.0, 2.0, 2.0};
    // float  = 1.0f;
    float triBounceScale = 5.0f;
    worldTriangle2_bounce.scale = { 1.0f, triBounceScale, triBounceScale };
    worldTriangle2_bounce.position_0 = { 45.0f, 25.0f, 10.0f };
    worldTriangle2_bounce.position = worldTriangle2_bounce.position_0;
    worldTriangle2_bounce.velocity = { 0.0f, 0.0f, 0.0f };
    worldTriangle2_bounce.velocity_0 = { -1.0f, -0.5f, 25.0f };
    // worldCube1.printVertices();

    worldTriangle2_bounce.setVaoVbo330();
    worldTriangle2_bounce.setShaderProgram(&worldShader);
    worldObjects.push_back(worldTriangle2_bounce);



    // TRIANGLE 1 - TEXTURE

    // WorldObject worldTriangle1("src/models/triangle.pso");
    worldTriangle1Texture.name = "worldTriangle1Texture";
    worldTriangle1Texture.LoadWorldObject("src/models/triangle.pso");

    worldTriangle1Texture.scale = { 1.0, 5.0, 5.0 };
    worldTriangle1Texture.position = { -5.0f, -10.0f, 10.0f };

    worldTriangle1Texture.setVaoVbo332();
    worldTriangle1Texture.setShaderProgram(&worldShader);

    worldTriangle1Texture.hasTexture = 1;
    worldTriangle1Texture.glTexture = mountainTexture;
    worldObjects.push_back(worldTriangle1Texture);
   



    // USE NEW WORLD OBJECTS
    for(WorldObject& _worldObject : worldObjects){
        if (_worldObject.name == "worldTriangle2_bounce")
            worldTriangle2_simobj_pointer = &_worldObject;
        if (_worldObject.name == "worldGround1")
            worldGround_pointer = &_worldObject;
    }
}

void generateBlackWhiteImageBuffer(){


    for (long unsigned int rgb_i = 0; rgb_i < sizeof(blackWhiteImageBuffer); rgb_i += 3) {
        // std::cout << rgb_i << " ";
        int r = rand();

        if (r < 1073741823) {
            blackWhiteImageBuffer[rgb_i] = 0;
            blackWhiteImageBuffer[rgb_i + 1] = 0;
            blackWhiteImageBuffer[rgb_i + 2] = 0;
        }
        else {
            blackWhiteImageBuffer[rgb_i] = 255;
            blackWhiteImageBuffer[rgb_i + 1] = 255;
            blackWhiteImageBuffer[rgb_i + 2] = 255;
        }
    }
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


}
