#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Simulation.hpp"
#include "Camera.hpp"

#include "WorldObject.hpp"
#include "obj_loader.hpp"

#include "WorldRenderer.hpp"

extern Shader worldShader;
extern Shader worldObjShader;
extern Shader wireframeShader;


#include "WorldScene.hpp"

#include "bmp_loader.hpp"

#include "Timing.hpp"

#include "VectorMath.hpp"


// SIMULATORS
#include "SimWorldContainer.hpp"
#include "Simulator.hpp"
extern Sim::Simulator* sim_1_ptr;


// SCENE - RENDPIPE - 2024-09-28
#include "Scene.hh"
Scene scene_1;

// WORLD SIMULATOR
#include "WorldSimulator.cc"
std::vector<WorldSimulator*> worldSimulators;



// WORLD OBJECTS

// std::vector<WorldObject> worldObjects;
std::vector<WorldObject*> worldObjects;

WorldObject worldGround1;
WorldObject* worldGround_pointer;

// Cube cube_bounding_1;

WorldObject worldCube1;
WorldObject worldCube_spin;
WorldObject cube_3_gravity;
WorldObject worldCube4_obj;
WorldObject house1_obj;

WorldObject worldTriangle1Texture;
WorldObject worldTriangle2_bounce;

SimWorldContainer simContainer_1;

WorldObject* worldTriangle2_simobj_pointer;


// TEXTURES
unsigned int mountainTexture;
unsigned int grayTexture;
unsigned int qrTexture;


#define GROUND_Z_0 0.0f


void ws_init(){

    wr_init();

    ws_loadTextures();

    // ws_createSimulators();
    ws_createWorldObjects();

    cam_init();

    // RENDPIPE
    scene_1.camera = getCurrentCamera_pointer();

}

void ws_resetWorldObjects(){
    for(WorldObject* _worldObject_p : worldObjects){
        WorldObject& _worldObject = *_worldObject_p;
        if(_worldObject.hasRigidBody){
            _worldObject.position = _worldObject.position_0;
            _worldObject.velocity = _worldObject.velocity_0;
            _worldObject.rotation = _worldObject.rotation_0;
            _worldObject.angularVelocity = _worldObject.angularVelocity_0;
        }
    }
}

std::vector<WorldObject*> ws_getWorldObjects(){
    return worldObjects;
}


void ws_update() {

    // SIMULATION
    updateSimulation();
    // USE NEW WORLD OBJECTS
    // for (WorldObject& _worldObject : worldObjects) {
    //     if (_worldObject.name == "worldTriangle2_bounce")
    //         updateSimulation(_worldObject);

    // }


    // for(WorldObject& _wo : worldObjects)
    //     _wo.SetModelMatrixRowMajor();

    if(!timing_worldIsPaused()){
        ws_physics();
    }

    // Update Camera matrices (view/persp.)
    cam_UpdateCam();


    // Render all world objects (world renderer)
    wr_render(worldObjects);

    // new rendering pipeline - 2024-09-28
    ws_update_objects();
    ws_render();
}


// RENDPIPE
void ws_update_objects(){
    for (WorldObject* _worldObject : worldObjects) {
        if (_worldObject->isRendpipe)
            _worldObject->update();
    }

    for (WorldSimulator* _worldSim : worldSimulators) {
            _worldSim->update();
    }

    
}
void ws_render(){
    for(WorldObject* _worldObject : worldObjects){
        if (_worldObject->isRendpipe)
            _worldObject->render();
    }

    for (WorldSimulator* _worldSim : worldSimulators){
            _worldSim->render();
    }
}



