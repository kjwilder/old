#ifndef INCLUDES_H_
#define INCLUDES_H_

// __________________________________________________________________________
// This file contains stuff that ought to be included in every header
// and source file.

#include <math.h>
#include <cstdlib>
#include <cassert>

#define exitcheck(x, msg) if (x) { cerr << msg << endl; exit(1); }

// __________________________________________________________________________
// Unfortunately, I want my code to compile with Visual C++.
/*#define WIN95*/
#ifdef WIN95
#define NOX
#ifndef INTEL
#define INTEL
#endif
#define rint(x) static_cast<int>((x) + 0.5)
#define near nearr
#define far farr
#endif

#ifdef WIN95
#define Ifstream(v, f) ifstream v(f, ios::binary)
#define Ofstream(v, f) ofstream v(f, ios::binary)
#define Open(f) open(f, ios::binary)
#define Getline(b, s) getline(b, s, '\n')
#else
#define Ifstream(v, f) ifstream v(f)
#define Ofstream(v, f) ofstream v(f)
#define Open(f) open(f)
#define Getline(b, s) getline(b, s)
#endif

// __________________________________________________________________________
// Useful things to have around.

#ifndef USE_ASSERTS
#define NDEBUG
#endif

#define STRLEN 255
#define FUZZ 0.001

const int Sint = sizeof(int);
const int Slong = sizeof(int64_t);
const int Sfloat = sizeof(float);
const int Sdouble = sizeof(double);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// __________________________________________________________________________
// (Somewhat) platform independent reads and writes.

#ifdef INTEL
#define varwrite(s, v) \
{ for (int vw_i = sizeof(v) - 1; vw_i >= 0; --vw_i) \
  s.write(((char * const) &v) + vw_i, 1); }
#define varread(s, v) \
{ for (int vr_i = sizeof(v) - 1; vr_i >= 0; --vr_i) \
  s.read(((char * const) &v) + vr_i, 1); }
#else
#define varwrite(s, v) s.write((char * const) &v, sizeof(v));
#define varread(s, v) s.read((char * const) &v, sizeof(v));
#endif

#define arraywrite(s, v, n) \
{ for (int aw_i = 0; aw_i < n; ++aw_i) varwrite(s, (v[aw_i])) }
#define arrayread(s, v, n) \
{ for (int ar_i = 0; ar_i < n; ++ar_i) varread(s, (v[ar_i])) }

// __________________________________________________________________________
// Commonly used doubles

#ifndef PI2
  const double PI2   = 6.283185307;
#endif
#ifndef PI
  const double PI    = 3.141592653;
#endif
const double PI_2  = 1.570796326;
const double PI_4  = 0.785398163;
const double PI_8  = 0.392699081;
const double PI_16 = 0.196349540;
const double PI_32 = 0.098174770;
const double SQ2   = 1.414213562;

// __________________________________________________________________________
// Standard types

typedef unsigned char uchar;
typedef unsigned int uint;
typedef uint16_t suint;
typedef uint64_t ulong;

char *basename(char *name);

// __________________________________________________________________________

template <class T>
inline double distance(const T x1, const T y1, const T x2, const T y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// __________________________________________________________________________

inline double linfdist(const double x1, const double y1,
                       const double x2, const double y2) {
  return MAX(fabs(x2 - x1), fabs(y2 - y1));
}

inline int linfdist(int x1, int y1, int x2, int y2) {
  return MAX(abs(x2 - x1), abs(y2 - y1));
}

// __________________________________________________________________________

inline double distance_squared(const double x1, const double y1,
                               const double x2, const double y2) {
  return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

inline int distance_squared(int x1, int y1, int x2, int y2) {
  return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

// __________________________________________________________________________
// This apparently works even if (x1, y1) == (x2, y2)

template <class T>
inline double angle(const T x1, const T y1, const T x2, const T y2) {
  return atan2(y1 - y2, x2 - x1);
}

// __________________________________________________________________________
// Swap the contents of 'a' and 'b'.  Requires operator=().

template <class T>
inline void swap(T& a, T& b) {
  T tmp;
  tmp = a;
  a = b;
  b = tmp;
}

// __________________________________________________________________________

#define gridfun(f, u) \
{ \
  for (int i = 0; i < u.rows(); ++i) \
    for (int j = 0; j < u.cols(); ++j) \
      u(i, j) = f; \
}

// __________________________________________________________________________

#endif  // INCLUDES_H_

