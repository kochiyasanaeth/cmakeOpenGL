#include<cmath>
#include<vector>
#include<iostream>
#include<glm.hpp>
#include<gtc/type_ptr.hpp>
#include<gtc/matrix_transform.hpp>
#include"torus.hpp"
namespace model{
    Torus::Torus(float innerRadius,float outerRadius,int precIn): inner(innerRadius),outer(outerRadius),prec(precIn){
        this->init();
        std::cout << "hello" << __LINE__ << "prec:" << prec << std::endl;

    }
    float Torus::toRaidians(float degrees) {
        return (degrees * 2.0f * 3.1415926f) / 360.0f;
    }
    void Torus::init() {
        numVertices = (prec + 1) * (prec + 1);
        numIndices = prec * prec * 6;
        for (int i = 0;i < numVertices; i++) {vertices.push_back(glm::vec3());}
        for (int i = 0;i < numVertices; i++) {texCoords.push_back(glm::vec2());}
        for (int i = 0;i < numVertices; i++) {normal.push_back(glm::vec3());}
        for (int i = 0;i < numVertices; i++) {sTangents.push_back(glm::vec3());}
        for (int i = 0;i < numVertices; i++) {tTangents.push_back(glm::vec3());}
        for (int i = 0;i < numIndices; i++) {indices.push_back(0);}
        for(int i = 0;i < prec + 1;i++) {
            float amt  = toRaidians(i * 360.0f / prec);
            glm::mat4 rMat = glm::rotate(glm::mat4(1.0f),amt,glm::vec3(0.0f,0.0f,1.0f));
            glm::vec3 initPos(rMat * glm::vec4(outer,0.0f,0.0f,1.0f));
            vertices[i] = glm::vec3(initPos + glm::vec3(inner,0.0f,0.0f));
            texCoords[i] = glm::vec2(0.0f,(float) i / (float) prec);
            rMat = glm::rotate(glm::mat4(1.0f),amt,glm::vec3(0.0f,0.0f,1.0f));
            tTangents[i] = glm::vec3(rMat * glm::vec4(0.0,-1.0f,0.0f,1.0f));
            sTangents[i] = glm::vec3(glm::vec3(0.0f,0.0f,-1.0f));
            normal[i] = glm::cross(tTangents[i],sTangents[i]);
        }
        for(int ring = 1;ring < prec + 1;ring++) {
            for(int vert = 0;vert < prec + 1;vert++) {
                float amt = (float) (toRaidians(ring * 360.0f / float(prec)));
                glm::mat4 rMat = glm::rotate(glm::mat4(1.0f),amt,glm::vec3(0.0f,1.0f,0.0f));
                vertices[ring*(prec + 1) + vert] = glm::vec3(rMat * glm::vec4(vertices[vert],1.0f));

                texCoords[ring*(prec + 1) + vert] = glm::vec2((float)ring * 2.0f/(float)prec,texCoords[vert].t);
                if(texCoords[ring*(prec + 1) + vert].s > 1.0) {

                    texCoords[ring*(prec + 1) + vert].s -= -1.0f;
                }
                rMat = glm::rotate(glm::mat4(1.0f),amt,glm::vec3(0.0f,1.0f,0.0f));
                sTangents[ring * (prec + 1) + vert] = glm::vec4(rMat * glm::vec4(sTangents[vert],1.0f));
                rMat = glm::rotate(glm::mat4(1.0f),amt,glm::vec3(0.0f,1.0f,0.0f));
                tTangents[ring * (prec + 1) + vert] = glm::vec4(rMat * glm::vec4(tTangents[vert],1.0f));
                rMat = glm::rotate(glm::mat4(1.0f),amt,glm::vec3(0.0f,1.0f,0.0f));
                normal[ring * (prec + 1) + vert] = glm::vec4(rMat * glm::vec4(normal[vert],1.0f));
            }
        }
            for(int ring = 0;ring < prec;ring++) {
                for(int vert = 0;vert < prec; vert++) {
                    indices[((ring * prec + vert) * 2) * 3 + 0] = ring * (prec + 1) + vert;
                    indices[((ring * prec + vert) * 2) * 3 + 1] = (ring + 1) * (prec + 1) + vert;
                    indices[((ring * prec + vert) * 2) * 3 + 2] = ring * (prec + 1) + vert + 1;
                    indices[((ring * prec + vert) * 2 + 1) * 3 + 0] = ring * (prec + 1) + vert + 1;
                    indices[((ring * prec + vert) * 2 + 1) * 3 + 1] = (ring + 1) * (prec + 1) + vert;
                    indices[((ring * prec + vert) * 2 + 1) * 3 + 2] = (ring + 1) * (prec + 1) + vert + 1;
                }
            }
    }
    int Torus::getNumVertices() {
        return this->numVertices;
    };
    int Torus::getNumIndices() {
        return this->numIndices;
    };
    std::vector<int> Torus::getIndices() {
        return this->indices;
    }
    std::vector<glm::vec3> Torus::getVertices() {
        return this->vertices;
    }
    std::vector<glm::vec2> Torus::getTexCoords() {
        return this->texCoords;
    }
    std::vector<glm::vec3> Torus::getNormals() {
        return this->normal;
    }
}