#include<cmath>
#include<vector>
#include<iostream>
#include<glm.hpp>
namespace mymaterial{
   class materialFactory {
   public:
      virtual float *Ambient();
      virtual float *Diffuse();
      virtual float *Specular();
      virtual float Shininess();
      virtual ~materialFactory() = default;
   };
   class  glodmaterial : public materialFactory{
   public:
      float *Ambient() override {
         static float ambient[4] = {0.2473f,0.1995f,0.0745f,1};
         return (float *)ambient;
      }
      float *Diffuse() override {
         static float diffuse[4] = {0.7516f,0.6065f,0.2255f,1};
         return (float *)diffuse;
      }
      float *Specular() override {
         static float shininess[4] = {0.6283f,0.5559f,0.3661f,1};
         return (float *)shininess;
      }
      float Shininess() override {
         return 51.2f;
      }
   };
}