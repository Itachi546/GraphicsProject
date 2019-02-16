#ifndef TERRAIN_H
#define TERRAIN_H

#include "Math/vec.h"

class Shader;

struct VertexData{
  vec3 position;
  vec3 normal;
  VertexData(){}
  VertexData(vec3 position, vec3 normal){
    this->position = position;
    this->normal = normal;
  }
};

class Grid{

 public:
  ~Grid();
  Grid();
  void Init();
  void Destroy();
  void Render(); 

  unsigned int m_vao;
  unsigned int m_numIndices;

 private:
  void InitBuffers();
  void LoadHeightMap(const char* filename);
  float GetHeight(int i, int j);

  int m_terrainWidth;
  int m_terrainHeight;

  unsigned int m_offset;
  unsigned int m_vbo, m_ebo;
  unsigned int m_numVertices;
  unsigned char* m_heightMap;
};

#endif
