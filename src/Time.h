#ifndef TIME_H
#define TIME_H

class Time{
  static double m_elapsedTime;
  static double m_frameTime;
 public:
  static void IncreaseElapsedTime(double delta);
  static double GetElapsedTime();
  static double GetDeltaTime();
};

#endif
