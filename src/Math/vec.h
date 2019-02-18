#ifndef MATH_VEC_H
#define MATH_VEC_H

#include <cmath>
#include <cassert>
#include <iostream>

#define EPSILON  0.000001

struct vec2{
  union{
    struct{
    float x;
    float y;
    };
    float v[2];
  };

  vec2(float val){
    x = val;
    y = val;
  }

  vec2(){
    x = 0.0f;
    y = 0.0f;
  }

  vec2(float x, float y){
    this->x = x;
    this->y = y;
  }

  //Basic vector operation
  vec2 operator+(vec2 v){
    return vec2(x + v.x, y + v.y);
  }
  
  vec2 operator-(vec2 v){
    return vec2(x - v.x, y - v.y);
  }

  vec2 operator*(float v){
    return vec2(x * v, y * v);
  }

  vec2 operator/(float v){
    //@Note maybe need to check for divide by zero
      return vec2(x / v, y / v);
  }

  vec2 operator+=(vec2 v){
    x += v.x;
    y += v.y;
    return *this;
  }
  
  vec2 operator-=(vec2 v){
    x -= v.x;
    y -= v.y;
    return *this;
  }

  vec2 operator*=(float v){
    x *= v;
    y *= v;
    return *this;
  }

  vec2 operator/=(float v){
    //@Note maybe need to check for divide by zero
    x /= v;
    y /= v;
    return *this;
  }

  //vector indexing
  float& operator[](const std::size_t index){
    assert(index >=0 && index < 2);
    return v[index];
  }
  const float& operator[](const std::size_t index) const{
    assert(index >=0 && index < 2);
    return v[index];
  }
  
  //square of magnitude
  static float lengthSquared(vec2 v){
    return v.x * v.x + v.y * v.y;
  }

  //magnitude of the vector
  static float length(vec2 v){
    return sqrt(v.x * v.x + v.y * v.y);
  }

  //normalize to unit vector
  static vec2 normalize(vec2 v){
    //@Note maybe need to check for divide by zero
    float len = length(v);
    return vec2(v.x / len, v.y/len);
  }
  
  //dot product of vector v1 and v2
  static float dot(vec2 v1, vec2 v2){
    return v1.x * v2.x + v1.y * v2.y;
  }

  //Debugging
  friend std::ostream& operator<<(std::ostream& os, vec2 v){
    os << v.x <<" " << v.y;
    return os;
  }
};

struct vec3{
  union{
    struct{
      float x;
      float y;
      float z;
    };
    float v[3];
  };
  vec3(float val){
    x = val;
    y = val;
    z = val;
  }

  vec3(){
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }

  vec3(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
  }

  //Basic vector operation
  vec3 operator+(vec3 v){
    return vec3(x + v.x, y + v.y, z + v.z);
  }
  
  vec3 operator-(vec3 v){
    return vec3(x - v.x, y - v.y, z - v.z);
  }

  vec3 operator*(float v){
    return vec3(x * v, y * v, z * v);
  }

  friend vec3 operator*(float val, vec3 v){
    return v * val;
  }
  vec3 operator/(float v){
    //@Note maybe need to check for divide by zero
    return vec3(x / v, y / v, z / v);
  }

  vec3 operator+=(vec3 v){
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  
  vec3 operator-=(vec3 v){
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  vec3 operator*=(float v){
    x *= v;
    y *= v;
    z *= v;
    return *this;
  }

  vec3 operator/=(float v){
    //@Note maybe need to check for divide by zero
    x /= v;
    y /= v;
    z /= v;
    return *this;
  }

  //vector accessing
  float& operator[](const std::size_t index){
    assert(index >=0 && index < 3);
    return v[index];
  }
  
  const float& operator[](const std::size_t index) const{
    assert(index >=0 && index < 3);
    return v[index];
  }
  
  //square of magnitude
  static float lengthSquared(vec3 v){
    return v.x * v.x + v.y * v.y + v.z * v.z;
  }

  //magnitude of the vector
  static float length(vec3 v){
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  }

  //normalize to unit vector
  static vec3 normalize(vec3 v){
    //@Note maybe need to check for divide by zero
    float len = length(v);
    return vec3(v.x / len, v.y/len, v.z/len);
  }
  
  //dot product of vector v1 and v2
  static float dot(vec3 v1, vec3 v2){
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }

  //cross prodcut of vector v1 and v2
  static vec3 cross(vec3 v1, vec3 v2){
    return vec3{
      v1.y * v2.z - v2.y * v1.z,
      v1.z * v2.x - v2.z * v1.x ,	
      v1.x * v2.y - v2.x * v1.y
	};
  }

  //Debugging
  friend std::ostream& operator<<(std::ostream& os, vec3 v){
    os << v.x <<" " << v.y <<" " << v.z;
    return os;
  }
};

struct vec4{
  union{
    struct{
      float x;
      float y;
      float z;
      float w;
    };
    float v[4];
  };
  vec4(){
    x = y = z = w = 0.0f;
  }

  vec4(float v){
    x = y = z = w = v;
  }

  vec4(float _x, float _y, float _z, float _w){
    x = _x;
    y = _y;
    z = _z;
    w = _w;
  }
  vec4(vec3 v, float _w){
    x = v.x;
    y = v.y;
    z = v.z;
    w = _w;
  }

  float& operator[](const std::size_t index){
    assert(index >= 0 && index < 4);
    return v[index];
  }

  const float& operator[](const std::size_t index) const {
    assert(index >= 0 && index < 4);
    return v[index];
  }

  friend std::ostream& operator<<(std::ostream& os, vec4 v){
    os << v.x <<" " << v.y << " " << v.z <<" " << v.w;
    return os;
  }

};


#endif
