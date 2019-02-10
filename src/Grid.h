#ifndef TERRAIN_H
#define TERRAIN_H

#include "Math/vec.h"

class Shader;

struct VertexData{
  vec3 position;
  VertexData(){}
  VertexData(vec3 position){
    this->position = position;
  }
};

class Grid{

 private:
  void InitBuffers();
 public:
  ~Grid();
  Grid();
  void Init(unsigned int dims = 1);
  void Destroy();
  void Render(); 

  unsigned int m_vao;
  unsigned int m_numIndices;

 private:
  unsigned int m_dims;
  unsigned int m_vbo, m_ebo;
  const unsigned int m_numVertices = 2;
};

#endif
