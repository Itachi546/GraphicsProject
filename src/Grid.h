#ifndef TERRAIN_H
#define TERRAIN_H

#include "Shader.h"
#include "Math/vec.h"
#include "Model.h"

#include <vector>

struct mat4;
struct vec3;

class Shader;
class Camera;

struct VertexData{
  vec3 position;
  vec3 normal;
  vec2 texCoord;
  VertexData(){}
  VertexData(vec3 position, vec3 normal, vec2 texCoord){
    this->position = position;
    this->normal = normal;
    this->texCoord = texCoord;
  }
};

class Grid{

 public:
  ~Grid();
  Grid();
  void Init();
  void Destroy();
  void Render(Camera& camera);
  float GetHeight(int i, int j);
  float GetHeight(float x, float z);
  float fogDensity = 0.007f;
  float fogGradient = 1.5f;
  
 private:
  void InitBuffers();
  void LoadHeightMap(const char* filename);
  void LoadUniforms(Shader& shader, Camera& camera);

  unsigned int m_vao;
  unsigned int m_grassVAO;
  unsigned int m_numIndices;

  //Heightmap data
  int m_terrainWidth;
  int m_terrainHeight;
  float heightScale, gridScale;
  Shader shader, grassShader, modelShader;
  unsigned int m_offset;
  float m_halfDims;
  //grass data
  unsigned int m_vbo, m_ebo;
  unsigned int m_numVertices;
  unsigned char* m_heightMap;
  std::vector<float> m_height;
  unsigned int grassPackTexture;
  unsigned int splatMap, grassTexture, dirtTexture, rockTexture;
  unsigned int fernTexture, flowerTexture, groundGrassTexture;
  Model treeModel, grassModel;
  std::vector<vec3> treePos;
  std::vector<vec3> grassPos;
};

#endif
