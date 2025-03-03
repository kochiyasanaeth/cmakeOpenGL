#include <iostream>
#include <cstdlib>
#include "glew.h"
#include "glfw3.h"
#include <gl\gl.h>


int main(int argc, char* argv[])
{
    if(!glfwInit())
    {
        std::cout << "errorinit" << std::endl;

    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow* _window = glfwCreateWindow(800,600,"hello_world",NULL,NULL);
    glfwMakeContextCurrent(_window);
    if(glewInit() != GLEW_OK)
    {
        std::cout << "errorinit" << std::endl;
    } 
    while ((!glfwWindowShouldClose(_window)))
    {
        [&] {
            glClearColor(1.0f,1.0f,0.0f,0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        };
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
    glfwDestroyWindow(_window);
    glfwTerminate();
    system("pause");
}

