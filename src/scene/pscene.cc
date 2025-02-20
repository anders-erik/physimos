#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Windowing.hpp"

#include "Simulation.hpp"
#include "Camera.hpp"

#include "pobject.hh"

#include "pscene.hh"

#include "Timing.hpp"



// WORLD SIMULATORS
#include "WorldSimulator.hh"
std::vector<WorldSimulator*> worldSimulators;


// WORLD OBJECTS
std::vector<PObject*> worldObjects;

// WORLD GROUND
#define GROUND_Z_0 0.0f
PObject worldGround1;
PObject* worldGround_pointer;


// Bouncy triangle for nostalgia
PObject* worldTriangle2_simobj_pointer;



namespace PScene {

    static ::PScene::Scene scene1;
    static ::PScene::Scene* currentScene;

    PObject* bouncyTrianglePointer;
    PObject* groundObjectPointerScene1;



    PObject* getWorldObjectByName(std::string _name){
        for (PObject* pObject : currentScene->pObjects) {
            if (pObject->name == _name)
                return pObject;
        }
        return nullptr;
    }

    // returns the objects of the current scene.
    std::vector<PObject*> getPObjects() {
        return currentScene->pObjects;
    }


    void addPObjectToCurrentScene(::PObject* pObject){
        currentScene->pObjects.push_back(pObject);
    }

    void windowChangeUpdate(PhysWin _physimos_window){
        currentScene->camera->setPerspectiveMatrix(_physimos_window.width, _physimos_window.height);
    }

    void init(){

        scene1 = Scene();
        currentScene = &scene1;

        populateScene1();
        // ::ws_createWorldObjects();

        // ::cam_init();
        // ::scene_1.camera = ::getCurrentCamera_pointer();
        
        scene1.camera = new PCamera::Camera();

        subscribeWindowChange_scene(windowChangeUpdate);
    }

    Scene* getCurrentScene(){
        return currentScene;
    }


    void updateCurrentScene(){

        // SIMULATION
        // ::updateSimulation();
        ::updateSimWithBouncyTriangle(bouncyTrianglePointer);


        if (!::timing_worldIsPaused()) {
            physics();
        }

        // Update Camera matrices (view/persp.)
        // ::cam_UpdateCam();
        
        // This will write to the same inputstate and make the scene interactions wonky if used parallel to the old global camera object
        currentScene->camera->update();


        // UPDATE OBJECTS
        for (PObject* pObject : currentScene->pObjects) {
            if (pObject->isActive)
                pObject->update();
        }
        for (WorldSimulator* _worldSim : currentScene->worldSimulators) {
            _worldSim->update();
        }
    }



    void renderCurrentScene() {

        // Enable later only when enabling a specific scene
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


        for (PObject* oPobject : currentScene->pObjects) {
            if (oPobject->isActive){
                // oPobject->render();
                
                renderPObject(oPobject);
            }
        }

        // TODO: NOT APPLYING
        for (WorldSimulator* _worldSim : scene1.worldSimulators) {
            // _worldSim->render();
            // ::WorldObject* pSim = dynamic_cast<::WorldObject*>(_worldSim);
            renderPObject(_worldSim->simContainer);

            for (PObject* _wo : _worldSim->simulatorWorldObjects) {
                renderPObject(_wo);
            }
        }
    }

