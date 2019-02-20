#include "Grid.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "mat.h"
#include "Time.h"

#include <GL/glew.h>
#include <vector>
#include <fstream>


Grid::Grid(){
  m_numIndices = 0;
}

float Grid::GetHeight(int i, int j){
  if(i < 0 || j < 0) return 0.0f;
  return (m_heightMap[j * m_terrainWidth +  i] / 255.0f) * heightScale;
}

float barryCentric(vec3 p1, vec3 p2, vec3 p3, vec2 pos) {
  float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
  float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
  float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
  float l3 = 1.0f - l1 - l2;
  return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

float Grid::GetHeight(float x, float z){
  float gridSize = m_halfDims / (m_numVertices - 1);
  int xIndex = int(x / gridSize);
  int zIndex = int(z / gridSize);

  if(xIndex < 0 || zIndex < 0 || xIndex >= m_numVertices-1 || zIndex >= m_numVertices-1 ) return 5.0f;
  float y = m_height[zIndex * m_numVertices + xIndex];

  return y;
}

void Grid::InitBuffers(){
  std::vector<VertexData> vertices;
  std::vector<GLuint> indices;
  std::vector<vec3> grassPosition;

  m_halfDims = m_terrainWidth * gridScale * 2.0f;
  m_height.resize(m_numVertices * m_numVertices);
  int k = 0;


  float sampleRadius = 2.0f;
  float numGrid = m_halfDims / sampleRadius;
  
  std::vector<bool> map;
  int iNumGrid = int(numGrid) + 1;
  map.resize(iNumGrid * iNumGrid);

  for(unsigned int j = 0; j < m_numVertices; ++j){
    for(unsigned int i = 0; i < m_numVertices; ++i){
      //generate local uv from 0 to 1
      vec2 texCoord;
      texCoord.x = float(i / float(m_numVertices - 1));
      texCoord.y = float(j / float(m_numVertices - 1));

      //generate coordinate 
      float x = texCoord.x;
      float z = texCoord.y;

      vec3 pos = vec3(x* m_halfDims, 0.0f, z * m_halfDims);
      int xIndex = k % m_numVertices;
      int yIndex = int(k / m_numVertices);
      pos.y = GetHeight(xIndex, yIndex);

      //sample grid
      int gridX = int(pos.x / sampleRadius);
      int gridY = int(pos.z / sampleRadius);
      float r = rand() / float(RAND_MAX);
      
      if(!map[gridY * iNumGrid + gridX] && r > 0.4f){
	map[gridY * iNumGrid + gridX] = true;
	if(r > 0.42f)
	  grassPos.push_back(pos);
	else if(r < 0.42)
	  treePos.push_back(pos);
      }

      m_height[j * m_numVertices + i] = pos.y;
      vertices.push_back(VertexData(pos, vec3(0.0f, 1.0f, 0.0), texCoord));

      //increase the counter by the offset or pixel channel
      k += m_offset;
    }
  }
  std::cout << "Tree" << treePos.size() << std::endl;
  //Generate Indices for Grid
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

  //Fill buffer for the grid
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);
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

  modelShader.InitShader("../Shaders/model.vert", "../Shaders/model.frag");
  shader.InitShader("../Shaders/mesh.vert", "../Shaders/mesh.frag");
  grassShader.InitShader("../Shaders/grassM.vert", "../Shaders/grass.frag");

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
      splatMap = TextureLoader::LoadTexture(text.c_str());      
    else if(prefix == "grassTexture")
      grassTexture = TextureLoader::LoadTexture(text.c_str());
    else if(prefix == "dirtTexture")
      dirtTexture = TextureLoader::LoadTexture(text.c_str());
    else if(prefix == "rockTexture")
      rockTexture = TextureLoader::LoadTexture(text.c_str());
    else if(prefix == "heightScale")
      heightScale = std::stof(text);
    else if(prefix == "grassPackTexture"){
      grassPackTexture = TextureLoader::LoadTexture(text.c_str());
    }
    else if(prefix == "gridScale")
      gridScale = std::stof(text);

  }

  fernTexture = TextureLoader::LoadTexture("../Assets/Models/props/fern.png");
  flowerTexture = TextureLoader::LoadTexture("../Assets/Models/props/flower.png");
  groundGrassTexture = TextureLoader::LoadTexture("../Assets/Models/props/grass.png");
  
  //Initialize buffer
  m_numVertices = 256;
  InitBuffers();

  //model
  //treeModel.SetInstances(treePos);
  treeModel.LoadModel("../Assets/Models/tree/pine.obj");

  grassModel.SetInstances(grassPos);
  grassModel.LoadModel("../Assets/Models/props/grass.obj");

  //Shader uniform initialization
  shader.Use();
  shader.LoadInt("splatmap", 0);
  shader.LoadInt("grassTexture", 1);
  shader.LoadInt("dirtTexture", 2);
  shader.LoadInt("rockTexture", 3);
  shader.Unuse();

  grassShader.Use();
  grassShader.LoadInt("grass", 0);
  grassShader.LoadInt("fern", 1);
  grassShader.LoadInt("flower", 2);
  grassShader.Unuse();
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

void Grid::Render(Camera& camera){

  shader.Use();
  shader.LoadMat("projection", camera.GetProjection());
  shader.LoadMat("model", mat4());
  shader.LoadMat("view", camera.GetViewMatrix());
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
  
  grassShader.Use();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  grassShader.LoadMat("projection", camera.GetProjection());
  grassShader.LoadMat("model", mat4::scale(vec3(0.5f)));
  grassShader.LoadMat("view", camera.GetViewMatrix());
  grassShader.LoadFloat("time", float(Time::GetElapsedTime()));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, groundGrassTexture);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fernTexture);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, flowerTexture);

  grassModel.Draw(grassShader);
  glBindTexture(GL_TEXTURE_2D, 0);

  grassShader.Unuse();

  modelShader.Use();
  modelShader.LoadMat("projection", camera.GetProjection());
  modelShader.LoadMat("view", camera.GetViewMatrix());
  
  mat4 translate = mat4::translate(vec3(0.0f, 0.0f, 0.0f));
  modelShader.LoadMat("model", mat4::scale(vec3(1.0f)));
  treeModel.Draw(modelShader);
  modelShader.Unuse();

  glDisable(GL_BLEND);
}


void Grid::LoadHeightMap(const char* filename){
  TextureData textureData = TextureLoader::LoadHeightMap(filename);
  m_heightMap = textureData.image;
  m_terrainWidth = textureData.width;
  m_terrainHeight = textureData.height;
  m_offset = textureData.numChannel;
}

Grid::~Grid(){
  delete[] m_heightMap;
}
