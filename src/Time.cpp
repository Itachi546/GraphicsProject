#include "Time.h"

double Time::m_elapsedTime = 0.0;

void Time::IncreaseElapsedTime(double delta){
    m_elapsedTime += delta;
  }

double Time::GetElapsedTime(){
    return m_elapsedTime;
}
