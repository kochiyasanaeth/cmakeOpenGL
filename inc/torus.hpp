#include<cmath>
#include<vector>
#include<iostream>
#include<glm.hpp>
namespace model{
    class Torus {
    public:    
        Torus(float innerRadius,float outerRadius,int prec);
        int getNumVertices();
        int getNumIndices();
        std::vector<int> getIndices();
        std::vector<glm::vec3> getVertices();
        std::vector<glm::vec2> getTexCoords();
        std::vector<glm::vec3> getNormals();
        std::vector<glm::vec3> getStangents();
        std::vector<glm::vec3> getTtangents();
        float toRaidians(float degrees);
    private:
        int numVertices;
        int numIndices;
        int prec;
        float inner;
        float outer;
        std::vector<int> indices;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normal;
        std::vector<glm::vec3> sTangents;
        std::vector<glm::vec3> tTangents;
        void init();    
    };
}