#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

struct vec2;

class Input{
 private:
  static bool m_keyHeld[512];
  static bool m_newKeyState[512];
  static vec2 oldMousePos;
  static vec2 mousePos;
 public:
  static bool IsKeyPressed(int key);
  static void KeyDown(int key);
  static void KeyUp(int key);
  static bool IsKeyHeld(int key);
  static void Update();
  static void SetMousePos(double x, double y);
  static vec2 GetMouseDelta();
};

#endif
