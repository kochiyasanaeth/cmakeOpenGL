#include<iostream>
#include<cstdlib>
#include<cmath>
#include<glm.hpp>
#include<gtc/type_ptr.hpp>
#include<gtc/matrix_transform.hpp>
#include "shaderopt.hpp"
#include"glew.h"
#include"glfw3.h"
#include"gl/GL.h"

float cameraX,cameraY,cameraZ;
float cubeLocx,cubeLocy,cubeLocz;
constexpr int numVaos = 1,numVbos = 2;
GLuint vao[numVaos];
GLuint vbo[numVbos];
int width,height;
float aspect;
glm::mat4 pMat,mMat,vMat,mvMat,tMat,rMat;
GLuint renProgram;
static float vertexPosition[] = {
    -1.0f, 1.0f,-1.0f, -1.0f,-1.0f,-1.0f,  1.0f,-1.0f,-1.0f,//00
     1.0f,-1.0f,-1.0f,  1.0f, 1.0f,-1.0f, -1.0f, 1.0f,-1.0f,//01
     1.0f,-1.0f,-1.0f,  1.0f,-1.0f, 1.0f,  1.0f, 1.0f,-1.0f,//02
     1.0f,-1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,-1.0f,//03
     1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f,  1.0f, 1.0f, 1.0f,//04
    -1.0f,-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,//05
    -1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f, -1.0f, 1.0f, 1.0f,//06
    -1.0f,-1.0f,-1.0f, -1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f,//07
    -1.0f,-1.0f, 1.0f,  1.0f,-1.0f, 1.0f,  1.0f,-1.0f,-1.0f,//08
     1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f,//09
    -1.0f, 1.0f,-1.0f,  1.0f, 1.0f,-1.0f,  1.0f, 1.0f, 1.0f,//10
     1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f,//11
};
GLuint mvLoc,proLoc;
using msd = myShader::shaderSource; 
myShader::shaderSource sd = {
    std::pair<msd::shaderType,std::string>(msd::vertexShader,"cube.vert"),
    std::pair<msd::shaderType,std::string>(msd::fragmentShader,"cube.frag"),
};

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

void complieShader()
{
    sd.shadersCompileProgramLink();
    renProgram = *(static_cast<GLuint *>(sd.data()));
    dumpLog(sd.errLogInfoData(),sd.errLogTagData())();
}

void setVertices(void)
{
    glGenVertexArrays(1,vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVbos,vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertexPosition),vertexPosition,GL_STATIC_DRAW);
}

void init(void) {
    complieShader();
    glUseProgram(renProgram);
    cameraX = 0.4f,cameraY = 0.0f,cameraZ = 8.0f;
    cubeLocx = 0.4f,cubeLocy = -2.0f,cubeLocz = 1.0f;
    setVertices();
}


void display(GLFWwindow* _window,double _curTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renProgram);
    mvLoc = glGetUniformLocation(renProgram,"mv_matrix");
    proLoc = glGetUniformLocation(renProgram,"proj_matrix");
    glfwGetFramebufferSize(_window,&width,&height);
    aspect = (float)width / (float)height;
    tMat = glm::translate(glm::mat4(1.0f),glm::vec3(sin(0.35f*_curTime),cos(0.52f*_curTime),sin(0.7f*_curTime)));
    rMat = glm::rotate(glm::mat4(1.0f),(float)(1.75f*_curTime),glm::vec3(0.0f,1.0f,0.0f));
    rMat = glm::rotate(rMat,1.75f*(float)(_curTime),glm::vec3(1.0f,0.0f,0.0f));
    rMat = glm::rotate(rMat,1.75f*(float)(_curTime),glm::vec3(0.0f,0.0f,1.0f));
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f); // 60 degress
    //mMat = glm::translate(glm::mat4(1.0f),glm::vec3(cubeLocx,cubeLocy,cubeLocz));
    mMat = tMat * rMat;
    vMat = glm::translate(glm::mat4(1.0f),glm::vec3(-cameraX,-cameraY,-cameraZ));
    mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvMat));
    glUniformMatrix4fv(proLoc,1,GL_FALSE,glm::value_ptr(pMat));
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES,0,36);
}

int main()
{
    std::cout << "hello world" << std::endl;  
    if(!glfwInit()) {
        std::cout << "error" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow* win = glfwCreateWindow(600,600,"chapter4 -program1",NULL,NULL);
    glfwMakeContextCurrent(win);
    if(glewInit() != GLEW_OK) {
        std::cout << "error" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);
    init();
    while (!glfwWindowShouldClose(win))
    {
        display(win,glfwGetTime());
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwDestroyWindow(win);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}