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
#include "scene.hpp"
#include "torus.hpp"
#include "material.hpp"
float cameraX,cameraY,cameraZ;
float cubeLocx,cubeLocy,cubeLocz;
constexpr int numVaos = 1,numVbos = 4;
GLuint vao[numVaos];
GLuint vbo[numVbos];
int width = 600,height = 600;
float aspect;
glm::mat4 pMat,mMat,vMat,mvMat,tMat,rMat,invTrMat;
GLuint renProgram;
GLuint woodTextire;
GLuint globalAmbLoc,ambLoc,diffLoc,specLoc,posLoc,mAmLoc,mDiffLoc,mSpecLoc,mShiLoc;
glm::vec3 currentLightPos,lightPosV;
glm::vec3 initiallLightLoc = glm::vec3(5.0f,2.0f,2.0f);
float globalAmbient[4] = {0.7f,0.7f,0.7f,1.0f};
float lightAmbient[4] = {0.0f,0.0f,0.0f,1.0f};
float lightDiffuse[4] = {1.0f,1.0f,1.0f,1.0f};
float lightSpecular[4] = {1.0f,1.0f,1.0f,1.0f};
mymaterial::glodmaterial materialgold;
float *matAmb = materialgold.Ambient();
float *matDif = materialgold.Diffuse();
float *matSpe = materialgold.Specular();
float matShi = materialgold.Shininess();
float lightPos[3];


void installLights(glm::mat4 vMatrix) {
    lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos,1.0));
    //glm::vec4(currentLightPos,1.0);
    lightPos[0] = lightPosV.x;
    lightPos[1] = lightPosV.y;
    lightPos[2] = lightPosV.z;

    globalAmbLoc = glGetUniformLocation(renProgram,"globalAmbient");
    ambLoc = glGetUniformLocation(renProgram,"light.ambient");
    diffLoc = glGetUniformLocation(renProgram,"light.diffuse");
    specLoc = glGetUniformLocation(renProgram,"light.specular");
    posLoc = glGetUniformLocation(renProgram,"light.position");
    mAmLoc = glGetUniformLocation(renProgram,"material.ambient");
    mDiffLoc = glGetUniformLocation(renProgram,"material.diffuse");
    mSpecLoc = glGetUniformLocation(renProgram,"material.specular");
    mShiLoc = glGetUniformLocation(renProgram,"material.shininess");
    
    glProgramUniform4fv(renProgram,globalAmbLoc,1,globalAmbient);
    glProgramUniform4fv(renProgram,ambLoc,1,lightAmbient);
    glProgramUniform4fv(renProgram,diffLoc,1,lightDiffuse);
    glProgramUniform4fv(renProgram,specLoc,1,lightSpecular);
    glProgramUniform4fv(renProgram,posLoc,1,lightPos);
    glProgramUniform4fv(renProgram,mAmLoc,1,matAmb);
    glProgramUniform4fv(renProgram,mDiffLoc,1,matDif);
    glProgramUniform4fv(renProgram,mSpecLoc,1,matSpe);
    glProgramUniform1f(renProgram,mShiLoc,matShi);

}

void loadTexture2D(GLuint &texid,const char *path) {
    int width,height,channels;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texid);
    unsigned char *data = stbi_load(path,&width,&height,&channels,STBI_rgb_alpha);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "load success" << std::endl;
    }
    else
    {
        std::cout << "load fail" << std::endl;
    }
    stbi_image_free(data);
}

