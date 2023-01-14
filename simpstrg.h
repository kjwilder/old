#ifndef SIMPSTRG_H
#define SIMPSTRG_H

// __________________________________________________________________________
// Include files

#include <iostream>
#include <string.h>
#include <cstdio>

// __________________________________________________________________________

#define defsize 80

class simpstrg
{
  friend class istream& operator>>(istream& s, simpstrg& x);
  friend class ostream& operator<<(ostream& s, const simpstrg& x);

 private:
  unsigned int curr;
  unsigned int max;
  char defbuf[defsize + 1];
  char* buf;

 public:
  // Constructors
  simpstrg();
  simpstrg(const simpstrg &s);
  simpstrg(const char* s);
  simpstrg(const int n);

  // Destructor
  ~simpstrg();

  // Operator =
  simpstrg& operator=(const simpstrg& s);
  simpstrg& operator=(const char* s);

  // Concatentation
  simpstrg operator+(const simpstrg& s) const;
  simpstrg operator+=(const simpstrg& s);

  // Operator ==
  int operator==(const simpstrg& s) const;
  int operator==(const char* s) const;
  int operator!=(const simpstrg& s) const;
  int operator!=(const char* s) const;

  // Substring
  simpstrg operator()(int start, int length) const;

  // Operator char*
  operator const char *() const;
};

// __________________________________________________________________________
// simpstrg.h

#endif // SIMPSTRG_H

