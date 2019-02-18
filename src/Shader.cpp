#include "Shader.h"
#include "Math/mat.h"
#include "Math/vec.h"

#include <fstream>
#include <iostream>

#define DEBUG_LOG(val)  std::cout << val << std::endl;

void Shader::InitShader(std::string vertexshader, std::string fragmentshader){

  GLuint vshaderId = glCreateShader(GL_VERTEX_SHADER);
  std::string source = LoadFromFile(vertexshader);
  const char * vtmp = source.c_str();
  glShaderSource(vshaderId, 1, &vtmp, NULL);

  glCompileShader(vshaderId);
  if(!CheckStatus(vshaderId, vertexshader)){
    DEBUG_LOG("Error Compiling vertex shader");
  }

  GLuint fshaderId = glCreateShader(GL_FRAGMENT_SHADER);
  source = LoadFromFile(fragmentshader);
  const char* ftemp = source.c_str();
  glShaderSource(fshaderId, 1, &ftemp, NULL);

  
  glCompileShader(fshaderId);
  if(!CheckStatus(fshaderId, fragmentshader)){
    DEBUG_LOG("Error Compiling fragment shader");
  }

  m_programId = glCreateProgram();
  glAttachShader(m_programId, vshaderId);
  glAttachShader(m_programId, fshaderId);

  glLinkProgram(m_programId);

  GLint status;
  glGetProgramiv(m_programId, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLog = new GLchar[infoLogLength];
    glGetProgramInfoLog(m_programId, infoLogLength, NULL, infoLog);
    DEBUG_LOG(std::string(infoLog));
    delete[] infoLog;
  }

  glDeleteShader(vshaderId);
  glDeleteShader(fshaderId);

}

std::string Shader::LoadFromFile(std::string filename){
  std::string buffer;
  std::ifstream infile(filename);
  if(!infile){
    DEBUG_LOG(filename + std::string(" Failed to Load ShaderFile "));
  }
  else{
    std::string line;
    while(std::getline(infile, line)){
      buffer.append(line);
      buffer.append("\r\n");
    }
  }
    return buffer;
}


bool Shader::CheckStatus(GLuint id, std::string filename){
  GLint status;
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if(status == GL_FALSE){
    GLint length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    GLchar* infoLog = new GLchar[length];
    glGetShaderInfoLog(id, length, NULL, infoLog);
    DEBUG_LOG(std::string(infoLog));
    delete[] infoLog;
    return false;
  }

  return true;
}


void Shader::Use(){
  glUseProgram(m_programId);
}

void Shader::Unuse(){
  glUseProgram(0);
}

void Shader::LoadInt(std::string location, int val){
  int loc = glGetUniformLocation(m_programId, location.c_str());
  if(loc >= 0)
    glUniform1i(loc, val);
}

void Shader::LoadFloat(std::string location, float val){
  int loc = glGetUniformLocation(m_programId, location.c_str());
  if(loc >= 0)
    glUniform1f(loc, val);
}

void Shader::LoadBool(std::string location, bool val){
  int loc = glGetUniformLocation(m_programId, location.c_str());
  if(loc >= 0)
    glUniform1i(loc, int(val));
}

void Shader::LoadVec(std::string location, vec2 val){
  int loc = glGetUniformLocation(m_programId, location.c_str());
  if(loc >= 0)
    glUniform2fv(loc, 1,  &val[0]);
}

void Shader::LoadVec(std::string location, vec3 val){
  int loc = glGetUniformLocation(m_programId, location.c_str());
  if(loc >= 0)
    glUniform3fv(loc, 1,  &val[0]);
  
}

void Shader::LoadVec(std::string location, vec4 val){
  int loc = glGetUniformLocation(m_programId, location.c_str());
    if(loc >= 0)
    glUniform4fv(loc, 1,  &val[0]);

}

void Shader::LoadMat(std::string location, mat4 val){
  int loc = glGetUniformLocation(m_programId, location.c_str());
    if(loc >= 0)
      glUniformMatrix4fv(loc, 1, GL_FALSE,  &val[0][0]);

}

/*
void Shader::LoadMat(std::string location, mat3 val){
  int loc = glGetUniformLocation(m_programId, location.c_str());
    if(loc >= 0)
      glUniformMatrix3fv(loc, 1, GL_FALSE,  &val[0][0]);
  
}
*/

void Shader::Destroy(){
  glDeleteProgram(m_programId);
}
