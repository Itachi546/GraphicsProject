#include "Shader.h"
#include "Grid.h"
#include "Input.h"

#include "Math/vec.h"
#include "Math/mat.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod){
  if(action == GLFW_PRESS)
     Input::KeyDown(key);
  else if(action == GLFW_RELEASE)
    Input::KeyUp(key);
}

void mousebutton_callback(GLFWwindow* window, int key, int action, int mod){
 
  if(action == GLFW_PRESS)
    Input::KeyDown(key);
  else if(action == GLFW_RELEASE)
    Input::KeyUp(key);
}
  

int main(){

  if(!glfwInit()) glfwInit();
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Graphics", 0, 0);
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mousebutton_callback);

  if(!glewInit()) glewInit();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);

  Shader shader;
  shader.InitShader("../Shaders/mesh.vert", "../Shaders/mesh.frag");
  Grid grid;
  grid.Init();

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  mat4 projection = mat4::perspective(45.0f, 4.0f/3.0f, 0.1f, 1000.0f);
  mat4 view = mat4::lookAt(vec3(0.0f, 1.0f, -4.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));

  while(!glfwWindowShouldClose(window)){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    if(Input::IsKeyPressed(GLFW_KEY_SPACE)){
      std::cout << "Pressed" << std::endl;
    }

    float angle = float(glfwGetTime()) * 40.0f;
    mat4 model = mat4::rotateY(angle);
    shader.Use();
    shader.LoadMat("projection", projection);
    shader.LoadMat("model", model);
    shader.LoadMat("view", view);
    grid.Render();
    shader.Unuse();
    glfwSwapBuffers(window);
    Input::Update();  
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}
