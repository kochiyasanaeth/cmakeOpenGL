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
#include "stb_image.hpp"


float cameraX,cameraY,cameraZ;
float cubeLocx,cubeLocy,cubeLocz;
constexpr int numVaos = 1,numVbos = 2;
GLuint vao[numVaos];
GLuint vbo[numVbos];
int width,height;
float aspect;
glm::mat4 pMat,mMat,vMat,mvMat,tMat,rMat;
GLuint renProgram;
GLuint woodTextire;

void loadTexture2D(GLuint &texid,const char *path) {
    int width,height,channels;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texid);
    unsigned char *data = stbi_load(path,&width,&height,&channels,STBI_rgb_alpha);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "load fail" << std::endl;
    }
    stbi_image_free(data);
}


static float tetrahedroPosition[] = {
    -1.0f,-1.0f, 1.0f,  1.0f,-1.0, 1.0f,  0.0f, 1.0f, 0.0f,//00
     1.0f,-1.0f, 1.0f,  1.0f,-1.0,-1.0f,  0.0f, 1.0f, 0.0f,//01
     1.0f,-1.0f,-1.0f, -1.0f,-1.0,-1.0f,  0.0f, 1.0f, 0.0f,//02
    -1.0f,-1.0f,-1.0f, -1.0f,-1.0, 1.0f,  0.0f,-1.0f, 0.0f,//03
    -1.0f,-1.0f,-1.0f,  1.0f,-1.0, 1.0f, -1.0f,-1.0f, 1.0f,//04
     1.0f,-1.0f, 1.0f, -1.0f,-1.0,-1.0f,  1.0f,-1.0f,-1.0f //05
};

static float tetrahedroTex[] = {
    0.0f,0.0f,1.0f, 0.0f,0.5f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.5f,1.0f,
    0.0f,0.0f,1.0f, 0.0f,0.5f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.5f,1.0f,
    0.0f,0.0f,1.0f, 1.0f,0.5f,1.0f, 1.0f,1.0f,0.0f, 0.0f,1.0f,0.0f,
};

GLuint mvLoc,proLoc,tfLoc;
using msd = myShader::shaderSource; 
myShader::shaderSource sd = {
    std::pair<msd::shaderType,std::string>(msd::vertexShader,"tex.vert"),
    std::pair<msd::shaderType,std::string>(msd::fragmentShader,"tex.frag"),
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
    glBufferData(GL_ARRAY_BUFFER,sizeof(tetrahedroPosition),tetrahedroPosition,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(tetrahedroTex),tetrahedroTex,GL_STATIC_DRAW);
}

void init() {
    complieShader();
    glUseProgram(renProgram);
    cameraX = 0.4f,cameraY = 0.0f,cameraZ = 8.0f;
    cubeLocx = 0.4f,cubeLocy = -2.0f,cubeLocz = 1.0f;
    setVertices();
    glGenTextures(1,&woodTextire);
    loadTexture2D(woodTextire,"TexWood.jpg");
}


void display(GLFWwindow* _window,double _curTime) {
    GLint __tetraLocX = 1,__tetraLocY = 2,__tetraLocZ = 1;
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renProgram);
    mvLoc = glGetUniformLocation(renProgram,"mv_matrix");
    proLoc = glGetUniformLocation(renProgram,"proj_matrix");
    glfwGetFramebufferSize(_window,&width,&height);
    aspect = (float)width / (float)height;
    rMat = glm::rotate(glm::mat4(1.0f),(0.60f),glm::vec3(0.0f,1.0f,0.0f));
    rMat = glm::rotate(rMat,-0.20f,glm::vec3(1.0f,0.0f,0.0f));
    rMat = glm::rotate(rMat,0.00f,glm::vec3(0.0f,0.0f,1.0f));
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f); // 60 degress
    glUniformMatrix4fv(proLoc,1,GL_FALSE,glm::value_ptr(pMat));
    vMat = glm::translate(glm::mat4(1.0f),glm::vec3(-cameraX,-cameraY,-cameraZ));
    mMat = glm::translate(glm::mat4(1.0f),glm::vec3(__tetraLocX,__tetraLocY,__tetraLocZ)) * rMat;
    mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvMat));
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTextureEXT(GL_TEXTURE_2D,woodTextire);
    glBindTexture(GL_TEXTURE_2D,woodTextire);
    if(glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat anisoSetting = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&anisoSetting);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY,anisoSetting);
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES,0,18);
}
void winReshaepCb(GLFWwindow* _win,int newWidth,int newHeight) {
    aspect = (float)newWidth / (float)height;
    glViewport(0,0,newHeight,newWidth);
    std::cout << newHeight << ' ' << newWidth << std::endl;
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
    GLFWwindow* win = glfwCreateWindow(600,600,"chapter5 -program1",NULL,NULL);
    int scrHeight,scrWeight;
    glfwGetFramebufferSize(win,&scrHeight,&scrWeight);
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