    // moved rendering to scene to to prevent PObject dependency on PScene
    void renderPObject(::PObject* pObject){
        // pObject->render();
        pObject->shader->use();


        // object/scene data
        shader_setWorldObject_uniforms(pObject->modelMatrixRowMajor, scene1.camera->viewMatrix, scene1.camera->perspectiveMatrix16, sanityMatrix16);



        // TEXTURE

        // Currently the pso shader branches, forcing me to toggle the 'hasTexture' uniform for each object..
        if (pObject->model_ptr->modelFileType == res::ModelFileType::pso && pObject->model_ptr->glTexture != 0)
            setHasTextureUniform(1);
        else
            setHasTextureUniform(0);

        pObject->model_ptr->useTexture();



        // VAO & DRAW
        if (pObject->model_ptr->loadedVertStructure == res::ModelVertStucture::p3) { // if it is a wireframe model, use wireframe shader but regular model
            glBindVertexArray(pObject->model_ptr->vao);
            drawLines(pObject->model_ptr->wireVertexCount);
        }
        else if (pObject->shaderType == Shaders::worldWireframe) { // if model with faces but wireframe shader, use wire-vao
            glBindVertexArray(pObject->model_ptr->vaoWire);
            drawLines(pObject->model_ptr->wireVertexCount);
        }
        else { // Just draw regular triangles
            glBindVertexArray(pObject->model_ptr->vao);
            // Shader call
            drawTriangles(pObject->model_ptr->vertexCount);
        }


        // Recursive rendering
        for (PObject* _wo : pObject->children) {
            // _wo->render();
            renderPObject(_wo);
        }
    }



    float* _mul_vec3_mat16(float* _vec3, float* _mat16) {
        float _tempVec3[3] = { 0.0f, 0.0f, 0.0f };

        _tempVec3[0] += _mat16[0] * _vec3[0] + _mat16[1] * _vec3[1] + _mat16[2] * _vec3[2] + _mat16[3] * 1.0f;
        _tempVec3[1] += _mat16[4] * _vec3[0] + _mat16[5] * _vec3[1] + _mat16[6] * _vec3[2] + _mat16[7] * 1.0f;
        _tempVec3[2] += _mat16[8] * _vec3[0] + _mat16[9] * _vec3[1] + _mat16[10] * _vec3[2] + _mat16[11] * 1.0f;
        // 1.0f += _mat16[12] * _vec3[0] + _mat16[13] * _vec3[1] + _mat16[14] * _vec3[2] + _mat16[15] * 1.0f;


        _vec3[0] = _tempVec3[0];
        _vec3[1] = _tempVec3[1];
        _vec3[2] = _tempVec3[2];

        return _vec3;
    }

