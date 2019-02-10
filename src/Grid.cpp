#include "Grid.h"
#include <GL/glew.h>
#include <vector>

Grid::Grid(){
  m_numIndices = 0;
}

void Grid::InitBuffers(){

  std::vector<VertexData> vertices;
  std::vector<GLuint> indices;

  vertices.reserve((m_numVertices+1) * (m_numVertices+1));
  indices.reserve(m_numVertices * m_numVertices * 6);

  float halfDims = m_dims / 2.0f;
  for(unsigned int j = 0; j <= m_numVertices; ++j){
    for(unsigned int i = 0; i <= m_numVertices; ++i){
      float x = (float(i) / m_numVertices) * 2.0f - 1.0f;
      float z = (float(j) / m_numVertices) * 2.0f - 1.0f;
      vertices.push_back(VertexData(vec3(x * halfDims, 0.0f, z * halfDims)));
    }
  }

  for(unsigned int i = 0; i < m_numVertices; ++i){
    for(unsigned int j = 0; j < m_numVertices; ++j){
      int i0 = i * (m_numVertices+1) + j;
      int i1 = i0 + 1;
      int i2 = i0 + m_numVertices + 1;
      int i3 = i2 + 1;
      if((j+i) % 2){
	indices.push_back(i0);indices.push_back(i2);indices.push_back(i1);      
	indices.push_back(i1);indices.push_back(i2);indices.push_back(i3);     
      }else{
	indices.push_back(i0);indices.push_back(i2);indices.push_back(i3);      
	indices.push_back(i0);indices.push_back(i3);indices.push_back(i1);      	
      }
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  m_numIndices = indices.size();
}

void Grid::Init(unsigned int dims){
  m_dims = dims;
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

Grid::~Grid(){
  
}
