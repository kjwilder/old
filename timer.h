#ifndef TIMER_H
#define TIMER_H

#include <ctime>

// __________________________________________________________________________

class timer
{
 private:
  int running;
  double accumulated_time;
  clock_t start_time;

  timer(const timer& t);
  void operator=(const timer& t);

 public:
  timer() : running(0), accumulated_time(0) { }
  ~timer() { }

  void start(const char* timer_name = 0);
  void restart(const char* timer_name = 0);
  void stop(const char* timer_name = 0);
  double elapsed_time();
  double check(const char* timer_name = 0);

}; // class timer

// __________________________________________________________________________
// timer.h

#endif // TIMER_H
