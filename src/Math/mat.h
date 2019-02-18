#ifndef MATH_MAT_H
#define MATH_MAT_H

#include "vec.h"

#define PI 3.1415926f
#define DEG2RAD (PI / 180.0f)

/*
  represented as transpose of standard matrix representation

  m00 m10 m20 m30
  m01 m11 m21 m31
  m02 m12 m22 m32
  m03 m12 m23 m33
 */

struct mat4{
  //column wise or element wise 
  union{
    float m[4][4]; //not used so often

    struct{
      vec4 col[4];
    };
  };

  //initialize to identity matrix
  mat4(){
    for(int i = 0; i < 4; ++i){
      for(int j = 0; j < 4; ++j){
	m[i][j] = i == j ? 1.0f : 0.0f;
      }
    }
  }

  //matrix multiplication
  mat4 operator*(mat4 rhs){
    mat4 res;

    for(int i = 0; i < 4; ++i)
      for(int j = 0; j < 4; ++j)
	res[j][i] = m[0][i] * rhs.m[j][0] + m[1][i] * rhs.m[j][1] + m[2][i] * rhs.m[j][2] + m[3][i] * rhs.m[j][3];

    return res;
  }

  vec4 operator*(vec4 rhs){
    vec4 res;
    for(int i = 0; i < 4; ++i){
      res[i] = m[i][0] * rhs.x + m[i][1] * rhs.y + m[i][2] * rhs.z + m[i][3] * rhs.w;
    }
    return res;
  }
  
  //indexing operator overloading for index operation
  vec4& operator[](const std::size_t index){
    assert(index >= 0 && index < 4);
    return col[index];
  }

  const vec4& operator[](const std::size_t index) const{
    assert(index >= 0 && index < 4);
    return col[index];
  }

  //perspective projection matrix
  static mat4 perspective(float fov, float aspect, float near, float far){
    fov = fov / 2.0f;
    float s = 1.0f / tan(fov * DEG2RAD);
    float invDenom = 1.0f / (near - far);

    mat4 m;
    m[0][0] = s / aspect;
    m[1][1] = s;

    m[2][2] = (near * far) * invDenom;
    m[2][3] = -1.0f;

    m[3][2] = (2.0f * near * far) * invDenom;
    m[3][3] = 0.0f;

    return m;
  }

  //Translation matrix
  static mat4 translate(vec3 pos){
    mat4 m;
    m[3][0] = pos.x;
    m[3][1] = pos.y;
    m[3][2] = pos.z;
    return m;
  }

  //rotation along z-axis
  static mat4 rotateZ(float angle){
    angle = angle * DEG2RAD;
    float cosA = cos(angle);
    float sinA = sin(angle);

    mat4 m;
    m[0][0] = cosA;
    m[0][1] = sinA;

    m[1][0] = -sinA;
    m[1][1] = cosA;

    return m;
  }

  //rotation along y-axis
  static mat4 rotateY(float angle){
    angle = angle * DEG2RAD;
    float cosA = cos(angle);
    float sinA = sin(angle);

    mat4 m;
    m[0][0] = cosA;
    m[0][2] = sinA;

    m[2][0] = -sinA;
    m[2][2] = cosA;

    return m;
  }

  //rotation along x-axis
  static mat4 rotateX(float angle){
    angle = angle * DEG2RAD;
    float cosA = cos(angle);
    float sinA = sin(angle);

    mat4 m;
    m[1][1] = cosA;
    m[1][2] = sinA;

    m[2][1] = -sinA;
    m[2][2] = cosA;

    return m;
  }

  static mat4 rotate(vec3 angle){
    mat4 x = rotateX(angle.x);
    mat4 y = rotateY(angle.y);
    mat4 z = rotateZ(angle.z);

    return y * z * x;
  }

  //scale
  static mat4 scale(vec3 val){
    mat4 m;
    m[0][0] = val.x;
    m[1][1] = val.y;
    m[2][2] = val.z;

    return m;
  }

  //view transform with respect to camera
  static mat4 lookAt(vec3 eye, vec3 target, vec3 up){
    vec3 f = vec3::normalize(eye - target);
    vec3 r = vec3::normalize(vec3::cross(up, f));
    vec3 u = vec3::normalize(vec3::cross(f, r));

    mat4 res;
    res[0] = vec4(r.x, u.x, f.x, 0.0f);
    res[1] = vec4(r.y, u.y, f.y, 0.0f);
    res[2] = vec4(r.z, u.z, f.z, 0.0f);
    res[3] = vec4(-vec3::dot(eye, r), -vec3::dot(eye, u), -vec3::dot(eye, f), 1.0f);
    return res;
  }
  
  //Debugging
  friend std::ostream& operator<<(std::ostream& os, mat4 mat){
    for(int i = 0; i < 4; ++i){
      for(int j = 0; j < 4; ++j)
	os << mat.m[i][j] << " ";
      std::cout << '\n';
    }
    return os;
  }
};
#endif