    void physics() {
        PObject& worldGround = *groundObjectPointerScene1;

        for (PObject* _worldObject_p : currentScene->pObjects) {
            PObject& _worldObject = *_worldObject_p;
            // std::cout << "_worldObject.name =  " << _worldObject.name << std::endl;
            // std::cout << "_worldObject.rotation.x =  " << _worldObject.rotation.x << std::endl;


            // GRAVITY
            if (_worldObject.gravityOn == 1) {
                _worldObject.transform->velocity.data[2] += -9.8 * 0.0133; // dt = 0.0133s
                _worldObject.transform->position.data[2] += _worldObject.transform->velocity.data[2] * 0.0133;



                // Need to update after movement for collision check
                _worldObject.SetModelMatrixRowMajor();


                // Check ground bounce

                // USING RIGID BODY

                // for(int i = 2; i < _worldObject.)
                if (_worldObject.hasRigidBody) {
                    float worldVertex[3] = { 0 };
                    float localVertex[3] = { 0 };
                    float* tempVertex = NULL;

                    // Loop through the verticies of the rigid body, apply model transform, if one is below ground, BOUNCE
                    for (unsigned int i = 0; i < _worldObject.rigidBody.vertices.size(); i += 3) {

                        // float localVertex[3] = { _worldObject.rigidBody.vertices[i], _worldObject.rigidBody.vertices[i+1] , _worldObject.rigidBody.vertices[i+2] };
                        localVertex[0] = _worldObject.rigidBody.vertices[i];
                        localVertex[1] = _worldObject.rigidBody.vertices[i + 1];
                        localVertex[2] = _worldObject.rigidBody.vertices[i + 2];

                        // std::cout << "localVertex[0] " << localVertex[0] << std::endl;
                        tempVertex = _mul_vec3_mat16(localVertex, _worldObject.modelMatrixRowMajor);


                        worldVertex[0] = tempVertex[0];
                        worldVertex[1] = tempVertex[1];
                        worldVertex[2] = tempVertex[2];

                        // std::cout << "localVertex[2] = " << localVertex[2] << std::endl;
                        // std::cout << "worldVertex[2] = " << worldVertex[2] << std::endl;


                        // std::cout << "OKOKOKOKOKOKKOK" << std::endl;
                        if (worldVertex[2] < 0.0) {
                            // std::cout << "RIGID BODY COLLISION  ===================" << std::endl;
                            // std::cout << "_worldObject.name = " << _worldObject.name << std::endl;
                            _worldObject.transform->position.data[2] -= _worldObject.transform->velocity.data[2] * 0.0133;
                            _worldObject.transform->velocity.data[2] = -_worldObject.transform->velocity.data[2] * 0.5;

                            // Improve by checking direction of vertex velocity??
                            _worldObject.transform->angularVelocity = { -_worldObject.transform->angularVelocity.data[0], -_worldObject.transform->angularVelocity.data[1] , -_worldObject.transform->angularVelocity.data[2] };

                            // Make sure we don't collide more than one vertexs
                            break;
                        }
                    }
                }


                // USING WORLD OBJECT BOUNDING BOX
                // worldObjectsCollide_aabb_z(worldGround, _worldObject);
                // if (_worldObject.position.z < (worldGround.position.z + _worldObject.offsetToBottom)) {
                if (::PScene::_worldObjectCollidingWithGround_aabb_z(worldGround, _worldObject) == 1) {
                    // std::cout << "COLLISION" << std::endl;

                    // _worldObject.position.z -= _worldObject.velocity.z * 0.0133;
                    // _worldObject.velocity.z = -_worldObject.velocity.z * 0.8;
                }
            }


            // APPLY VELOCITIES

            if (_worldObject.name == "worldCube_spin")
                _worldObject.transform->rotation += { 0.03f, 0.01f, 0.0f };
                // _worldObject.transform->Rotate({ 0.03f, 0.01f, 0.0f });
            if (_worldObject.name == "cube_3_gravity")
                _worldObject.transform->rotation += _worldObject.transform->angularVelocity;
                // _worldObject.transform->Rotate(_worldObject.transform->angularVelocity);
            if (_worldObject.name == "transform_1")
                _worldObject.transform->rotation += _worldObject.transform->angularVelocity;
                // _worldObject.transform->Rotate(_worldObject.transform->angularVelocity);

            _worldObject.transform->position.data[0] += _worldObject.transform->velocity.data[0] * 0.0133;
            _worldObject.transform->position.data[1] += _worldObject.transform->velocity.data[1] * 0.0133;

        }
    }


    int _worldObjectCollidingWithGround_aabb_z(PObject& ground, PObject& wo2) {

        // vertical test
        // if (wo2.position.z < (ground.position.z + wo2.offsetToBottom)) {
        if ((wo2.transform->position.data[2] + wo2.boundingBox.z_min) < ground.boundingBox.z_max) {
            // if(wo)
            // std::cout << "wo2.boundingBox.z_min   " << wo2.boundingBox.z_min << std::endl;
            // std::cout << "ground.boundingBox.z_max   " << ground.boundingBox.z_max << std::endl;

            // x test
            // std::cout << "" << var << std::endl;
            // std::cout << "wo2.position.x + wo2.boundingBox.x_min = " << wo2.position.x + wo2.boundingBox.x_min << std::endl;
            // std::cout << "ground.boundingBox.x_max               = " << ground.boundingBox.x_max << std::endl;


            if ((wo2.transform->position.data[0] + wo2.boundingBox.x_min < ground.boundingBox.x_max) && (wo2.transform->position.data[0] + wo2.boundingBox.x_min > ground.boundingBox.x_min)) {
                // std::cout << "ddddddddddddddddddd" << std::endl;

                // y test
                if ((wo2.transform->position.data[1] + wo2.boundingBox.y_min < ground.boundingBox.y_max) && (wo2.transform->position.data[1] + wo2.boundingBox.y_max > ground.boundingBox.y_min)) {
                    return 1;
                }
            }

        }


        return 0;

    }


