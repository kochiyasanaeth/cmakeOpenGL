#include<cmath>
#include<vector>
#include<iostream>
#include<glm.hpp>
#include<string>
#include<sstream>
#include<fstream>
#include"loadmodel.hpp"
namespace model{
    class modelImporter {
        public:
           modelImporter() = default;
           void parseObj(std::string filePath);
           int getNumVertices();
           std::vector<float> getNormals();
           std::vector<float> getVertices();
           std::vector<float> getTextureCoordinates();
        private:
           std::vector<float> vertVals;
           std::vector<float> stVals;
           std::vector<float> normVals;
           std::vector<float> triagleVerts;
           std::vector<float> textureCoords;
           std::vector<float> normals;
    };
    
    void modelImporter::parseObj(std::string filePath) {
        float x,y,z;
        std::string content;
        std::ifstream filestream(filePath,std::ios::in);
        std::string line;
        while(!filestream.eof()) {
            std::getline(filestream,line);
            if(line.compare(0,2,"v ") == 0) {
                std::cout << __LINE__ <<"_"<< vertVals.size() << std::endl;
                std::stringstream ss(line.erase(0,1));
                ss >> x;
                ss >> y;
                ss >> z;
                vertVals.push_back(x);
                vertVals.push_back(y);
                vertVals.push_back(z);
            }
            std::cout << line << __LINE__ <<"_"<< line.compare(0,2,"vt") << std::endl;
            if(line.compare(0,2,"vt") == 0) {
                std::cout << __LINE__ <<"_"<< stVals.size() << std::endl;
                std::stringstream ss(line.erase(0,2));
                ss >> x;
                ss >> y;
                stVals.push_back(x);
                stVals.push_back(y);
            }
            if(line.compare(0,2,"vn") == 0) {
                std::cout << __LINE__ <<"_"<< normals.size() << std::endl;
                std::stringstream ss(line.erase(0,2));
                ss >> x;
                ss >> y;
                ss >> z;
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
            if(line.compare(0,2,"f ") == 0) {
                std::cout << __LINE__ << line << std::endl;
                std::string oneCorner,v,t,n;
                std::stringstream ss(line.erase(0,2));
                for(int i = 0; i < 3;i++) {
                    getline(ss,oneCorner,' ');
                    std::stringstream oneCornerSS(oneCorner);
                    std::getline(oneCornerSS,v,'/');
                    std::getline(oneCornerSS,t,'/');
                    std::getline(oneCornerSS,n,'/');
                    int vertRef = (stoi(v) - 1) * 3;
                    int tcRef = (stoi(t) - 1) * 2;
                    int normalRef = (stoi(n) - 1) * 3;
                    triagleVerts.push_back(vertVals[vertRef]);
                    triagleVerts.push_back(vertVals[vertRef + 1]);
                    triagleVerts.push_back(vertVals[vertRef + 2]);
                    textureCoords.push_back(stVals[tcRef]);
                    textureCoords.push_back(stVals[tcRef] + 1);
                    normals.push_back(normals[normalRef]);
                    normals.push_back(normals[normalRef + 1]);
                    normals.push_back(normals[normalRef + 2]);
                }

            }
        }
    }
    int modelImporter::getNumVertices() {
        return static_cast<int>(this->triagleVerts.size() / 3);
    }
    std::vector<float> modelImporter::getNormals() {
        return this->normals;
    }
    std::vector<float> modelImporter::getVertices() {
        return this->triagleVerts;
    }
    std::vector<float> modelImporter::getTextureCoordinates() {
        return this->textureCoords;
    }
    
    loadModel::loadModel(std::string path) {
        modelImporter _mi;
        _mi.parseObj(path);
        numVertices = _mi.getNumVertices();
        std::vector<float> verts = _mi.getVertices();
        std::vector<float> tcs = _mi.getTextureCoordinates();
        std::vector<float> normals = _mi.getNormals();
        for(int i = 0;i < this->numVertices;i++) {
            std::cout << __LINE__ << "numVertices" << this->numVertices <<"elesize"<< verts.size() << std::endl;
            vertices.push_back(glm::vec3(verts[i * 3],verts[i * 3 + 1],verts[i*3 + 2]));
            std::cout << __LINE__ << "numVertices" << this->numVertices <<"elesize"<< tcs.size() << std::endl;
            texCoords.push_back(glm::vec2(tcs[i * 2],tcs[i * 2 + 1]));
            std::cout << __LINE__ << "numVertices" << this->numVertices <<"elesize"<< normals.size() << std::endl;
            normalVecs.push_back(glm::vec3(normals[i * 3],normals[i * 3 + 1],normals[i*3 + 2]));
        }
    }
    int loadModel::getNumVertices() {
        return this->numVertices;
    }
    std::vector<glm::vec3> loadModel::getVertices() {
        return this->vertices;
    }
    std::vector<glm::vec2> loadModel::getTextureCoords() {
        return this->texCoords;
    }
    std::vector<glm::vec3> loadModel::getNormals() {
        return this->normalVecs;
    }
}