GLuint mvLoc,proLoc,tfLoc,nLoc;
using msd = myShader::shaderSource; 
myShader::shaderSource sd = {
    std::pair<msd::shaderType,std::string>(msd::vertexShader,"gouraud.vert"),
    std::pair<msd::shaderType,std::string>(msd::fragmentShader,"gouraud.frag"),
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
model::Torus _ts(0.5f,0.2f,48);

void setVertices(void)
{
    
    std::vector<int> ind = _ts.getIndices();
    std::vector<glm::vec3> vert = _ts.getVertices();
    std::vector<glm::vec2> tex = _ts.getTexCoords();
    std::vector<glm::vec3> norm = _ts.getNormals();
    int numVertices = _ts.getNumVertices();
    std::cout << __LINE__ << "numIndices:" << numVertices << std::endl;
    std::vector<float> pvalues;
    std::vector<float> tvalues;
    std::vector<float> nvalues;
    for(int i = 0;i < numVertices;i++) {
        pvalues.push_back(vert[i].x);
        pvalues.push_back(vert[i].y);
        pvalues.push_back(vert[i].z);
        tvalues.push_back(tex[i].s);
        tvalues.push_back(tex[i].t);
        nvalues.push_back(norm[i].x);
        nvalues.push_back(norm[i].y);
        nvalues.push_back(norm[i].z);
    }
    //std::cout << "nvalues.size():" << nvalues.size() << ',' << "pvalues.size():" << pvalues.size() << std::endl;
    for(size_t i = 0;i < 10;i++) {
            std::cout << "normal[i].x" <<nvalues[i]  << std::endl;
    }
    glGenVertexArrays(1,vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(4,vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,pvalues.size() * 4,&pvalues[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,tvalues.size() * 4,&tvalues[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
    glBufferData(GL_ARRAY_BUFFER,nvalues.size() * 4,&nvalues[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,ind.size() * 4,&ind[0],GL_STATIC_DRAW);
}

void init() {
    complieShader();
    glUseProgram(renProgram);
    cameraX = 0.4f,cameraY = 0.0f,cameraZ = 8.0f;
    cubeLocx = 0.4f,cubeLocy = -2.0f,cubeLocz = 1.0f;
    std::cout << __LINE__ << "renProgram:" << renProgram << std::endl;
    setVertices();

    std::cout << __LINE__ << "renProgram:" << renProgram << std::endl;

    glGenTextures(1,&woodTextire);
    loadTexture2D(woodTextire,"TexWood.jpg");

}

void display(void) {
    GLint __tetraLocX = 0,__tetraLocY = 0,__tetraLocZ = 2;
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renProgram);
    mvLoc = glGetUniformLocation(renProgram,"mv_matrix");
    proLoc = glGetUniformLocation(renProgram,"proj_matrix");
    nLoc = glGetUniformLocation(renProgram,"norm_matrix");   
    aspect = (float)width / (float)height;
    rMat = glm::rotate(glm::mat4(1.0f),(0.00f),glm::vec3(0.0f,1.0f,0.0f));
    rMat = glm::rotate(rMat,-0.00f,glm::vec3(1.0f,0.0f,0.0f));
    rMat = glm::rotate(rMat,0.00f,glm::vec3(0.0f,0.0f,1.0f));
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f); // 60 degress
    vMat = glm::translate(glm::mat4(1.0f),glm::vec3(-cameraX,-cameraY,-cameraZ));
    // mMat = glm::translate(glm::mat4(1.0f),glm::vec3(__tetraLocX,__tetraLocY,__tetraLocZ)) * rMat;
    mMat = glm::translate(glm::mat4(1.0f),glm::vec3(__tetraLocX,__tetraLocY,__tetraLocZ));
    mMat *= glm::rotate(mMat,_ts.toRaidians(25.0f),glm::vec3(1.0f,0.0f,0.0f));
    currentLightPos = glm::vec3(initiallLightLoc.x,initiallLightLoc.y,initiallLightLoc.z);
    installLights(vMat);
    mvMat = vMat * mMat;
    invTrMat = glm::transpose(glm::inverse(mvMat));
    glUniformMatrix4fv(proLoc,1,GL_FALSE,glm::value_ptr(pMat));
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvMat));
    glUniformMatrix4fv(nLoc,1,GL_FALSE,glm::value_ptr(invTrMat));
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,woodTextire);
    if(glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat anisoSetting = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&anisoSetting);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY,anisoSetting);
    }
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo[3]);
    glDrawElements(GL_TRIANGLES,_ts.getNumIndices(),GL_UNSIGNED_INT,0);
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
    myScene::scene sc("chapter7 -program1",1080,1080);
    sc.createW();
    sc.setDisplayFunction(display);
    sc.check();
    glfwSwapInterval(1);
    init();
    while (!sc.shouldCloseW())
    {
        sc.sceneDisplay();
    }
    exit(EXIT_SUCCESS);
    return 0;
}