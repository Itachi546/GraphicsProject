#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"
#include "mat.h"
#include "Input.h"
#include <GLFW/glfw3.h>

class Camera{
 private:
  vec3 look;
  vec3 up;
  vec3 position;
  vec3 rotation;
  mat4 projection;
  mat4 view;
  const float SPEED = 0.1f;
 public:
  Camera(){
    position = vec3(0.0f, 5.0f, -1.0f);
    look = vec3(0.0f, 0.0f, 1.0f);
    up = vec3(0, 1, 0);
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

  void Camera::Strafe(float val){
    vec3 right = vec3::cross(look, up);
    position += right * val * SPEED;
    Update();
  }

  void Camera::Lift(float val){
    position += val * SPEED * up;
    Update();
  }

  void Camera::Walk(float val){
    position += val * SPEED * look;
    Update();
  }

  void Update(){
    mat4 rotate = mat4::rotate(rotation);
    vec4 u = rotate * vec4(vec3(0, 1, 0), 0.0f);
    up = vec3::normalize(vec3(u.x, u.y, u.z));
    vec4 f = rotate * vec4(0, 0, 1, 0);
    look = vec3::normalize(vec3(f.x, f.y, f.z));
    vec3 target = position + look;
    view = mat4::lookAt(position, target, up);
  }

  mat4 GetViewMatrix(){
    return view;
  }
  
};



#endif
