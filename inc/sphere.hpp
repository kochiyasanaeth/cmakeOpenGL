#include<cmath>
#include<vector>
#include<iostream>
#include<glm.hpp>
namespace model{
    class Sphere{
    public:
        Sphere (int prec);
        std::vector<glm::vec3> getVertics();
        std::vector<glm::vec2> getTexCoords();
        std::vector<glm::vec3> getNormals();
        std::vector<int> getIndieces();
        int getnumIdices();
        int getnumVertices();
        float toRadians(float degress);

    private: 
        int numVertices;
        int numIdices;
        std::vector<int> indices;
        std::vector<glm::vec3> vertics;
        std::vector<glm::vec2> texcroods;
        std::vector<glm::vec3> normals;
        void init(int prec);    
    };
}