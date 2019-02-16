#include "Grid.h"
#include <GL/glew.h>
#include <vector>
#include <fstream>
#include "Texture.h"

Grid::Grid(){
  m_numIndices = 0;
}

float Grid::GetHeight(int i, int j){
  if(i < 0 || j < 0) return 0.0f;
  return (m_heightMap[j * m_terrainWidth +  i] / 255.0f) * 130.0f;
}

void Grid::InitBuffers(){
  std::vector<VertexData> vertices;
  std::vector<GLuint> indices;
  float halfDims = m_terrainWidth;
  int k = 0;
  for(unsigned int j = 0; j < m_numVertices; ++j){
    for(unsigned int i = 0; i < m_numVertices; ++i){

      float x = (float(i) / (m_numVertices - 1)) * 2.0f - 1.0f;
      float z = (float(j) / (m_numVertices - 1)) * 2.0f - 1.0f;
      int xIndex = k % m_numVertices;
      int yIndex = int(k / m_numVertices);
      float y = GetHeight(xIndex, yIndex);
      vertices.push_back(VertexData(vec3(x * halfDims, y, z * halfDims), vec3(0.0f, 1.0f, 0.0)));
      k += m_offset;
    }
  }

  for(unsigned int i = 0; i < m_numVertices-1; ++i){
    for(unsigned int j = 0; j < m_numVertices-1; ++j){
      int i0 = i * (m_numVertices) + j;
      int i1 = i0 + 1;
      int i2 = i0 + m_numVertices;
      int i3 = i2 + 1;

      indices.push_back(i0);indices.push_back(i2);indices.push_back(i1);      
      indices.push_back(i1);indices.push_back(i2);indices.push_back(i3);     

      vec3 normal = vec3::cross(vertices[i2].position - vertices[i0].position, vertices[i1].position - vertices[i0].position);
      normal = vec3::normalize(normal);
      vertices[i0].normal = normal;
      vertices[i1].normal = normal;
      vertices[i2].normal = normal;
      normal = vec3::cross(vertices[i2].position - vertices[i1].position, vertices[i3].position - vertices[i1].position);
      normal = vec3::normalize(normal);
      vertices[i3].normal = normal;

    }
  }

  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);
  glGenVertexArrays(1, &m_vao);

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), &vertices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)sizeof(vec3));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  m_numIndices = indices.size();
}

void Grid::Init(){

  LoadHeightMap("../Assets/Textures/output.bmp");
  m_numVertices = 256;
  InitBuffers();
}

void Grid::Destroy(){
  glDeleteVertexArrays(1, &m_vao);
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
}

void Grid::Render(){
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}


void Grid::LoadHeightMap(const char* filename){
  TextureData textureData = Texture::LoadHeightMap(filename);
  m_heightMap = textureData.image;
  m_terrainWidth = textureData.width;
  m_terrainHeight = textureData.height;
  m_offset = textureData.numChannel;
  std::cout << textureData.width <<" " << textureData.height << std::endl;
  std::cout << textureData.numChannel << std::endl;
}

Grid::~Grid(){
  delete[] m_heightMap;
}
