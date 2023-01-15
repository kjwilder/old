#ifndef SIMPSTRG_H_
#define SIMPSTRG_H_

#include <string.h>
#include <cstdio>
#include <iostream>

using std::istream;
using std::ostream;

// __________________________________________________________________________

#define kDefSize 80

class simpstrg {
  friend istream& operator>>(istream& s, simpstrg& x);
  friend ostream& operator<<(ostream& s, const simpstrg& x);

 private:
  unsigned int curr;
  unsigned int max;
  char defbuf[kDefSize + 1];
  char* buf;

 public:
  // Constructors
  simpstrg();
  simpstrg(const simpstrg &s);
  explicit simpstrg(const char* s);
  explicit simpstrg(const int n);

  // Destructor
  ~simpstrg();

  // Operator =
  simpstrg& operator=(const simpstrg& s);
  simpstrg& operator=(const char* s);

  // Concatentation
  simpstrg operator+(const simpstrg& s) const;
  simpstrg operator+=(const simpstrg& s);
  simpstrg operator+=(const char* buf);

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

#endif  // SIMPSTRG_H_

