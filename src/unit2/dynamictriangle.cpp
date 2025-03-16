#include <iostream>
#include <cstdlib>
#include "glew.h"
#include "glfw3.h"
#include <gl/gl.h>
#include "shaderopt.hpp"
#include "cstdio"
#include <thread>
#include <chrono>
GLint creShaderProgram()
{
    const char * fshadersource= 
    "#version 430\n"
    "out vec4 color;\n"
    "void main(void)\n"
    "{color = vec4(0.0,1.0,1.0,1.0);};"
    ;
    myShader::shaderLoad _vsd("./triangledraw.vert");
    char * vshadersource = NULL;
    if(_vsd.shaderGetContentSize()) {
        vshadersource = _vsd.shaderGetCharContent();
    }
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
    float _x = 0.0f;
    float inc = 0.1f;
    while ((!glfwWindowShouldClose(_window)))
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0,0.0,0.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(renProgram);
        _x += inc;
        std::cout << _x << std::endl;
        if(_x > 1.0f) {
            inc = -0.1f;
        }
        else if( _x < -1.0f) {
            inc = 0.1f;
        }
        GLint _offsetLoc = glGetUniformLocation(renProgram,"xoffset");
        glProgramUniform1f(renProgram,_offsetLoc,_x);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(_window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    glfwDestroyWindow(_window);
    glfwTerminate();
}

