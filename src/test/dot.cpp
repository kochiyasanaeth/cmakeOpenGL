#include <iostream>
#include <cstdlib>
#include "glew.h"
#include "glfw3.h"
#include <gl\gl.h>
#include "shaderopt.hpp"

GLint creShaderProgram()
{
    const char * vshadersource= 
    "#version 430\n"
    "void main(void)\n"
    "{gl_Position = vec4(0.0,0.0,0.0,1.0);};"
    ;
    const char * fshadersource= 
    "#version 430\n"
    "out vec4 color;\n"
    "void main(void)\n"
    "{color = vec4(0.0,0.0,1.0,0.0);};"
    ;
    GLint vshader = glCreateShader(GL_VERTEX_SHADER);
    GLint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vshader,1,&vshadersource,NULL);
    glShaderSource(fshader,1,&fshadersource,NULL);
    glCompileShader(vshader);
    GLint verCompiled;
    GLint fraCompiled;
    myShader::shaderErr::checkGLerr();
    glGetShaderiv(vshader,GL_COMPILE_STATUS,&verCompiled);
    if(verCompiled != 1)
    {
        std::cout << "vertex compilation failed" << std::endl;
        myShader::shaderErr::printShaderLog(vshader);
    }
    glCompileShader(fshader);
    myShader::shaderErr::checkGLerr();
    glGetShaderiv(fshader,GL_COMPILE_STATUS,&fraCompiled);
    if(fraCompiled != 1)
    {
        std::cout << "vertex compilation failed" << std::endl;
        myShader::shaderErr::printShaderLog(fshader);
    }
    GLuint vfprogram = glCreateProgram();
    glAttachShader(vfprogram,vshader);
    glAttachShader(vfprogram,fshader);
    glLinkProgram(vfprogram);
    GLint linkst;
    myShader::shaderErr::checkGLerr();
    glGetProgramiv(vfprogram,GL_LINK_STATUS,&linkst);
    if(linkst != 1)
    {
        std::cout << "link failed" << std::endl;
        myShader::shaderErr::printProgramLog(vfprogram);
    }
    return vfprogram;
}

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
    GLuint renProgram;
    renProgram = creShaderProgram();
    GLuint vao[1];
    glGenVertexArrays(1,vao);
    glBindVertexArray(vao[0]);
    while ((!glfwWindowShouldClose(_window)))
    {
        glUseProgram(renProgram);
        glDrawArrays(GL_POINT,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
    glfwDestroyWindow(_window);
    glfwTerminate();
    system("pause");
}

