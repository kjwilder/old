#include <algorithm>
#include <cstdio>
#include "simpstrg.h"

#define notype

notype simpstrg::simpstrg() : curr(0), max(kMaxStringLength), buf(defbuf) {
  buf[0] = 0;
}

notype simpstrg::simpstrg(const simpstrg &s) : curr(s.curr) {
  max = (curr > kMaxStringLength ? curr : kMaxStringLength);
  buf = (curr > kMaxStringLength ? new char[curr + 1] : defbuf);
  memcpy(buf, s.buf, curr + 1);
}

notype simpstrg::simpstrg(const char* s) : curr(strlen(s)) {
  max = (curr > kMaxStringLength ? curr : kMaxStringLength);
  buf = (curr > kMaxStringLength ? new char[curr + 1] : defbuf);
  memcpy(buf, s, curr + 1);
}

notype simpstrg::simpstrg(const int n) : max(kMaxStringLength), buf(defbuf) {
  snprintf(buf, kMaxStringLength, "%i", n);
  curr = strlen(buf);
}

notype simpstrg::~simpstrg() {
  if (buf != defbuf) {
    delete [] buf;
  }
}

simpstrg& simpstrg::operator=(const simpstrg& s) {
  if (this == &s) {
    return *this;
  }
  if (max < (curr = s.curr)) {
    if (max > kMaxStringLength) {
      delete [] buf;
    }
    buf = new char[curr + 1];
    max = curr;
  }
  memcpy(buf, s.buf, curr + 1);
  return *this;
}

simpstrg& simpstrg::operator=(const char* s) {
  if (this->buf == s) {
    return *this;
  }
  if (max < (curr = strlen(s))) {
    if (max > kMaxStringLength) {
      delete [] buf;
    }
    buf = new char[curr + 1];
    max = curr;
  }
  memcpy(buf, s, curr + 1);
  return *this;
}

simpstrg simpstrg::operator+=(const simpstrg& s) {
  if (max > curr + s.curr) {
    memcpy(buf + curr, s.buf, s.curr);
    buf[curr += s.curr] = 0;
  } else if (max > kMaxStringLength) {
    max = curr + s.curr;
    char *tmpbuf = new char[max + 1];
    memcpy(tmpbuf, buf, curr);
    memcpy(tmpbuf + curr, s.buf, s.curr);
    tmpbuf[curr += s.curr] = 0;
    delete [] buf;
    buf = tmpbuf;
  } else {  // max == kMaxStringLength
    max = curr + s.curr;
    buf = new char[max + 1];
    memcpy(buf, defbuf, curr);
    memcpy(buf + curr, s.buf, s.curr);
    buf[curr += s.curr] = 0;
  }
  return *this;
}

simpstrg simpstrg::operator+=(const char* buf) {
  return this->operator+=(simpstrg(buf));
}

simpstrg simpstrg::operator+(const simpstrg& s) const {
  simpstrg tmps;
  if ((tmps.curr = curr + s.curr) > kMaxStringLength) {
    tmps.max = tmps.curr;
  }
  tmps.buf = (tmps.curr > kMaxStringLength ?
      new char[tmps.curr + 1] : tmps.defbuf);
  memcpy(tmps.buf, buf, curr);
  memcpy(tmps.buf + curr, s.buf, s.curr + 1);
  return tmps;
}

int simpstrg::operator==(const simpstrg& s) const {
  if (this == &s) {
    return 1;
  }
  if (curr != s.curr) {
    return 0;
  }
  return (!memcmp(buf, s.buf, curr));
}

int simpstrg::operator==(const char* s) const {
  if (this->buf == s) {
    return 1;
  }
  if (curr != strlen(s)) {
    return 0;
  }
  return (!memcmp(buf, s, curr));
}

int simpstrg::operator!=(const simpstrg& s) const {
  return 1 - operator==(s);
}

int simpstrg::operator!=(const char* s) const {
  return 1 - operator==(s);
}

simpstrg simpstrg::operator()(int start, int length) const {
  // Return a null string if start is larger than the string length.
  if (start > 0 && static_cast<unsigned int>(start) >= curr) {
    return simpstrg();
  }

  // Start < 0 counts from the end of the string.  For instance,
  // simpstrg("example")(-4, 3) = "mpl"
  if (start < 0) {
    start += curr;
    if (start < 0) {
      start = 0;
    }
  }

  // length < 0 counts from the end of the string.  For instance,
  // simpstrg("example")(3,-2) = "mpl"
  if (length < 0) {
    length += curr - start + 1;
  }
  if (static_cast<unsigned int>(length) >
      curr - static_cast<unsigned int>(start)) {
    length = curr - start;
  }

  if (length < 0) {
    return simpstrg();
  }

  simpstrg tmps(length);
  tmps.curr = length;
  memcpy(tmps.buf, buf + start, length);
  tmps.buf[length] = 0;
  return tmps;
}

notype simpstrg::operator const char *() const {
  return buf;
}

std::istream& operator>>(std::istream& s, simpstrg& x) {
  x = "";
  char c;
  int done = 0;
  while (!done) {
    char buf[kMaxStringLength];
    s.get(buf, kMaxStringLength, '\n');
    done = (!(s.get(c) && c != '\n'));
    x += buf;
  }
  return s;
}

std::ostream& operator<<(std::ostream& o, const simpstrg& s) {
  o << s.buf;
  return o;
}

#undef notype

