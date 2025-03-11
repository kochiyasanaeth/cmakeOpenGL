#include <iostream>
#include <cstdlib>
#include "glew.h"
#include "glfw3.h"
#include <gl\gl.h>
#include "../../inc/shaderopt.hpp"
#include "cstdio"
#include <thread>
#include <chrono>

struct dumpLog
{
    void operator()() {
        if(_strv.size() != 0)  {
            std::cout << "tag cnt" << _strv.size() << std::endl;
            for(auto i = _strv.begin();i != _strv.end();i++) {
                std::cout << *i << std::endl;
            }
        }
        if(_intv.size() != 0) {
            std::cout << "info cnt" << _intv.size() << std::endl;
            for(auto i = _intv.begin();i != _intv.end();i++) {
                std::cout << *i << std::endl;
            }
        }
    }
    dumpLog(std::vector<std::string> strv,std::vector<int> intv) :
    _strv(strv),_intv(intv)
    {;}
private:
    std::vector<std::string> _strv;
    std::vector<int> _intv;
};
myShader::shaderSource *sdp = NULL;
GLuint creShaderProgram()
{
    using msd = myShader::shaderSource; 
    sdp = new myShader::shaderSource{
        std::pair<msd::shaderType,std::string>(msd::vertexShader,"triangledraw.vert"),\
        std::pair<msd::shaderType,std::string>(msd::fragmentShader,"trianglecolor.frag"),
    };
    sdp->shadersCompileProgramLink();
    GLuint ret = *(static_cast<GLuint *>(sdp->data()));
    dumpLog(sdp->errLogInfoData(),sdp->errLogTagData())();
    return ret;
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
    delete sdp;
}

