#include<vector>
#include<string>
#include<utility> 
#include <iostream>
#include <cstdlib>
#include "glew.h"
#include "glfw3.h"
#include <gl/gl.h>
#include"scene.hpp"
namespace myScene{
    class scene::_sceWindow{ //pImpl
    public:
        _sceWindow() : __win(NULL){std::cout << "__win:" << __win <<std::endl;};
        GLFWwindow* _getWinptr() {
            return __win;
        }
        void _setWinptr(GLFWwindow* wptr) {
            std::cout << "__win:" << __win << "wptr:" << wptr << std::endl;
            __win = wptr;
        }
    private:
        GLFWwindow* __win;
    };

    sceneElement::sceneElement(std::string name) {
        this->_name = name;
    }
    std::string sceneElement::getName(void) {
       return this->_name;
    }
    void sceneElement::setName(std::string name) {
        this->_name = name;
    }

    scene::scene(std::string sceneName,uint32_t Wwidth,uint32_t Wheight) \
    : _sceneName(sceneName),_Wwidth(Wwidth),_Wheight(Wheight),_Wptr(std::make_unique<_sceWindow>()), \
    _display ( [&]{;})
    {
        ;
    }
    bool scene::check()
    {
        if(!glfwInit()) {
            std::cout << "glfwInit error" << std::endl;
            return false;
        };
        if(glewInit() != GLEW_OK) {
            std::cout << "glewInit error" << std::endl;
            return false;
        }
        return true;
    }
    void scene::createW() {
        if(!glfwInit()) {
            std::cout << "glfwInit error" << std::endl;
        };
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        GLFWwindow* win = glfwCreateWindow(this->_Wheight,this->_Wheight,this->_sceneName.data(),NULL,NULL);
        glfwMakeContextCurrent(win);
        _Wptr->_setWinptr(win);
    }
    void scene::setDisplayFunction(std::function<void()> displayfunction) {
        this->_display = displayfunction;
    }
    void scene::sceneDisplay() {
        glClear(GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        this->_display();
        glfwSwapBuffers(this->_Wptr->_getWinptr());
        glfwPollEvents();
    }
    void scene::importElementData(sceneElement element) {
        this->_elementslist.insert({element.getName(),element});
    }
    size_t scene::getElementSize(void) {
        return this->_elementslist.size();
    }
    void scene::removeElement(std::string elename) {
        this->_elementslist.erase(elename);
    }
    sceneElement scene::getElementData(std::string elename) {
        auto i = this->_elementslist.find(elename);
        if(i != this->_elementslist.end()) {
        }
        return i->first;
    }
    bool scene::shouldCloseW() {
        return glfwWindowShouldClose(this->_Wptr->_getWinptr());
    }
    scene::~scene(){
        glfwDestroyWindow(this->_Wptr->_getWinptr());
        glfwTerminate();
    }
}