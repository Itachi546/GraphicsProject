#include "Time.h"
#include <iostream>

double Time::m_elapsedTime = 0.0;
double Time::m_frameTime = 0.0;

void Time::IncreaseElapsedTime(double delta){
  m_frameTime = delta;
  m_elapsedTime += delta;
}

double Time::GetElapsedTime(){
    return m_elapsedTime;
}

double Time::GetDeltaTime(){
  return m_elapsedTime;
}
