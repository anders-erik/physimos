
#include "WorldRenderer.hpp"

#include "Model.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "WorldObject.hpp"

#include "shader.hpp"

#include "bmp_loader.hpp"

#include "Simulation.hpp"

#include "Camera.hpp"

Shader worldShader;

WorldObject worldGround1;

WorldObject worldCube1;
WorldObject worldCube_spin;

WorldObject worldTriangle1Texture;
WorldObject worldTriangle2_bounce;


unsigned int texture;


unsigned int transformLoc;
unsigned int viewLoc;
unsigned int sanityLoc;
unsigned int perspectiveLoc;
unsigned int colorLoc;
unsigned int hasTextureLoc;

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

    


    // GROUND 1

   // WorldObject worldCube1("src/models/cube.pso");
    worldGround1.LoadWorldObject("src/models/ground_1.pso");

    // worldCube1.scale = {2.0, 2.0, 2.0};
    float groundScale = 50.0f;
    worldGround1.scale = { groundScale, groundScale, 1.0f };
    worldGround1.position = { 50.0f, 0.0f, -19.0f };
    // worldCube1.printVertices();

    worldGround1.setVaoVbo330();
    worldGround1.setShaderProgram(&worldShader);


    
    // CUBE 1

    // WorldObject worldCube1("src/models/cube.pso");
    worldCube1.LoadWorldObject("src/models/cube.pso");

    // worldCube1.scale = {2.0, 2.0, 2.0};
    worldCube1.scale = { 0.5, 0.5, 0.5 };
    worldCube1.position = { 20.0f, 0.0f, 0.0f };
    // worldCube1.printVertices();

    worldCube1.setVaoVbo330();
    worldCube1.setShaderProgram(&worldShader);



    // CUBE 2 - SPIN

    // WorldObject worldCube1("src/models/cube.pso");
    worldCube_spin.LoadWorldObject("src/models/cube.pso");

    // worldCube1.scale = {2.0, 2.0, 2.0};
    float cubeSpinScale = 1.0f;
    worldCube_spin.scale = { cubeSpinScale, cubeSpinScale, cubeSpinScale };
    worldCube_spin.position = { 5.0f, 0.0f, 3.0f };
    // worldCube1.printVertices();

    worldCube_spin.setVaoVbo330();
    worldCube_spin.setShaderProgram(&worldShader);


    // TRIANGLE 2 - BOUNCE

    // WorldObject worldCube1("src/models/cube.pso");
    worldTriangle2_bounce.LoadWorldObject("src/models/firstSimTriangle.pso");

    // worldCube1.scale = {2.0, 2.0, 2.0};
    // float  = 1.0f;
    float triBounceScale = 20.0f;
    worldTriangle2_bounce.scale = { 1.0f, triBounceScale, triBounceScale };
    worldTriangle2_bounce.position = { 45.0f, 25.0f, -0.0f };
    worldTriangle2_bounce.position_0 = { 45.0f, 25.0f, -0.0f };
    worldTriangle2_bounce.velocity = { 0.0f, 0.0f, 0.0f };
    worldTriangle2_bounce.velocity_0 = { -1.0f, -0.5f, 25.0f };
    // worldCube1.printVertices();

    worldTriangle2_bounce.setVaoVbo330();
    worldTriangle2_bounce.setShaderProgram(&worldShader);



    // TRIANGLE 1 - TEXTURE

    // WorldObject worldTriangle1("src/models/triangle.pso");
    worldTriangle1Texture.LoadWorldObject("src/models/triangle.pso");

    worldTriangle1Texture.scale = { 10.0, 10.0, 10.0 };
    worldTriangle1Texture.position = { -5.0f, 0.0f, 0.0f };

    worldTriangle1Texture.setVaoVbo332();
    worldTriangle1Texture.setShaderProgram(&worldShader);

    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // bmp_loader.loadBMPFile("media/net_100x100.bmp");
    bmp_loader_loadBMPFile("media/mountain.bmp");
    // bmp_loader.prettyPrint();
    // std::cout << "UI image buffer size: " << ui.charImageBuffer.size() << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp_getWidth(), bmp_getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, bmp_getImageDataBuffer().data());


    // Generate a black and white test 'image'
    int width = 100;
    int height = 100;
    // Generate black and white texture
    unsigned char data[3 * height * width]; // = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, };

    for (long unsigned int rgb_i = 0; rgb_i < sizeof(data); rgb_i += 3) {
        // std::cout << rgb_i << " ";
        int r = rand();

        if (r < 1073741823) {
            data[rgb_i] = 0;
            data[rgb_i + 1] = 0;
            data[rgb_i + 2] = 0;
        }
        else {
            data[rgb_i] = 255;
            data[rgb_i + 1] = 255;
            data[rgb_i + 2] = 255;
        }
    }
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}



