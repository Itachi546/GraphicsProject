#include "Grid.h"
#include <GL/glew.h>
#include <vector>
#include <fstream>
#include "Texture.h"
#include "Shader.h"
#include "mat.h"

Grid::Grid(){
  m_numIndices = 0;
}

float Grid::GetHeight(int i, int j){
  if(i < 0 || j < 0) return 0.0f;
  return (m_heightMap[j * m_terrainWidth +  i] / 255.0f) * heightScale;
}

void Grid::InitBuffers(){
  std::vector<VertexData> vertices;
  std::vector<GLuint> indices;
  float halfDims = m_terrainWidth * gridScale;
  int k = 0;
  for(unsigned int j = 0; j < m_numVertices; ++j){
    for(unsigned int i = 0; i < m_numVertices; ++i){
      //generate local uv from 0 to 1
      vec2 texCoord;
      texCoord.x = float(i / float(m_numVertices - 1));
      texCoord.y = float(j / float(m_numVertices - 1));

      //generate coordinate 
      float x = texCoord.x * 2.0f - 1.0f;
      float z = texCoord.y * 2.0f - 1.0f;
      int xIndex = k % m_numVertices;
      int yIndex = int(k / m_numVertices);
      float y = GetHeight(xIndex, yIndex);

      vertices.push_back(VertexData(vec3(x * halfDims, y, z * halfDims), vec3(0.0f, 1.0f, 0.0), texCoord));

      //increase the counter by the offset or pixel channel
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

      //Generate crude normal by taking cross product of two edges of triangle

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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)offsetof(VertexData, normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)offsetof(VertexData, texCoord));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  m_numIndices = indices.size();
}

void Grid::Init(){
  shader.InitShader("../Shaders/mesh.vert", "../Shaders/mesh.frag");


  //Load config file
  std::ifstream infile("../Assets/terrain.txt");
  if(!infile){
    std::cerr << "Failed to load config file" << std::endl;
    return;
  }

  std::string buffer;
  while(std::getline(infile, buffer)){
    int firstSpace = buffer.find_first_of(' ');
    std::string prefix = buffer.substr(0, firstSpace);
    std::string text = buffer.substr(firstSpace+1, buffer.size());

    if(prefix == "heightmap")
      LoadHeightMap(text.c_str());
    else if(prefix == "splatmap")
      splatMap = Texture::LoadTexture(text.c_str());      
    else if(prefix == "grassTexture")
      grassTexture = Texture::LoadTexture(text.c_str());
    else if(prefix == "dirtTexture")
      dirtTexture = Texture::LoadTexture(text.c_str());
    else if(prefix == "rockTexture")
      rockTexture = Texture::LoadTexture(text.c_str());
    else if(prefix == "heightScale")
      heightScale = std::stof(text);
    else if(prefix == "gridScale")
      gridScale = std::stof(text);
  }

  //Initialize buffer
  m_numVertices = 256;
  InitBuffers();


  //Shader uniform initialization
  shader.Use();
  mat4 projection = mat4::perspective(45.0f, 4.0f/3.0f, 0.1f, 1000.0f);
  shader.LoadMat("projection", projection);
  shader.LoadInt("splatmap", 0);
  shader.LoadInt("grassTexture", 1);
  shader.LoadInt("dirtTexture", 2);
  shader.LoadInt("rockTexture", 3);
  shader.Unuse();


}

void Grid::Destroy(){
  shader.Destroy();
  glDeleteTextures(1, &splatMap);
  glDeleteTextures(1, &grassTexture);
  glDeleteTextures(1, &dirtTexture);
  glDeleteTextures(1, &rockTexture);

  glDeleteVertexArrays(1, &m_vao);
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
}

void Grid::Render(mat4& view, vec3& translate){
  shader.Use();  
  mat4 model = mat4::translate(translate);
  shader.LoadMat("model", model);
  shader.LoadMat("view", view);
  glBindVertexArray(m_vao);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, splatMap);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, grassTexture);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, dirtTexture);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, rockTexture);

  glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  shader.Unuse();
}


void Grid::LoadHeightMap(const char* filename){
  TextureData textureData = Texture::LoadHeightMap(filename);
  m_heightMap = textureData.image;
  m_terrainWidth = textureData.width;
  m_terrainHeight = textureData.height;
  m_offset = textureData.numChannel;
}

Grid::~Grid(){
  delete[] m_heightMap;
}
