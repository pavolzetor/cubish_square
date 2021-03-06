#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "cube.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "renderer.hpp"
#include "updatecubestate.hpp"
#include "vec.hpp"

#ifdef DEBUG
#define TIME(func, msg)\
{\
    const double start = glfwGetTime();\
    (func);\
    std::cout << (msg) << ": " << std::fixed << (glfwGetTime() - start) << std::endl;\
}
#else
#define TIME(func, msg) (func)
#endif

static void error_callback(int error, const char *desc)
{
    std::cerr << desc << std::endl;
}


void get_key_callback(GLFWwindow *window, int key, int scancode,
                      int action, int mods)
{
    /* TODO static, dynamic cast? */
    Input *input = (Input *)glfwGetWindowUserPointer(window);
    input->get_key_callback(key, scancode, action, mods);
}


int main(void)
{
    const int width = 800;
    const int height = 600;

    /* initialize GLFW window and OpenGL context */
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE); 
    }

    /* OpenGL 4.4 with debug */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(width, height, "Cubish Square", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create window" << std::endl;
        std::exit(EXIT_FAILURE);
    }
        
    glfwMakeContextCurrent(window);

    /* initialize GLEW to load OpenGL extensions */
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(window);
        glfwTerminate();
        std::cerr << "Failed to initialize GLEW" << std::endl;
        std::exit(EXIT_FAILURE);
    }
   
    /* general initialization code */ 
    const Cube cube1{0.0, 0.0, 0.0, 1.0};
    const Cube cube2{0.0, 1.0, 0.0, 2.0};
    const Cube cube3{2.0, 0.0, 0.0, 1.0};
    const Cube cube4{10.0, 55.0, 0.0, 5.0};
    std::vector<Cube> cubes{cube1, cube2, cube3, cube4};

    Renderer renderer{width, height}; 
    
    Input input{};
    glfwSetWindowUserPointer(window, (void *)&input);
    glfwSetKeyCallback(window, get_key_callback); 
    
    while (!glfwWindowShouldClose(window)) {
        TIME(update_master_state(cubes, input.return_key()), "Processing keys");
        
        TIME(physics_update(cubes, 0.016666666), "Processing physics");

        glClear(GL_COLOR_BUFFER_BIT);

        TIME((renderer.draw(cubes), glFinish()), "Rendering");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* free memory */
    glfwDestroyWindow(window);
    glfwTerminate();

    std::exit(EXIT_SUCCESS);
}
