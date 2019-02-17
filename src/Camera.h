#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"
#include "mat.h"
#include "Input.h"
#include <GLFW/glfw3.h>

class Camera{
 private:
  vec3 look;
  vec3 position;
  vec3 rotation;
  mat4 projection;
  mat4 view;
  mat4 rotate;
 public:
  Camera(){
    position = vec3(0.0f, 5.0f, -1.0f);
    look = vec3(0.0f, 0.0f, 1.0f);
    rotation = vec3(0.0f);
    Update();
  }

  void Init(vec3 position, vec3 look){
    this->position = position;
    this->look = look;
    Update();
  }

  void SetPosition(vec3 p){
    this->position = p;
    Update();
  }

  void IncreaseRotation(vec3 delta){
    rotation += delta;
    Update();
  }

  vec3 GetPosition(){
    return this->position;
  }

  void SetProjection(float fov, float aspect, float near = 0.1f, float far = 1000.0f){
    projection = mat4::perspective(fov, aspect, near, far);
  }

  mat4 GetProjection(){
    return projection;
  }

  void Update(){
    vec3 target = position - look;
    rotate = mat4::rotate(rotation);
    view = mat4::lookAt(position, target, vec3(0.0f, 1.0f, 0.0f));
  }

  mat4 GetViewMatrix(){
    return rotate * view;
  }
  
};



#endif