    void populateScene1() {

        // 1. - CUBE 1

        PObject* worldCube1 = new PObject("cube", "worldCube1");
        scene1.pObjects.push_back(worldCube1);
        worldCube1->isActive = true;

        // This will properly set the data-array thanks to initializer list constructor
        worldCube1->transform->scale = { 1.5, 0.5, 0.5 };
        worldCube1->transform->position = { -20.0f, 0.0f, 0.0f };
        worldCube1->transform_0->position = { -20.0f, 0.0f, 0.0f };




        // 2. - CUBE 2 - SPIN

        // WorldObject worldCube1("resources/models/cube.pso");
        PObject* worldCube_spin = new PObject("cube", "worldCube_spin");
        scene1.pObjects.push_back(worldCube_spin);
        worldCube_spin->isActive = true;

        float cubeSpinScale = 1.0f;
        worldCube_spin->transform->scale = { cubeSpinScale, cubeSpinScale, cubeSpinScale };
        worldCube_spin->transform->position  = { 5.0f, 0.0f, 3.0f };
        worldCube_spin->transform_0->position = { 5.0f, 0.0f, 3.0f };






        // 3. - CUBE 3 - GRAVITY

        PObject* cube_3_gravity = new PObject("cube", "cube_3_gravity");
        scene1.pObjects.push_back(cube_3_gravity);
        cube_3_gravity->isActive = true;

        // RIGID BODY
        cube_3_gravity->hasRigidBody = true;
        // TODO : RENDER THE RIGID BODY AS WIREFRAME
        // cube_3_gravity->rigidBody.shader = &wireframeShader;
        // cube_3_gravity->rigidBody.setVaoVbo_wireframe();

        // GRAVITY ENABLED CUBES
        float cube_3_scale = 2.0;
        cube_3_gravity->transform->scale = { cube_3_scale, cube_3_scale, cube_3_scale };
        cube_3_gravity->transform->rotation = { 40.0f, -10.0f, 185.0f };
        cube_3_gravity->transform_0->rotation = cube_3_gravity->transform->rotation;
        cube_3_gravity->transform->angularVelocity = { 0.01f, 0.01f, 0.0f };
        cube_3_gravity->transform_0->angularVelocity = cube_3_gravity->transform->angularVelocity;

        cube_3_gravity->transform_0->position = { -5.0f, 0.0f, 15.0f };
        cube_3_gravity->transform->position = cube_3_gravity->transform_0->position;
        cube_3_gravity->transform_0->velocity = { 0.0f, 0.0f, 0.0f };
        cube_3_gravity->transform->velocity = cube_3_gravity->transform_0->velocity;
        // worldCube1.printVertices();
        cube_3_gravity->gravityOn = true;

        cube_3_gravity->offsetToBottom = cube_3_scale;

        cube_3_gravity->boundingBox.x_min = -cube_3_scale;
        cube_3_gravity->boundingBox.x_max = cube_3_scale;
        cube_3_gravity->boundingBox.y_min = -cube_3_scale;
        cube_3_gravity->boundingBox.y_max = cube_3_scale;
        cube_3_gravity->boundingBox.z_min = -cube_3_scale;
        cube_3_gravity->boundingBox.z_max = cube_3_scale;


        // Add more cubes
        // for (int i = 0; i < 1; i++) {
        //     cube_3_gravity->position_0 = { 40.0f, -10.0f + 5.0f * (float)i, 30.0f };
        //     cube_3_gravity->position = cube_3_gravity->position_0;

        // }




        // 4. - CUBE 4 - OBJ !

        PObject* worldCube4_obj = new PObject("blend-cube-texture-1", "worldCube4_obj");
        scene1.pObjects.push_back(worldCube4_obj);
        worldCube4_obj->isActive = true;
        worldCube4_obj->transform->scale = { 2.0, 2.0, 2.0 };
        worldCube4_obj->transform->position  = { 20.0f, -10.0f, 10.0f };
        worldCube4_obj->transform_0->position = { 20.0f, -10.0f, 10.0f };
        worldCube4_obj->gravityOn = true;




        // 5. - TEMP - BOUNDING BOX ??
        PObject* cube_bounding_1 = new PObject("cube", "cube_bounding_1");
        scene1.pObjects.push_back(cube_bounding_1);
        cube_bounding_1->isActive = true;

        cube_bounding_1->transform->scale = { 0.5, 0.5, 0.5 };
        cube_bounding_1->transform->position = { -10.0f, -5.0f, 20.0f };




        // 6. -  HOUSE 1 - OBJ MODEL

        PObject* house1_obj = new PObject("house-1", "house1_obj");
        scene1.pObjects.push_back(house1_obj);
        house1_obj->isActive = true;

        house1_obj->transform->scale = { 1.0, 1.0, 1.0 };
        house1_obj->transform->position = { 10.0f, 20.0f, 3.0f };
        house1_obj->transform_0->position = house1_obj->transform->position;
        





        // 7. - TRIANGLE 2 - BOUNCE

        PObject* worldTriangle2_bounce = new PObject("tri_pso.pso", "worldTriangle2_bounce");
        scene1.pObjects.push_back(worldTriangle2_bounce);
        worldTriangle2_bounce->isActive = true;

        float triBounceScale = 5.0f;
        worldTriangle2_bounce->transform->scale = { 1.0f, triBounceScale, triBounceScale };
        worldTriangle2_bounce->transform_0->position = { 45.0f, 25.0f, 10.0f };
        worldTriangle2_bounce->transform->position = worldTriangle2_bounce->transform_0->position;
        worldTriangle2_bounce->transform->velocity = { 0.0f, 0.0f, 0.0f };
        worldTriangle2_bounce->transform_0->velocity = { -1.0f, -0.5f, 25.0f };



        // 8. - TRIANGLE 1 - TEXTURE

        PObject* worldTriangle1Texture = new PObject("tri_tex_pso.pso", "worldTriangle1Texture");
        scene1.pObjects.push_back(worldTriangle1Texture);
        worldTriangle1Texture->isActive = true;

        worldTriangle1Texture->transform->scale = { 1.0, 5.0, 5.0 };
        worldTriangle1Texture->transform->position = { -5.0f, -10.0f, 10.0f };



        /*
            9. - rendpipe_obj   ::   FIRST RENDER PIPLINE OBJECT
        */

        PObject* rendpipe_obj_ptr = new PObject("blend-cube-texture-1", "rendpipe_obj");
        scene1.pObjects.push_back(rendpipe_obj_ptr);

        rendpipe_obj_ptr->isActive = true;

        // WorldObject& rendpipe_obj = *rendpipe_obj_ptr;

        // rendpipe_obj.scale = { 0.5, 0.5, 0.5 };
        // rendpipe_obj.position_0 = { -10.0f, 0.0f, 5.0f };
        // rendpipe_obj.position = { -10.0f, 0.0f, 5.0f };



        /*
            10. - rendpipe_cube2_ptr   ::  TEST PIPLINE WITH NO TEXTURE OBJECT
        */
        // blend-cube-no-texture
        PObject* rendpipe_cube2_ptr = new PObject("cube", "rendpipe_cube2");
        scene1.pObjects.push_back(rendpipe_cube2_ptr);

        rendpipe_cube2_ptr->isActive = true;
        rendpipe_cube2_ptr->toggleWireframe();
        rendpipe_cube2_ptr->transform->position = { -10.0f, 0.0f, 5.0f };


        /*
            11. - ground_01   ::  TEST PIPLINE WITH NO TEXTURE OBJECT
        */
        // blend-cube-no-texture
        PObject* ground_01 = new PObject("ground-01", "ground_01");
        scene1.pObjects.push_back(ground_01);

        ground_01->isActive = true;

        float groundScale = 40; // currently scaled in blender..

        ground_01->transform->position = { -10.0f, 0.0f, GROUND_Z_0 };
        // worldCube1.printVertices();
        ground_01->boundingBox.x_min = ground_01->transform->position.data[0] - groundScale;
        ground_01->boundingBox.x_max = ground_01->transform->position.data[0] + groundScale;
        ground_01->boundingBox.y_min = ground_01->transform->position.data[1] - groundScale;
        ground_01->boundingBox.y_max = ground_01->transform->position.data[1] + groundScale;
        ground_01->boundingBox.z_min = ground_01->transform->position.data[2] + GROUND_Z_0;
        ground_01->boundingBox.z_max = ground_01->transform->position.data[2] + GROUND_Z_0;


        /*
            12. - tri_pso : First pso in new rendering pipline
        */
        PObject* tri_pso = new PObject("tri_pso.pso", "tri_pso");
        scene1.pObjects.push_back(tri_pso);

        tri_pso->isActive = true;

        tri_pso->transform->position = { -20.0f, 1.0f, 2.0f };


        /*
            13. - tri_tex_pso : First pso with texture in new rendering pipline
        */
        PObject* tri_tex_pso = new PObject("tri_tex_pso.pso", "tri_tex_pso");
        scene1.pObjects.push_back(tri_tex_pso);

        tri_tex_pso->isActive = true;

        tri_tex_pso->transform->position = { -20.1f, 1.0f, 3.0f };




        /*
            14. - wordlSim_1  - First WorldSimulator
         */

        WorldSimulator* worldSim_1 = new WorldSimulator("worldSim_1");
        scene1.worldSimulators.push_back(worldSim_1);

        worldSim_1->createSimContainer("cube-wire.pso", "worldSim_1_container");
        worldSim_1->simContainer->transform->scale = { 3.0, 3.0, 3.0 };
        worldSim_1->simContainer->transform->position = { -15.0f, -5.0f, 3.1f };
        worldSim_1->simContainer->isActive = true;

        Sim::Simulator* simulator1_ptr = Sim::getSim1Pointer();
        worldSim_1->setSimulator(simulator1_ptr);

        // 16. - worldSim_1_obj_1 - First simulator, first sim object
        PObject* worldSim_1_obj_1 = new PObject("cube", "worldSim_1_obj_1");
        worldSim_1->simulatorWorldObjects.push_back(worldSim_1_obj_1);
        worldSim_1_obj_1->isActive = true;
        worldSim_1_obj_1->transform->scale = { 0.5, 0.5, 0.5 };
        worldSim_1_obj_1->transform_0->position = { 0.0f, 0.0f, 0.0f };
        worldSim_1_obj_1->transform->position = { 0.0f, 0.0f, 0.0f };






        /*
            14. transform_1 : first WO with a transform-object AND a child object
        */

        PObject* transform_1 = new PObject("cube", "transform_1");
        scene1.pObjects.push_back(transform_1);

        transform_1->transform->scale = { 0.1, 0.1, 0.1 };
        transform_1->transform->position = { -25.0f, -5.0f, 2.0f };
        transform_1->transform->velocity = { 0.0f, -0.1f, 0.0f };
        transform_1->transform->rotation = { 0.0f, 0.0f, 0.0f };
        transform_1->transform->angularVelocity = { 0.0f, 0.01f, 0.1f };
        transform_1->isActive = true;
        // transform_1->gravityOn = true;

        PObject* transform_1_child_1 = new PObject("cube", "transform_1_child_1");
        transform_1_child_1->parent = transform_1;
        transform_1->children.push_back(transform_1_child_1);

        transform_1_child_1->transform->scale = { 0.05, 0.05, 0.05 };
        transform_1_child_1->transform->position = { 0.0f, 0.0f, 1.0f };
        transform_1_child_1->transform->rotation = { 0.0f, 0.0f, 0.0f };
        transform_1_child_1->transform->angularVelocity = { 0.0f, 0.0f, 0.1f }; // not being updated in 'physics'
        transform_1_child_1->isActive = true;




        // Set Single variables using object names
        for (PObject* pObject_p : scene1.pObjects ) {
            PObject& pObject = *pObject_p;
            if (pObject.name == "worldTriangle2_bounce")
                bouncyTrianglePointer = &pObject;
            if (pObject.name == "ground_01")
                groundObjectPointerScene1 = ground_01;
        }

    }

}




