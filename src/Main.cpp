#include "Grid.h"
#include "Camera.h"
#include "Time.h"
#include "Model.h"

#include "Math/vec.h"
#include "Math/mat.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
  

int main(){

  
  if(!glfwInit()) glfwInit();
  GLFWwindow* window = glfwCreateWindow(g_windowWidth, g_windowHeight, "Graphics", 0, 0);
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mousebutton_callback);
  glfwSetWindowSizeCallback(window, windowsize_callback);
  glfwSwapInterval(1);

  if(!glewInit()) glewInit();
  glClearColor(0.5f, 0.6f, 1.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  // glCullFace(GL_CW);

  Camera camera;
  camera.Init(vec3(0.0f, 50.0f, - 3.0f), vec3(0.0f, 0.0f, 1.0f));

  Grid grid;
  grid.Init();

  bool wireframe = false;
  
  while(!glfwWindowShouldClose(window)){

    double startTime = glfwGetTime();
    glViewport(0, 0, g_windowWidth, g_windowHeight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    camera.SetProjection(45.0f, float(g_windowWidth) / g_windowHeight);

    grid.Render(camera);


    if(Input::IsKeyPressed(GLFW_KEY_ESCAPE))
      break;

    if(Input::IsKeyHeld(GLFW_KEY_UP))
      camera.Walk(2.0f);
    else if(Input::IsKeyHeld(GLFW_KEY_DOWN))
      camera.Walk(-2.0f);
    if(Input::IsKeyHeld(GLFW_KEY_LEFT))
      camera.Strafe(-2.0f);
    else if(Input::IsKeyHeld(GLFW_KEY_RIGHT))
      camera.Strafe(2.0f);

    if(Input::IsKeyHeld(GLFW_KEY_A))
      camera.Lift(-2.0f);
    else if(Input::IsKeyHeld(GLFW_KEY_Q))
      camera.Lift(2.0f);

    if(Input::IsKeyPressed(GLFW_KEY_F2))
      wireframe = !wireframe;

    if(Input::IsKeyHeld(GLFW_KEY_P))
      camera.IncreaseRotation(vec3(0.0f, -0.5f, 0.0f));
    else if(Input::IsKeyHeld(GLFW_KEY_O))
      camera.IncreaseRotation(vec3(0.0f, 0.5f, 0.0f));
    if(wireframe)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Time::IncreaseElapsedTime(glfwGetTime() - startTime);
    glfwSwapBuffers(window);
    Input::Update();  
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}
