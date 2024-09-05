#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <time.h> 
#include <unistd.h>

#include <cstdio>

#include "Timing.hpp"

// timing
#define TARGET_FPS 60
#define FRAME_DURATION (1000 / TARGET_FPS) // Duration of each frame in milliseconds

double glfwTime;

int current_fps = 0;

clock_t currentTime = 0;
clock_t lastFrameTime = 0;

time_t epoch_fps, current_second_fps, current_time_fps, last_frame_fps;
int secondCount;
struct timespec wait = { 0, 20000000L }; // nanoseconds of added wait between each frame
struct timespec waitForFrame = { 0, 0L }; // nanoseconds of added wait between each frame



    

void timing_init(){
    // INITIAL CLOCK INFO
    printf("CLOCKS_PER_SEC = %li \n", CLOCKS_PER_SEC);

    glfwTime = glfwGetTime();
    printf("glfwTime1 = %f \n", glfwTime);
    glfwTime = glfwGetTime();
    printf("glfwTime2 = %f \n", glfwTime);

    time(&epoch_fps);

    
}


void timing_newFrame() {
    //
            // FPS INFO
            //

            // time(&last_frame_fps);
    time(&current_time_fps);
    ++secondCount;
    // entered new second
    if (current_time_fps > current_second_fps)
    {
        current_second_fps = current_time_fps;
        printf("FPS: %d \n", secondCount);
        current_fps = secondCount;
        // printf("%d \n", (int)(current_time - epoch_time));
        secondCount = 0;
    }

}

void timing_waitForNextFrame() {
    // Keep track of time since last frame and wait to match the target frame rate
    currentTime = clock();
    double timeTaken = (double)(currentTime - lastFrameTime) / CLOCKS_PER_SEC * 1000.0;

    if (timeTaken < FRAME_DURATION)
    {
        usleep((FRAME_DURATION - timeTaken) * 1000); // Sleep for the remaining time
    }

    lastFrameTime = clock();

}