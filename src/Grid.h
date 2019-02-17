#ifndef TERRAIN_H
#define TERRAIN_H

#include "Shader.h"
#include "Math/vec.h"

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
  //  float GetHeight(float x, float z);
  unsigned int m_vao;
  unsigned int m_grassVAO;
  unsigned int m_numIndices;

 private:
  void InitBuffers();
  void LoadHeightMap(const char* filename);


  //Heightmap data
  int m_terrainWidth;
  int m_terrainHeight;
  float heightScale, gridScale;
  Shader shader, grassShader;
  unsigned int m_offset;

  //grass data
  unsigned int m_vbo, m_ebo;
  unsigned int m_grassVBO, m_grassEBO, m_grassPBO;
  unsigned int m_numGrassVertices, m_numGrass;
  unsigned int m_numVertices;
  unsigned char* m_heightMap;

  unsigned int grassPackTexture;
  unsigned int splatMap, grassTexture, dirtTexture, rockTexture;
};

#endif
