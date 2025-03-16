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
#include <stack>

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

static float tetrahedroPosition[] = {
    -1.0f,-1.0f, 1.0f,  1.0f,-1.0, 1.0f,  0.0f, 1.0f, 0.0f,//00
     1.0f,-1.0f, 1.0f,  1.0f,-1.0,-1.0f,  0.0f, 1.0f, 0.0f,//01
     1.0f,-1.0f,-1.0f, -1.0f,-1.0,-1.0f,  0.0f, 1.0f, 0.0f,//02
    -1.0f,-1.0f,-1.0f, -1.0f,-1.0, 1.0f,  0.0f,-1.0f, 0.0f,//03
    -1.0f,-1.0f,-1.0f,  1.0f,-1.0, 1.0f, -1.0f,-1.0f, 1.0f,//04
     1.0f,-1.0f, 1.0f, -1.0f,-1.0,-1.0f,  1.0f,-1.0f,-1.0f //05
};

GLuint mvLoc,proLoc,tfLoc;
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
    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(tetrahedroPosition),tetrahedroPosition,GL_STATIC_DRAW);
}

void init(GLFWwindow* _win) {
    complieShader();
    glUseProgram(renProgram);
    cameraX = 0.0f,cameraY = 0.0f,cameraZ = 12.0f;
    cubeLocx = 0.0f,cubeLocy = 0.0f,cubeLocz = 1.0f;
    glfwGetFramebufferSize(_win,&width,&height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f); // 60 degress
    setVertices();
}


void display(GLFWwindow* _window,double _curTime) {
    GLint __tetraLocX = 1,__tetraLocY = 1,__tetraLocZ = 1;
    std::stack<glm::mat4> matSt;
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renProgram);
    mvLoc = glGetUniformLocation(renProgram,"mv_matrix");
    proLoc = glGetUniformLocation(renProgram,"proj_matrix");


    vMat = glm::translate(glm::mat4(1.0f),glm::vec3(-cameraX,-cameraY,-cameraZ));
    matSt.push(vMat);
    matSt.top() *= glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
    matSt.push(matSt.top());
    matSt.top() *= glm::rotate(glm::mat4(1.0f),(float)_curTime,glm::vec3(0.0,1.0,0.0));
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(matSt.top()));
    glUniformMatrix4fv(proLoc,1,GL_FALSE,glm::value_ptr(pMat));
    matSt.pop();
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glDrawArrays(GL_TRIANGLES,0,36);
    
    matSt.push(matSt.top());
    matSt.top() *= glm::translate(glm::mat4(1.0f),glm::vec3(sin((float)_curTime) * 7.0,0.0f,cos((float)_curTime * 7.0)));
    matSt.push(matSt.top());
    matSt.top() *= glm::rotate(glm::mat4(1.0f),(float)_curTime,glm::vec3(1.0,0.0,0.0));
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(matSt.top()));
    matSt.pop();

    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glFrontFace(GL_CCW);
    glDrawArrays(GL_TRIANGLES,0,18);

    matSt.push(matSt.top());
    matSt.top() *= glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,sin((float)_curTime) * 3.0,cos((float)_curTime * 3.0)));
    matSt.top() *= glm::rotate(glm::mat4(1.0f),(float)_curTime,glm::vec3(0.0,0.0,1.0)) * glm::scale(glm::mat4(1.0f),\
    glm::vec3(0.25f,0.25f,0.25f));
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(matSt.top()));
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glFrontFace(GL_CW);
    glDrawArrays(GL_TRIANGLES,0,36);

}

void winReshapeCb(GLFWwindow* _win,int _nwidth,int _nheight) {
    aspect = (float)_nwidth / (float)_nheight;
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f); // 60 degress
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
    GLFWwindow* win = glfwCreateWindow(600,600,"chapter4 -program4",NULL,NULL);
    glfwMakeContextCurrent(win);
    if(glewInit() != GLEW_OK) {
        std::cout << "error" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
    init(win);
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