void ws_physics(){
    WorldObject& worldGround = *worldGround_pointer;

    // float modelMatrix16[16] = {0};

    for (WorldObject* _worldObject_p : worldObjects){
        WorldObject& _worldObject = *_worldObject_p;
        // std::cout << "_worldObject.name =  " << _worldObject.name << std::endl;
        // std::cout << "_worldObject.rotation.x =  " << _worldObject.rotation.x << std::endl;
       
        if (_worldObject.name == "worldCube_spin")
            _worldObject.Rotate({ 0.03f, 0.01f, 0.0f });
        if (_worldObject.name == "cube_3_gravity")
            _worldObject.Rotate(_worldObject.angularVelocity);
        
        // GRAVITY
        if (_worldObject.gravityOn == 1){
            _worldObject.velocity.z += -9.8 * 0.0133; // dt = 0.0133s
            _worldObject.position.z += _worldObject.velocity.z * 0.0133;


            // Need to update after movement for collision check
            _worldObject.SetModelMatrixRowMajor();


            // Check ground bounce

            // USING RIGID BODY
            // for(int i = 2; i < _worldObject.)
            if(_worldObject.hasRigidBody){
                float worldVertex[3] = {0};
                float localVertex[3] = {0};
                float * tempVertex = NULL;

                // Loop through the verticies of the rigid body, apply model transform, if one is below ground, BOUNCE
                for(unsigned int i = 0; i < _worldObject.rigidBody.vertices.size(); i += 3){
                    
                    // float localVertex[3] = { _worldObject.rigidBody.vertices[i], _worldObject.rigidBody.vertices[i+1] , _worldObject.rigidBody.vertices[i+2] };
                    localVertex[0] = _worldObject.rigidBody.vertices[i];
                    localVertex[1] = _worldObject.rigidBody.vertices[i+1];
                    localVertex[2] = _worldObject.rigidBody.vertices[i+2];
                    
                    // std::cout << "localVertex[0] " << localVertex[0] << std::endl;
                    tempVertex = mul_vec3_mat16(localVertex, _worldObject.modelMatrixRowMajor);
                    

                    worldVertex[0] = tempVertex[0];
                    worldVertex[1] = tempVertex[1];
                    worldVertex[2] = tempVertex[2];
                    // std::cout << "worldVertex[2] " << worldVertex[2] << std::endl;

                    // std::cout << "OKOKOKOKOKOKKOK" << std::endl;
                    if (worldVertex[2] < 0.0){
                        // std::cout << "RIGID BODY COLLISION  ===================" << std::endl;
                        _worldObject.position.z -= _worldObject.velocity.z * 0.0133;
                        _worldObject.velocity.z = -_worldObject.velocity.z * 0.5;

                        // Improve by checking direction of vertex velocity??
                        _worldObject.angularVelocity = { -_worldObject.angularVelocity.x, -_worldObject.angularVelocity.y , -_worldObject.angularVelocity.z };

                        // Make sure we don't collide more than one vertexs
                        break;
                    }
                }
            }


            // USING WORLD OBJECT BOUNDING BOX
            // worldObjectsCollide_aabb_z(worldGround, _worldObject);
            // if (_worldObject.position.z < (worldGround.position.z + _worldObject.offsetToBottom)) {
            if ( worldObjectCollidingWithGround_aabb_z(worldGround, _worldObject) == 1) {
                // std::cout << "COLLISION" << std::endl;
                
                // _worldObject.position.z -= _worldObject.velocity.z * 0.0133;
                // _worldObject.velocity.z = -_worldObject.velocity.z * 0.8;
            }
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

    bmp_loader_loadBMPFile("resources/textures/mountain.bmp");
    // bmp_loader_loadBMPFile("resources/models/blend-cube-texture-1.bmp");

    // blend-cube-texture-1
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


    // RANDOM TEXTURE
    glGenTextures(1, &qrTexture);
    glBindTexture(GL_TEXTURE_2D, qrTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // bmp_loader_loadBMPFile("media/mountain.bmp");
    // bmp_loader.prettyPrint();

    // RANDOM DATA GENERATION
    // Old Black and white generated texture
    // Generate a black and white test 'image'
    int blackWhiteWidth = 100;
    int blackWhiteHeight = 100;
    // Generate black and white texture : 3 * blackWhiteWidth * blackWhiteHeight
    unsigned char blackWhiteImageBuffer[30000]; // = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, };

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, blackWhiteWidth, blackWhiteHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, &blackWhiteImageBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);




    glBindTexture(GL_TEXTURE_2D, 0);
}


void ws_createSimulators(){
    // SimWorldContainer simContainer_1("simContainer_1");
    // simulators.push_back(simContainer_1);
}

void ws_createWorldObjects(){

    
    


    // 1. - CUBE 1

    WorldObject* worldCube1 = new WorldObject("cube", "worldCube1");
    worldObjects.push_back(worldCube1);
    worldCube1->isActive = false;
    worldCube1->isRendpipe = true;

    worldCube1->scale = { 0.5, 0.5, 0.5 };
    worldCube1->position = { 20.0f, 0.0f, 0.0f };



    // 2. - CUBE 2 - SPIN

    // WorldObject worldCube1("resources/models/cube.pso");
    WorldObject* worldCube_spin = new WorldObject("cube", "worldCube_spin");
    worldObjects.push_back(worldCube_spin);
    worldCube_spin->isActive = false;
    worldCube_spin->isRendpipe = true;

    float cubeSpinScale = 1.0f;
    worldCube_spin->scale = { cubeSpinScale, cubeSpinScale, cubeSpinScale };
    worldCube_spin->position = { 5.0f, 0.0f, 3.0f };






    // 3. - CUBE 3 - GRAVITY

    WorldObject* cube_3_gravity = new WorldObject("cube", "cube_3_gravity");
    worldObjects.push_back(cube_3_gravity);
    cube_3_gravity->isActive = false;
    cube_3_gravity->isRendpipe = true;

    // RIGID BODY
    cube_3_gravity->hasRigidBody = true;
    cube_3_gravity->rigidBody.shader = &wireframeShader;
    cube_3_gravity->rigidBody.setVaoVbo_wireframe();

    // GRAVITY ENABLED CUBES
    float cube_3_scale = 2.0;
    cube_3_gravity->scale = { cube_3_scale, cube_3_scale, cube_3_scale };
    cube_3_gravity->rotation = { 40.0f, -10.0f, 185.0f };
    cube_3_gravity->rotation_0 = cube_3_gravity->rotation;
    cube_3_gravity->angularVelocity = { 0.01f, 0.01f, 0.0f };
    cube_3_gravity->angularVelocity_0 = cube_3_gravity->angularVelocity;

    cube_3_gravity->position_0 = { -5.0f, 0.0f, 15.0f };
    cube_3_gravity->position = cube_3_gravity->position_0;
    cube_3_gravity->velocity_0 = { 0.0f, 0.0f, 0.0f };
    cube_3_gravity->velocity = cube_3_gravity->velocity_0;
    // worldCube1.printVertices();
    cube_3_gravity->gravityOn = 1;

    cube_3_gravity->offsetToBottom = cube_3_scale;

    cube_3_gravity->boundingBox.x_min = - cube_3_scale;
    cube_3_gravity->boundingBox.x_max =   cube_3_scale;
    cube_3_gravity->boundingBox.y_min = - cube_3_scale;
    cube_3_gravity->boundingBox.y_max =   cube_3_scale;
    cube_3_gravity->boundingBox.z_min =  -cube_3_scale;
    cube_3_gravity->boundingBox.z_max =   cube_3_scale;


    // Add more cubes
    // for (int i = 0; i < 1; i++) {
    //     cube_3_gravity->position_0 = { 40.0f, -10.0f + 5.0f * (float)i, 30.0f };
    //     cube_3_gravity->position = cube_3_gravity->position_0;
        
    // }




    // 4. - CUBE 4 - OBJ !

    WorldObject* worldCube4_obj = new WorldObject("cube", "worldCube4_obj");
    worldObjects.push_back(worldCube4_obj);
    worldCube4_obj->isActive = false;
    worldCube4_obj->isRendpipe = true;

    worldCube4_obj->scale = { 2.0, 2.0, 2.0 };
    worldCube4_obj->position = { 20.0f, -10.0f, 10.0f };




    // 5. - TEMP - BOUNDING BOX ??
    WorldObject* cube_bounding_1 = new WorldObject("cube", "cube_bounding_1");
    worldObjects.push_back(cube_bounding_1);
    cube_bounding_1->isActive = false;
    cube_bounding_1->isRendpipe = true;

    cube_bounding_1->scale = { 0.5, 0.5, 0.5 };
    cube_bounding_1->position = { -10.0f, -5.0f, 20.0f };




    // 6. -  HOUSE 1 - OBJ MODEL

    WorldObject* house1_obj = new WorldObject("house-1", "house1_obj");
    worldObjects.push_back(house1_obj);
    house1_obj->isActive = false;
    house1_obj->isRendpipe = true;

    house1_obj->scale = { 1.0, 1.0, 1.0 };
    house1_obj->position = { 10.0f, 20.0f, 3.0f };
    house1_obj->position_0 = house1_obj->position;





    // 7. - TRIANGLE 2 - BOUNCE

    WorldObject* worldTriangle2_bounce = new WorldObject("tri_pso.pso", "worldTriangle2_bounce");
    worldObjects.push_back(worldTriangle2_bounce);
    worldTriangle2_bounce->isActive = false;
    worldTriangle2_bounce->isRendpipe = true;

    float triBounceScale = 5.0f;
    worldTriangle2_bounce->scale = { 1.0f, triBounceScale, triBounceScale };
    worldTriangle2_bounce->position_0 = { 45.0f, 25.0f, 10.0f };
    worldTriangle2_bounce->position = worldTriangle2_bounce->position_0;
    worldTriangle2_bounce->velocity = { 0.0f, 0.0f, 0.0f };
    worldTriangle2_bounce->velocity_0 = { -1.0f, -0.5f, 25.0f };



    // 8. - TRIANGLE 1 - TEXTURE

    WorldObject* worldTriangle1Texture = new WorldObject("tri_tex_pso.pso", "worldTriangle1Texture");
    worldObjects.push_back(worldTriangle1Texture);
    worldTriangle1Texture->isActive = false;
    worldTriangle1Texture->isRendpipe = true;

    worldTriangle1Texture->scale = { 1.0, 5.0, 5.0 };
    worldTriangle1Texture->position = { -5.0f, -10.0f, 10.0f };



    /* 
        9. - rendpipe_obj   ::   FIRST RENDER PIPLINE OBJECT
    */

    WorldObject* rendpipe_obj_ptr = new WorldObject("blend-cube-texture-1", "rendpipe_obj");
    worldObjects.push_back(rendpipe_obj_ptr);

    // MAKE SURE TO MAKE NOT ACTIVE TO PREVENT OLD RENDERING PIPELINE!
    rendpipe_obj_ptr->isActive = false;
    // ENABLE NEW RENDPIPE
    rendpipe_obj_ptr->isRendpipe = true;

    // WorldObject& rendpipe_obj = *rendpipe_obj_ptr;

    // rendpipe_obj.scale = { 0.5, 0.5, 0.5 };
    // rendpipe_obj.position_0 = { -10.0f, 0.0f, 5.0f };
    // rendpipe_obj.position = { -10.0f, 0.0f, 5.0f };



    /*
        10. - rendpipe_cube2_ptr   ::  TEST PIPLINE WITH NO TEXTURE OBJECT
    */
    // blend-cube-no-texture
    WorldObject* rendpipe_cube2_ptr = new WorldObject("cube", "rendpipe_cube2");
    worldObjects.push_back(rendpipe_cube2_ptr);

    // MAKE SURE TO MAKE NOT ACTIVE TO PREVENT OLD RENDERING PIPELINE!
    rendpipe_cube2_ptr->isActive = false;
    // ENABLE NEW RENDPIPE
    rendpipe_cube2_ptr->isRendpipe = true;

    rendpipe_cube2_ptr->position = { -10.0f, 0.0f, 5.0f };


    /*
        11. - ground_01   ::  TEST PIPLINE WITH NO TEXTURE OBJECT
    */
       // blend-cube-no-texture
    WorldObject* ground_01 = new WorldObject("ground-01", "ground_01");
    worldObjects.push_back(ground_01);

    // MAKE SURE TO MAKE NOT ACTIVE TO PREVENT OLD RENDERING PIPELINE!
    ground_01->isActive = false;
    // ENABLE NEW RENDPIPE
    ground_01->isRendpipe = true;

    float groundScale = 40; // currently scaled in blender..

    ground_01->position = { -10.0f, 0.0f, GROUND_Z_0 };
    // worldCube1.printVertices();
    ground_01->boundingBox.x_min = ground_01->position.x - groundScale;
    ground_01->boundingBox.x_max = ground_01->position.x + groundScale;
    ground_01->boundingBox.y_min = ground_01->position.y - groundScale;
    ground_01->boundingBox.y_max = ground_01->position.y + groundScale;
    ground_01->boundingBox.z_min = ground_01->position.z + GROUND_Z_0;
    ground_01->boundingBox.z_max = ground_01->position.z + GROUND_Z_0;


    /* 
        12. - tri_pso : First pso in new rendering pipline
    */
    WorldObject* tri_pso = new WorldObject("tri_pso.pso", "tri_pso");
    worldObjects.push_back(tri_pso);

    tri_pso->isActive = false;
    tri_pso->isRendpipe = true;

    tri_pso->position = { -20.0f, 1.0f, 2.0f };


    /*
        13. - tri_tex_pso : First pso with texture in new rendering pipline
    */
    WorldObject* tri_tex_pso = new WorldObject("tri_tex_pso.pso", "tri_tex_pso");
    worldObjects.push_back(tri_tex_pso);

    tri_tex_pso->isActive = false;
    tri_tex_pso->isRendpipe = true;

    tri_tex_pso->position = { -20.1f, 1.0f, 3.0f };



    /*  
        14. wire_pos_1 : first wireframe pso-object loaded
    */

    WorldObject* wire_pos_1 = new WorldObject("cube-wire.pso", "wire_pos_1");
    worldObjects.push_back(wire_pos_1);

    wire_pos_1->scale = { 1.0, 1.0, 1.0 };
    wire_pos_1->position = { -21.0f, 15.0f, 4.1f };
    wire_pos_1->isActive = false;
    wire_pos_1->isRendpipe = true;




    /* 
        15. - wordlSim_1  - First WorldSimulator
     */

    WorldSimulator* worldSim_1 = new WorldSimulator("worldSim_1");
    worldSimulators.push_back(worldSim_1);

    worldSim_1->createSimContainer("cube-wire.pso", "worldSim_1_container");
    worldSim_1->simContainer->scale = { 3.0, 3.0, 3.0 };
    worldSim_1->simContainer->position = { -15.0f, -5.0f, 3.1f };
    worldSim_1->simContainer->isActive = false;
    worldSim_1->simContainer->isRendpipe = true;

    Sim::Simulator* simulator1_ptr = Sim::getSim1Pointer();
    worldSim_1->setSimulator(simulator1_ptr);

    // 16. - worldSim_1_obj_1 - First simulator, first sim object
    WorldObject* worldSim_1_obj_1 = new WorldObject("cube", "worldSim_1_obj_1");
    worldSim_1->simulatorWorldObjects.push_back(worldSim_1_obj_1);
    worldSim_1_obj_1->isActive = false;
    worldSim_1_obj_1->isRendpipe = true;
    worldSim_1_obj_1->scale = { 0.5, 0.5, 0.5 };
    worldSim_1_obj_1->position_0 = { 0.0f, 0.0f, 0.0f };
    worldSim_1_obj_1->position = { 0.0f, 0.0f, 0.0f };



    /*
        OLD SIMULATOR
    */


    // simContainer_1.name = "simContainer_1";
    // // simContainer_1.worldObjectType = WorldObjectType::SimWorldContainer;

    // simContainer_1.scale = { 3.0, 3.0, 3.0 };
    // simContainer_1.position = { 25.0f, -5.0f, 3.1f };

    // // simContainer_1.renderer.setVaoVbo_obj();
    // simContainer_1.addSimulatorContainerVertices();
    // simContainer_1.renderer.createSimulatorRenderer(simContainer_1.vertices);

    // simulator1_ptr = Sim::getSim1Pointer();
    // simContainer_1.SetSimulator(simulator1_ptr);

    // // std::cout << "simulator1_ptr->simSaveDirectory = " << simulator1_ptr->simSaveDirectory << std::endl;

    // // 15. - SIMULATOR 1 - OBJECT 1
    // WorldObject& sim1_containerObj_1 = simContainer_1.containerWorldObjects.emplace_back();
    // sim1_containerObj_1.name = "simContainer1_Object1";
    // sim1_containerObj_1.LoadWorldObject("resources/models/pso/cube.pso");

    // // worldCube1.scale = {2.0, 2.0, 2.0};
    // sim1_containerObj_1.scale = { 0.5, 0.5, 0.5 };
    // sim1_containerObj_1.position_0 = { 0.0f, 0.0f, 0.0f };
    // sim1_containerObj_1.position = { 0.0f, 0.0f, 0.0f };
    // // worldCube1.printVertices();

    // sim1_containerObj_1.setVaoVbo330();
    // sim1_containerObj_1.setShaderProgram(&worldShader);
    // // sim1_containerObj_1.push_back(worldCube1);

    // worldObjects.push_back(&simContainer_1);

    // for (WorldObject& _worldObject : worldObjects) {
    //         std::cout << "!!!!" << std::endl;

    //     if (_worldObject.name == "simContainer_1"){
    //         SimWorldContainer* container = static_cast<SimWorldContainer*>(&_worldObject);
    //         std::cout << "__________" << std::endl;
    //     }
    // }

    // std::cout << "simContainer_1.name = " << simContainer_1.name << std::endl;
    // std::cout << "simContainer_1.vertices.size() = " << simContainer_1.vertices.size() << std::endl;









    // USE NEW WORLD OBJECTS
    for(WorldObject* _worldObject_p : worldObjects){
        WorldObject& _worldObject = *_worldObject_p;
        if (_worldObject.name == "worldTriangle2_bounce")
            worldTriangle2_simobj_pointer = &_worldObject;
        if (_worldObject.name == "ground_01")
            worldGround_pointer = ground_01;
    }
}


