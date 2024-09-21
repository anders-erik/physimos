
#include "WorldRenderer.hpp"

#include "Model.hpp"

#include <glad/glad.h>
// #include <GLFW/glfw3.h>


#include "WorldObject.hpp"
// #include "simulator.hpp"
#include "WorldScene.hpp"

#include "shader.hpp"

#include "bmp_loader.hpp"

#include "Simulation.hpp"

#include "Camera.hpp"

// WORLD OBJECT CLASSES
#include "SimWorldContainer.hpp"




extern WorldObject worldGround1;

extern WorldObject worldCube1;
extern WorldObject worldCube_spin;

extern WorldObject worldTriangle1Texture;
extern WorldObject worldTriangle2_bounce;


extern unsigned int mountainTexture;

Shader worldShader;
unsigned int transformLoc;
unsigned int viewLoc;
unsigned int sanityLoc;
unsigned int perspectiveLoc;
unsigned int colorLoc;
unsigned int hasTextureLoc;

Shader worldObjShader;
unsigned int modelObjLoc;
unsigned int viewObjLoc;
unsigned int sanityObjLoc;
unsigned int perspectiveObjLoc;

Shader wireframeShader;
unsigned int modelWireLoc;
unsigned int viewWireLoc;
unsigned int sanityWireLoc;
unsigned int perspectiveWireLoc;

// NEW GROUND


const float sanityMatrix16[16] = {
    0, -1, 0, 0,
    0, 0, 1, 0,
    -1, 0, 0, 0,
    0, 0, 0, 1,
};





// TEMP SIMULATION UPDATER
extern SimState simState;
void update_sim_timestep(){
    
}


// unsigned int transformLoc_;

