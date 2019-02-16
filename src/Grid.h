#ifndef TERRAIN_H
#define TERRAIN_H

#include "Shader.h"
#include "Math/vec.h"

struct mat4;
struct vec3;

class Shader;

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
  void Render(mat4& view, vec3& translate); 

  unsigned int m_vao;
  unsigned int m_numIndices;

 private:
  void InitBuffers();
  void LoadHeightMap(const char* filename);
  float GetHeight(int i, int j);

  int m_terrainWidth;
  int m_terrainHeight;
  float heightScale, gridScale;
  Shader shader;
  unsigned int m_offset;
  unsigned int m_vbo, m_ebo;
  unsigned int m_numVertices;
  unsigned char* m_heightMap;

  unsigned int splatMap, grassTexture, dirtTexture, rockTexture;
};

#endif
