#include<cmath>
#include<vector>
#include<iostream>
#include<glm.hpp>
namespace model{


class loadModel{
public:
   loadModel(std::string path);
   int getNumVertices();
   std::vector<glm::vec3> getVertices();
   std::vector<glm::vec2> getTextureCoords();
   std::vector<glm::vec3> getNormals();
private:
   int numVertices;
   std::vector<glm::vec3> vertices;
   std::vector<glm::vec2> texCoords;
   std::vector<glm::vec3> normalVecs;
};
}