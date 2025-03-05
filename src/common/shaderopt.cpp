#include <iostream>
#include <cstdlib>
#include "glew.h"
#include "glfw3.h"
#include <gl\gl.h>
#include "shaderopt.hpp"
#include <filesystem>
#include <fstream>
#include <string>
namespace myShader {
    void shaderErr::printShaderLog(GLuint shader) {
        int lenth;
        int chWrittn = 0;
        char *log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&lenth);
        if(lenth > 0) {
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
        if(lenth > 0) {
            log = new char[lenth];
            glGetProgramInfoLog(shader,lenth,&chwrittn,log);
            std::cout << log << std::endl;
        }
    }
    bool shaderErr::checkGLerr(){
        bool iserr = false;
        int glErr = glGetError();
        while(glErr != GL_NO_ERROR) {
            std::cout << "glErr:" << glErr << std::endl;
            iserr = true;
            glErr = glGetError();
        }
        return iserr;
    }
    shaderLoad::shaderLoad(std::string path) {
        std::filesystem::path _pth(path);
        if(!std::filesystem::exists(_pth)) {
            std::cout << __FILE__ << ':'  << __LINE__ << ' ' << path << "not exist" << std::endl;
        }
        std::ifstream _file(path, std::ios::in);
        if(!_file.is_open()) {
            std::cout << __FILE__ << ':'  << __LINE__ << ' ' << path << "open fail" << std::endl;
        }
        this->_shdrContent =  std::string(std::istreambuf_iterator<char>(_file), std::istreambuf_iterator<char>());
    }
    shaderLoad& shaderLoad::shaderImportContent(std::string path){
        std::filesystem::path _pth(path);
        if(!std::filesystem::exists(_pth)) {
            std::cout << __FILE__ << ':'  << __LINE__ << ' ' << path << "not exist fail" << std::endl;
        }
        std::ifstream _file(path);
        if(!_file.is_open()) {
            std::cout << __FILE__ << ':'  << __LINE__ << ' ' << path << "open fail" << std::endl;
        }
        this->_shdrContent =  std::string(std::istreambuf_iterator<char>(_file), std::istreambuf_iterator<char>());
        return *this;
    }
    std::string shaderLoad::shaderGetContent(void) {
        if(this->_shdrContent.size() != 0) {
            return this->_shdrContent;
        }
        return nullptr;
    }
    char *shaderLoad::shaderGetCharContent(void) {
        printf("output%s",this->_shdrContent.data());
        return this->_shdrContent.data();
    }
    size_t shaderLoad::shaderGetContentSize(void) {
        return this->_shdrContent.size();
    }
}