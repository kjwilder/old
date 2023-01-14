#define TIMER_C

// __________________________________________________________________________
// Includes

#include <ctime>
#include <iostream>
#include <iomanip>

#include "timer.h"

// __________________________________________________________________________
// Determine how much time has passed since the timer was started.

double timer::elapsed_time()
{
  clock_t curr_time = clock();
  return double(curr_time - start_time) / CLOCKS_PER_SEC;

} // timer::elapsed_time

// __________________________________________________________________________
// Start a timer.  Return accumulated time.

void timer::start(const char* timer_name)
{
  // Return immediately if the timer is already running
  if (running)
    return;

  // Report the timer message
  if (timer_name)
    cout << "Timing [" << timer_name << "]...\n";

  // Change timer status to running
  running = 1;

  // Determine the current time;
  start_time = clock();

} // timer::start

// __________________________________________________________________________
// Check how much time has passed.

double timer::check(const char* timer_name)
{
  // Report the timer message
  if (timer_name)
    cout << "Checking timer [" << timer_name << "]...\n";

  if (running)
    return accumulated_time + elapsed_time();

  return accumulated_time;

} // timer::check

// __________________________________________________________________________

void timer::restart(const char* timer_name)
{
  // Report the timer message
  if (timer_name)
    cout << "Restarting [" << timer_name << "]...\n";

  accumulated_time = 0;

  // Change timer status to running
  running = 1;

  // Determine the start time;
  start_time = clock();

} // timer::restart

// __________________________________________________________________________

void timer::stop(const char* timer_name)
{
  // Recalculate the total accumulated time the timer has run
  if (running)
    accumulated_time += elapsed_time();

  running = 0;

  cout.setf(ios::fixed, ios::floatfield);
  setprecision(3);

  if (timer_name)
    cout << "Ending [" << timer_name << "] : " // << setprecision(3)
         << accumulated_time << endl;

} // timer::stop

// __________________________________________________________________________
// timer.C
