#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

struct mat4;
struct vec4;
struct vec3;
struct vec2;

class Shader{

 private:
  GLuint m_programId;
  
  std::string LoadFromFile(const char* filename);
  bool CheckStatus(GLuint id, std::string filename);
public:
  Shader(){}
  ~Shader(){}

  void InitShader(const char* vertexshader, const char* fragmentshader);
  void Use();
  void Unuse();

  void LoadInt(std::string location, int val);
  void LoadFloat(std::string location, float val);
  void LoadBool(std::string location, bool val);
  void LoadVec(std::string location, vec2 val);
  void LoadVec(std::string location, vec3 val);
  void LoadVec(std::string location, vec4 val);
  void LoadMat(std::string location, mat4 val);
  //  void LoadMat(std::string location, mat3 val);
  void Shader::Destroy();
};

#endif
