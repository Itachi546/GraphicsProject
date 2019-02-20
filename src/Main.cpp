#include "Grid.h"
#include "Camera.h"
#include "Time.h"
#include "Model.h"

#include "Math/vec.h"
#include "Math/mat.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"

#include <iostream>

int g_windowWidth = 800;
int g_windowHeight = 600;

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

void windowsize_callback(GLFWwindow* window, int width, int height){
  g_windowWidth = width;
  g_windowHeight = height;
}

void mousepos_callback(GLFWwindow* window, double x, double y){
  Input::SetMousePos(x, y);
}
  

int main(){
  
  if(!glfwInit()) glfwInit();
  GLFWwindow* window = glfwCreateWindow(g_windowWidth, g_windowHeight, "Graphics", 0, 0);
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mousebutton_callback);
  glfwSetWindowSizeCallback(window, windowsize_callback);
  //glfwSetCursorPosCallback(window, mousepos_callback);
  
  glfwSwapInterval(1);

  if(!glewInit()) glewInit();
  glClearColor(0.5f, 0.6f, 1.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  // glCullFace(GL_CW);

  Camera camera;
  camera.Init(vec3(100.0f, 40.0f, 100.0f), vec3(0.0f, 0.0f, 1.0f));

  Grid grid;
  grid.Init();

  bool wireframe = false;
  vec2 mousePos = {};
  while(!glfwWindowShouldClose(window)){

    double startTime = glfwGetTime();
    glViewport(0, 0, g_windowWidth, g_windowHeight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    camera.SetProjection(45.0f, float(g_windowWidth) / g_windowHeight);
    grid.Render(camera);

    if(Input::IsKeyPressed(GLFW_KEY_ESCAPE))
      break;

    if(Input::IsKeyHeld(GLFW_KEY_UP) || Input::IsKeyHeld(GLFW_KEY_W))
      camera.Walk(2.0f);
    else if(Input::IsKeyHeld(GLFW_KEY_DOWN) || Input::IsKeyHeld(GLFW_KEY_S))
      camera.Walk(-2.0f);

    if(Input::IsKeyHeld(GLFW_KEY_LEFT) || Input::IsKeyHeld(GLFW_KEY_A))
      camera.Strafe(-2.0f);
    else if(Input::IsKeyHeld(GLFW_KEY_RIGHT) || Input::IsKeyHeld(GLFW_KEY_D))
      camera.Strafe(2.0f);

    if(Input::IsKeyHeld(GLFW_KEY_R))
      camera.Lift(2.0f);
    else if(Input::IsKeyHeld(GLFW_KEY_F))
      camera.Lift(-2.0f);

    if(Input::IsKeyPressed(GLFW_KEY_F2))
      wireframe = !wireframe;

    if(wireframe)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //mouse position difference between the frame
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    vec2 newPos = {float(x), float(y)};

    const float ROTATION_SPEED = 1.0f;
    vec2 delta = (newPos - mousePos) * ROTATION_SPEED;
    mousePos = newPos;

    camera.IncreaseRotation(vec3(-delta.y, -delta.x, 0.0f));
    
    Time::IncreaseElapsedTime(glfwGetTime() - startTime);
    glfwSwapBuffers(window);
    Input::Update();  
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}
