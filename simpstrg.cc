#ifndef SIMPSTRG_C
#define SIMPSTRG_C

// __________________________________________________________________________
// Include files

#include "simpstrg.h"

#define notype

// __________________________________________________________________________

notype simpstrg::simpstrg() : curr(0), max(defsize), buf(defbuf)
{
  buf[0] = 0; 
}

notype simpstrg::simpstrg(const simpstrg &s) : curr(s.curr)
{ 
  max = (curr > defsize ? curr : defsize);
  buf = (curr > defsize ? new char[curr + 1] : defbuf);
  memcpy(buf, s.buf, curr + 1); 
}

notype simpstrg::simpstrg(const char* s) : curr(strlen(s))
{ 
  max = (curr > defsize ? curr : defsize);
  buf = (curr > defsize ? new char[curr + 1] : defbuf);
  memcpy(buf, s, curr + 1); 
}

notype simpstrg::simpstrg(const int n) : max(defsize), buf(defbuf)
{
  sprintf(buf, "%i", n);
  curr = strlen(buf);
}

notype simpstrg::~simpstrg() 
{ 
  if (buf != defbuf) 
    delete [] buf; 
}

simpstrg& simpstrg::operator=(const simpstrg& s)
{ 
  if (this == &s) 
    return *this; 
  if (max < (curr = s.curr)) { 
    if (max > defsize)
      delete [] buf; 
    buf = new char[curr + 1]; 
    max = curr; 
  }
  memcpy(buf, s.buf, curr + 1); 
  return *this; 
}

simpstrg& simpstrg::operator=(const char* s) 
{ 
  if (this->buf == s) 
    return *this; 
  if (max < (curr = strlen(s))) { 
    if (max > defsize)
      delete [] buf; 
    buf = new char[curr + 1]; 
    max = curr; 
  }
  memcpy(buf, s, curr + 1); 
  return *this; 
}

simpstrg simpstrg::operator+=(const simpstrg& s)
{ 
  if (max > curr + s.curr) {
    memcpy(buf + curr, s.buf, s.curr);
    buf[curr += s.curr] = 0;
  }
  else if (max > defsize) {
    max = curr + s.curr;
    char *tmpbuf = new char[max + 1];
    memcpy(tmpbuf, buf, curr);
    memcpy(tmpbuf + curr, s.buf, s.curr);
    tmpbuf[curr += s.curr] = 0;
    delete [] buf;
    buf = tmpbuf;
  }
  else { // max == defsize
    max = curr + s.curr;
    buf = new char[max + 1];
    memcpy(buf, defbuf, curr);
    memcpy(buf + curr, s.buf, s.curr);
    buf[curr += s.curr] = 0;
  }
  return *this; 
}

simpstrg simpstrg::operator+(const simpstrg& s) const 
{ 
  simpstrg tmps;
  if ((tmps.curr = curr + s.curr) > defsize)
    tmps.max = tmps.curr;
  tmps.buf = (tmps.curr > defsize ? new char[tmps.curr + 1] : tmps.defbuf);
  memcpy(tmps.buf, buf, curr); 
  memcpy(tmps.buf + curr, s.buf, s.curr + 1);
  return tmps; 
}

int simpstrg::operator==(const simpstrg& s) const 
{ 
  if (this == &s) 
    return 1;
  if (curr != s.curr) 
    return 0;
  return (!memcmp(buf, s.buf, curr)); 
}

int simpstrg::operator==(const char* s) const 
{
  if (this->buf == s)
    return 1;
  if (curr != strlen(s)) 
    return 0;
  return (!memcmp(buf, s, curr)); 
}

int simpstrg::operator!=(const simpstrg& s) const 
{ 
  return 1 - operator==(s); 
}

int simpstrg::operator!=(const char* s) const 
{ 
  return 1 - operator==(s); 
}

simpstrg simpstrg::operator()(int start, int length) const
{
  // Return a null string if start is larger than the string length.
  if (start >= int(curr))
    return simpstrg();

  // Start < 0 counts from the end of the string.  For instance,
  // simpstrg("example")(-4, 3) = "mpl"
  if (start < 0) {
    start += int(curr);
    if (start < 0)
      start = 0;
  }

  // length < 0 counts from the end of the string.  For instance,
  // simpstrg("example")(3,-2) = "mpl"
  if (length < 0)
    length += curr - start + 1;
  if (length > int(curr) - start)
    length = int(curr) - start;

  if (length < 0)
    return simpstrg();

  simpstrg tmps(length);
  tmps.curr = length;
  memcpy(tmps.buf, buf + start, length);
  tmps.buf[length] = 0;
  return tmps;
}

notype simpstrg::operator const char *() const 
{ 
  return buf; 
}

istream& operator>>(istream& s, simpstrg& x)
{
  x = "";
  char c;
  int done = 0;
  while (!done) {
    char buf[80];
    s.get(buf, 80, '\n');
    done = (!(s.get(c) && c != '\n'));
    x += buf;
  }
  return s;
}

ostream& operator<<(ostream& s, const simpstrg& x)
{
  s << x.buf; 
  return s;
}

// __________________________________________________________________________
// simpstrg.h

#undef notype

#endif // SIMPSTRG_C