void wr_init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    worldShader.buildShaderProgram("src/shaders/worldShader.vs", "src/shaders/worldShader.fs");

    glUseProgram(worldShader.ID);
    transformLoc = glGetUniformLocation(worldShader.ID, "transform");
    viewLoc = glGetUniformLocation(worldShader.ID, "view");
    sanityLoc = glGetUniformLocation(worldShader.ID, "sanityTransform");
    perspectiveLoc = glGetUniformLocation(worldShader.ID, "perspective");
    colorLoc = glGetUniformLocation(worldShader.ID, "vertexColor");
    hasTextureLoc = glGetUniformLocation(worldShader.ID, "hasTexture");

    glUniformMatrix4fv(sanityLoc, 1, GL_TRUE, sanityMatrix16);
    glUniformMatrix4fv(perspectiveLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
    glUniformMatrix4fv(viewLoc, 1, GL_TRUE, cam_getViewMatrix());



    worldObjShader.buildShaderProgram("src/shaders/worldObjShader_vs.glsl", "src/shaders/worldObjShader_fs.glsl");

    glUseProgram(worldObjShader.ID);
    modelObjLoc = glGetUniformLocation(worldObjShader.ID, "model");
    viewObjLoc = glGetUniformLocation(worldObjShader.ID, "view");
    sanityObjLoc = glGetUniformLocation(worldObjShader.ID, "sanityTransform");
    perspectiveObjLoc = glGetUniformLocation(worldObjShader.ID, "perspective");
    // colorObjLoc = glGetUniformLocation(worldObjShader.ID, "vertexColor");
    // hasTextureObjLoc = glGetUniformLocation(worldObjShader.ID, "hasTexture");

    glUniformMatrix4fv(sanityObjLoc, 1, GL_TRUE, sanityMatrix16);
    glUniformMatrix4fv(perspectiveObjLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
    glUniformMatrix4fv(viewObjLoc, 1, GL_TRUE, cam_getViewMatrix());

    
    wireframeShader.buildShaderProgram("src/shaders/worldWireframeShader_vs.glsl", "src/shaders/worldWireframeShader_fs.glsl");

    glUseProgram(wireframeShader.ID);
    modelWireLoc = glGetUniformLocation(wireframeShader.ID, "model");
    viewWireLoc = glGetUniformLocation(wireframeShader.ID, "view");
    sanityWireLoc = glGetUniformLocation(wireframeShader.ID, "sanityTransform");
    perspectiveWireLoc = glGetUniformLocation(wireframeShader.ID, "perspective");

    glUniformMatrix4fv(sanityWireLoc, 1, GL_TRUE, sanityMatrix16);
    glUniformMatrix4fv(perspectiveWireLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
    glUniformMatrix4fv(viewWireLoc, 1, GL_TRUE, cam_getViewMatrix());
}



void wr_render(std::vector<WorldObject>& _worldObjects) {
    // glUseProgram(worldShader.ID);
    
    // glUniformMatrix4fv(perspectiveLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
    // glUniformMatrix4fv(perspectiveLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
    // glUniformMatrix4fv(viewLoc, 1, GL_TRUE, cam_getViewMatrix());




    // RENDER WORLD OBJECTS !
    for(WorldObject& _worldObject : _worldObjects){
        // std::cout << "_worldObject.name = " << _worldObject.name << std::endl;
        
        if(!_worldObject.isActive){
            continue;
        }



        /* 

            USING NEW RENDERER
        
        */
        // Try RTTI determination
        // WorldObject* _worldObject_ptr = &_worldObject;
        // try
        // {
        //     SimWorldContainer* sim = dynamic_cast<SimWorldContainer*>(&_worldObject);
        //     std::cout << "" << sim << std::endl;
            
        // }
        // catch(const std::exception& e)
        // {
        //     std::cerr << e.what() << '\n';
        // }

        // SimWorldContainer* sim_2 = dynamic_cast<SimWorldContainer*>(&_worldObject)
            
        // std::cout << "sim_2 = " << &sim_2 << std::endl;
        
        // std::cout << "&_worldObject = s" << &_worldObject << std::endl;
        
        // if (SimWorldContainer* sim_2 = dynamic_cast<SimWorldContainer*>(&_worldObject))
        //     std::cout << "RTTI WORKS!!!!!!" << std::endl;
        
        // std::cout << "YELLO"  << std::endl;
        
            
        if (_worldObject.name == "simContainer_1"){
            // std::cout << _worldObject.name << " : HAS WIREFRAME" << std::endl;

            _worldObject.renderer.shader->use();
            glBindVertexArray(_worldObject.renderer.vao);
            
            _worldObject.SetModelMatrixRowMajor();
            
            glUniformMatrix4fv(perspectiveWireLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
            glUniformMatrix4fv(viewWireLoc, 1, GL_TRUE, cam_getViewMatrix());
            glUniformMatrix4fv(modelWireLoc, 1, GL_TRUE, _worldObject.modelMatrixRowMajor);
            glBindVertexArray(_worldObject.renderer.vao);
            glDrawArrays(GL_LINES, 0, _worldObject.vertices.size() / 3);

            continue;
        }
        


        
        _worldObject.shader->use();


        // Add wireframe
        if(_worldObject.hasRigidBody){
            // std::cout << _worldObject.name << " : HAS WIREFRAME" << std::endl;
            _worldObject.rigidBody.shader->use();
            glUniformMatrix4fv(perspectiveWireLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
            glUniformMatrix4fv(viewWireLoc, 1, GL_TRUE, cam_getViewMatrix());
            glUniformMatrix4fv(modelWireLoc, 1, GL_TRUE, _worldObject.modelMatrixRowMajor);
            glBindVertexArray(_worldObject.rigidBody.vao);
            glDrawArrays(GL_LINES, 0, _worldObject.rigidBody.vertices.size()/3);
        }


        glBindVertexArray(_worldObject.vao);
        _worldObject.SetModelMatrixRowMajor();

        
        // WORLD SHADER
        if(_worldObject.shader->ID == worldShader.ID){
            _worldObject.shader->use();

            glUniformMatrix4fv(perspectiveLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
            glUniformMatrix4fv(viewLoc, 1, GL_TRUE, cam_getViewMatrix());
            glUniformMatrix4fv(transformLoc, 1, GL_TRUE, _worldObject.modelMatrixRowMajor);

            if (_worldObject.hasTexture) {
                glUniform1i(hasTextureLoc, 1); // set texture bool
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, _worldObject.glTexture);
            }
            else {
                glUniform1i(hasTextureLoc, 0); // unset texture bool
            }
            
            glDrawArrays(GL_TRIANGLES, 0, _worldObject.vertexCount);

            glUniform1i(hasTextureLoc, 0);
            
        }
        // WORLD OBJ SHADER
        else if(_worldObject.shader->ID == worldObjShader.ID){
            // std::cout << "WORLD OBJ SHADER" << std::endl;

            _worldObject.shader->use();

            glUniformMatrix4fv(perspectiveObjLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
            glUniformMatrix4fv(viewObjLoc, 1, GL_TRUE, cam_getViewMatrix());
            glUniformMatrix4fv(modelObjLoc, 1, GL_TRUE, _worldObject.modelMatrixRowMajor);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _worldObject.model.glTexture);
            // if (_worldObject.hasTexture) {
            //     glUniform1i(hasTextureObjLoc, 1); // set texture bool
                
            // }
            // else {
            //     glUniform1i(hasTextureObjLoc, 0); // unset texture bool
            // }

            glDrawArrays(GL_TRIANGLES, 0, _worldObject.model.vertexCount);
            // std::cout << "_worldObject.name = " << _worldObject.name << std::endl;
            // std::cout << "_worldObject.model.glTexture = " << _worldObject.model.glTexture << std::endl;
            // std::cout << "_worldObject.model.vertexCount = " << _worldObject.model.vertexCount << std::endl;
            // std::cout << "_worldObject.model.vertices.size() = " << _worldObject.model.vertices.size() << std::endl;
            

            // glUniform1i(hasTextureObjLoc, 0);
            
        }

    }




}













