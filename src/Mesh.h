#pragma once
#include <GL/glew.h>
#include <iostream>
#include "Shader.h"
#include "vec.h"
#include <vector>

struct Vertex {
	
  vec3 position;
  vec3 normal;
  vec2 texCoord;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string name;
};

const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;
class Mesh {

private:

  GLuint VBO, VAO, EBO, InstancedVBO;
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;
  std::vector<vec3> instanceData;
  GLuint programId;
	
	void SetupVAO();

public:
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures, std::vector<vec3> instanceData);
	void Draw(Shader& shader);
	~Mesh();
};
