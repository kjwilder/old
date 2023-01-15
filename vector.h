#ifndef VECTOR_H
#define VECTOR_H

// __________________________________________________________________________
// Basic 1-dimensional array

using std::ifstream;

#include <cassert>
#include <fstream>
#include <iostream>

template <class T>
class vector
{
 private:
  int nx;
  T* sto;
  
  vector(const vector& v);
  
 public:
  vector(int x = 0) : nx(x) { assert(x >= 0); sto = (x == 0 ? 0 : new T[x]); }
  ~vector() { delete [] sto; }

  int dim() const { return nx; }
  void init(int x = 0) { assert(x >= 0);
    if (sto != 0) delete [] sto; nx = x; sto = (x == 0 ? 0 : new T[x]); 
  }
  operator T*() const { return sto; }
  T& operator()(int x) const { assert(x >= 0 && x < nx); return sto[x]; }
  void operator=(const vector& v) { 
    assert(nx == v.nx);
    memcpy(sto, v.sto, nx * sizeof(T));
  }
  void operator+=(const vector& v) {
    assert(nx == v.nx);
    for (int i = 0; i < nx; ++i) sto[i] += v.sto[i];
  }
  void clear(const T& val = 0) { 
    for (int i = 0; i < nx; ++i) sto[i] = val; 
  }
  int maxind() const {
    int maxi = 0;
    T max = sto[0];
    for (int i = 0; i < nx; ++i)
      if (sto[i] > max) {
        max = sto[i];
        maxi = i;
      }
    return maxi;
  }

  vector<T>& operator<<(vector &v);
  int read(const char* file);//Andrew adds this at Ken's suggestion 8/12/99

}; // class vector

// __________________________________________________________________________
// Read in text data into a vector.  The file should begin with an
// integer indicating the size of the vector.

template<class T>
int vector<T>::read(const char *file)
{
  Ifstream (ifs, file);
  if (!ifs)
    return 0;
  
  int dim;
  ifs >> dim;
  (*this) = vector<T>(dim);
  for (int i = 0; i < dim; ++i)
    ifs >> (*this)[i];
  return 1;

} // vector::read

// __________________________________________________________________________
// Set a grid equal to 'v'; obliterate 'v'.

template<class T>
vector<T>& vector<T>::operator<<(vector &v)
{
/*
  if (this != &v) {
    delete [] sto;
    nx = v.nx; sto = v.sto;
    v.sto = 0; v.nx = 0;
  }
*/
  return *this;

} // vector::operator<<

// __________________________________________________________________________
// Basic defines

typedef vector<char> cvector;
typedef vector<unsigned char> ucvector;
typedef vector<int> ivector;
typedef vector<unsigned int> uivector;
typedef vector<long> lvector;
typedef vector<float> fvector;
typedef vector<double> dvector;
typedef vector<cvector> cvectors;
typedef vector<ucvector> ucvectors;
typedef vector<ivector> ivectors;
typedef vector<uivector> uivectors;
typedef vector<lvector> lvectors;
typedef vector<fvector> fvectors;
typedef vector<dvector> dvectors;

// __________________________________________________________________________
// vector.h

#endif
