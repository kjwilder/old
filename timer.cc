#define TIMER_C

// __________________________________________________________________________
// Includes

#include <ctime>
#include <iostream>
#include <iomanip>

#include "timer.h"

// __________________________________________________________________________
// Determine how much time has passed since the timer was started.

double timer::elapsed_time() {
  clock_t curr_time = clock();
  return static_cast<double>(curr_time - start_time) / CLOCKS_PER_SEC;
}

// __________________________________________________________________________
// Start a timer.  Return accumulated time.

void timer::start(const char* timer_name) {
  if (running)  {  // Return immediately if the timer is already running
    return;
  }
  if (timer_name) {  // Report a timer message
    std::cout << "Timing [" << timer_name << "]...\n";
  }
  running = 1;
  start_time = clock();
}

// __________________________________________________________________________
// Check how much time has passed.

double timer::check(const char* timer_name) {
  if (timer_name) {  // Report the timer message
    std::cout << "Checking timer [" << timer_name << "]...\n";
  }
  if (running) {
    return accumulated_time + elapsed_time();
  }
  return accumulated_time;
}

// __________________________________________________________________________

void timer::restart(const char* timer_name) {
  if (timer_name) {  // Report the timer message
    std::cout << "Restarting [" << timer_name << "]...\n";
  }
  accumulated_time = 0;
  running = 1;
  start_time = clock();
}

// __________________________________________________________________________

void timer::stop(const char* timer_name) {
  if (running) {  // Recalculate the total accumulated time the timer has run
    accumulated_time += elapsed_time();
  }
  running = 0;
  if (timer_name) {
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::setprecision(3);
    std::cout << "Ending [" << timer_name << "] : "
         << accumulated_time << std::endl;
  }
}
