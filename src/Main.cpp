#include "Grid.h"
#include "Camera.h"

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
  glEnable(GL_CULL_FACE);
  glCullFace(GL_CW);

  Camera camera;
  camera.Init(vec3(0.0f, 2.0f, - 3.0f), vec3(0.0f, 0.0f, 1.0f));
  camera.SetProjection(45.0f, float(WINDOW_WIDTH) / WINDOW_HEIGHT);

  Grid grid;
  grid.Init();

  bool wireframe = false;

  while(!glfwWindowShouldClose(window)){
    
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 view = mat4::lookAt(vec3(0.0f, 1.0f, -4.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));

    if(wireframe)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    camera.Update();
    grid.Render(camera);
    
    if(Input::IsKeyPressed(GLFW_KEY_ESCAPE))
      break;

    const float speed = 0.1f;
    vec3 position = camera.GetPosition();
    if(Input::IsKeyHeld(GLFW_KEY_UP))
      position.z -= speed;
    else if(Input::IsKeyHeld(GLFW_KEY_DOWN))
      position.z +=speed;
    if(Input::IsKeyHeld(GLFW_KEY_LEFT))
      position.x -= speed;
    else if(Input::IsKeyHeld(GLFW_KEY_RIGHT))
      position.x +=speed;
    if(Input::IsKeyHeld(GLFW_KEY_A))
      position.y -= speed;
    else if(Input::IsKeyHeld(GLFW_KEY_Q))
      position.y +=speed;
    if(Input::IsKeyPressed(GLFW_KEY_F2))
      wireframe = !wireframe;
    if(Input::IsKeyHeld(GLFW_KEY_P))
      camera.IncreaseRotation(vec3(0.0f, 0.1f, 0.0f));
    else if(Input::IsKeyHeld(GLFW_KEY_O))
      camera.IncreaseRotation(vec3(0.0f, -0.1f, 0.0f));


    
    camera.SetPosition(position);

    glfwSwapBuffers(window);
    Input::Update();  
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}