void wr_render() {
    glUseProgram(worldShader.ID);
    
    // glUniformMatrix4fv(perspectiveLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
    glUniformMatrix4fv(perspectiveLoc, 1, GL_TRUE, cam_getPerspectiveMatrix());
    glUniformMatrix4fv(viewLoc, 1, GL_TRUE, cam_getViewMatrix());

    // GROUND 1
    worldGround1.shader->use();
    glBindVertexArray(worldGround1.vao);

    // worldCube1.Rotate({ 0.03f, 0.01f, 0.0f });
    // worldGround1.Translate({ 0.03f, 0.01f, -0.01f });
    // worldCube1.SetScale({ 100.0f, 100.0f, 100.0f });
    worldGround1.SetTransformMatrixRowMajor();

    // SetSimObjectTranform(&cube1);

    // float copy[16] = ()

    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, worldGround1.transformMatrixRowMajor);

    // // glUniformMatrix4fv(sanityLoc, 1, GL_TRUE, sanityMatrix16);

    // glUniform4f(colorLoc, 0.5f, 0.0f, 0.0f, 1.0f); // https://learnopengl.com/Getting-started/Shaders
    glDrawArrays(GL_TRIANGLES, 0, 6);




    // CUBE 1
    worldCube1.shader->use();
    glBindVertexArray(worldCube1.vao);

    // worldCube1.Rotate({ 0.03f, 0.01f, 0.0f });
    worldCube1.Translate({ 0.03f, 0.01f, -0.01f });
    // worldCube1.SetScale({ 100.0f, 100.0f, 100.0f });
    worldCube1.SetTransformMatrixRowMajor();

    // SetSimObjectTranform(&cube1);


    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, worldCube1.transformMatrixRowMajor);

    // glUniform4f(colorLoc, 0.5f, 0.0f, 0.0f, 1.0f); // https://learnopengl.com/Getting-started/Shaders
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // CUBE 2
    worldCube_spin.shader->use();
    glBindVertexArray(worldCube_spin.vao);

    worldCube_spin.Rotate({ 0.03f, 0.01f, 0.0f });
    // worldCube_spin.Translate({ 0.03f, 0.01f, -0.01f });
    // worldCube1.SetScale({ 100.0f, 100.0f, 100.0f });
    worldCube_spin.SetTransformMatrixRowMajor();
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, worldCube_spin.transformMatrixRowMajor);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // TRIANGLE 1

    worldTriangle1Texture.shader->use();
    glUniform1i(hasTextureLoc, 1); // set texture bool
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(worldTriangle1Texture.vao);
    worldTriangle1Texture.SetTransformMatrixRowMajor();
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, worldTriangle1Texture.transformMatrixRowMajor);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUniform1i(hasTextureLoc, 0); // unset texture bool


    // TRIANGLE - BOUNCE
    worldTriangle2_bounce.shader->use();
    glBindVertexArray(worldTriangle2_bounce.vao);

    // worldTriangle2_bounce.Rotate({ 0.03f, 0.01f, 0.0f });
    // worldTriangle2_bounce.Translate({ 0.03f, 0.01f, -0.01f });
    // worldTriangle2_bounce.SetScale({ 100.0f, 100.0f, 100.0f });
    worldTriangle2_bounce.SetTransformMatrixRowMajor();
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, worldTriangle2_bounce.transformMatrixRowMajor);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

















void wr_init_old() {

}



void wr_render_old(unsigned int transformLoc_) {


}

void modelFunction(){
    printf("In WorldRendered.cpp.o \n");
    printX();
    setX(3);
    printX();
    printf("Return from WorldRendered.cpp.o \n");
}

