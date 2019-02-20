#include "Input.h"
#include <string>

#include "vec.h"
bool Input::m_keyHeld[512] = {};
bool Input::m_newKeyState[512] = {};
vec2 Input::oldMousePos = {};
vec2 Input::mousePos = {};

bool Input::IsKeyPressed(int key){
  return m_newKeyState[key];
}

void Input::KeyDown(int key){
  m_newKeyState[key] = true;
  m_keyHeld[key] = true;
}

void Input::KeyUp(int key){
    m_newKeyState[key] = false;
    m_keyHeld[key] = false;
}

bool Input::IsKeyHeld(int key){
  return m_keyHeld[key];
}

void Input::Update(){
  std::memset(m_newKeyState, 0, 512);
}

void Input::SetMousePos(double x, double y){
  oldMousePos = mousePos;
  mousePos = {float(x), float(y)};
}

vec2 Input::GetMouseDelta(){
  return mousePos - oldMousePos;
}
