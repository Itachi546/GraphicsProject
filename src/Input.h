#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

class Input{
 private:
  static bool m_keyHeld[512];
  static bool m_newKeyState[512];

 public:
  static bool IsKeyPressed(int key);
  static void KeyDown(int key);
  static void KeyUp(int key);
  static bool IsKeyHeld(int key);
  static void Update();
};

#endif
