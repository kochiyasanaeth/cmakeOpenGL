#include <iostream>
#include <cstdlib>
#include "glew.h"
#include "glfw3.h"
#include <gl\gl.h>
#include "shadererr.hpp"

namespace myShader {
    void shaderErr::printShaderLog(GLuint shader){
        int lenth;
        int chWrittn = 0;
        char *log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&lenth);
        if(lenth > 0)
        {
            log = new char[lenth];
            glGetShaderInfoLog(shader,lenth,&chWrittn,log);
            std::cout << log << std::endl;
        }
    }
    void shaderErr::printProgramLog(GLuint shader) {
        int lenth;
        int chwrittn = 0;
        char *log;
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &lenth);
        std::cout<< __LINE__ << "log" << lenth<< std::endl;
        if(lenth > 0)
        {
            log = new char[lenth];
            glGetProgramInfoLog(shader,lenth,&chwrittn,log);
            std::cout << log << std::endl;
        }
    }
    bool shaderErr::checkGLerr(){
        bool iserr = false;
        int glErr = glGetError();
        while(glErr != GL_NO_ERROR)
        {
            std::cout << "glErr:" << glErr << std::endl;
            iserr = true;
            glErr = glGetError();
        }
        return iserr;
    }
}