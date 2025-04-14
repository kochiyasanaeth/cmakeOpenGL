#include<cmath>
#include<vector>
#include<iostream>
#include<glm.hpp>
#include"sphere.hpp"
namespace model{
    Sphere::Sphere(int prec){
        this->init(prec);
    }
    //constexpr double pi = acos(-1.0);
    float Sphere::toRadians(float degrees) {
        return (degrees * 2.0f * 3.1415926535) / 360.0f;
    }
    void Sphere::init(int prec) {
        this->numVertices = (prec + 1) * (prec + 1);
        this->numIdices = prec * prec * 6;
        for(int i = 0;i < numVertices;i++){
            vertics.push_back(glm::vec3());
        }
        for(int i = 0;i < numVertices;i++){
            texcroods.push_back(glm::vec3());
        }
        for(int i = 0;i < numVertices;i++){
            normals.push_back(glm::vec3());
        }
        for(int i = 0;i < numIdices;i++){
            indices.push_back(0);
        }
        for(int i =0;i <= prec;i++) {
            for(int j = 0;j <= prec;j++) {
                float y = (float)cos(this->toRadians(180.0f - i * 180.0f /prec));
                float x= -(float)cos(this->toRadians(j *360.0f/prec)) * (float)abs(cos(asin(y)));
                float z = (float)sin(this->toRadians(j *360.0f /prec)) * (float)abs(cos(asin(y)));
                vertics[(i*(prec+1)) + j] = glm::vec3(x,y,z);
                texcroods[(i*(prec+1)) + j] = glm::vec2((float) j / prec,((float)i / prec));
                normals[i*(prec + 1) + j] = glm::vec3(x,y,z);
            }
            for(int i = 0;i <prec; i++)
            {
                for(int j = 0; j < prec; j++) {
                    indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
                    indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
                    indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
                    indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
                    indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
                    indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
                }
            }
        }                                                                                                                                                                                                
    }
    int Sphere::getnumVertices() {
        return this->numVertices;
    };
    int Sphere::getnumIdices() {
        return this->numIdices;
    };
    std::vector<int> Sphere::getIndieces() {
        return this->indices;
    }
    std::vector<glm::vec3> Sphere::getVertics() {
        return this->vertics;
    }
    std::vector<glm::vec2> Sphere::getTexCoords() {
        return this->texcroods;
    }
    std::vector<glm::vec3> Sphere::getNormals() {
        return this->normals;
    }
}