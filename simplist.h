#ifndef SIMPLIST_H
#define SIMPLIST_H

// __________________________________________________________________________
// Include files

#include <cassert>
#include <iostream>

// __________________________________________________________________________
// Simple bi-directional list class.

template <class T>
class simplistiterator
{
  class simplistnode
  {
   public:
    simplistnode *prev, *next;
    T data;
    
    simplistnode(const T& d, simplistnode* p = 0, simplistnode* n = 0) :  
        prev(p), next(n), data(d) { }
  };

 private:
  simplistnode *p;

 public:
  simplistiterator(simplistnode *x = 0) : p(x) { }
  simplistiterator(const simplistiterator<T>& t) : p(t.p) { }
  void operator=(const simplistiterator<T>& t) { p = t.p; }
  int operator==(const simplistiterator<T>& t) const { return (p == t.p); }
  operator int() const { return (p ? 1 : 0); }
  T* operator->() const { assert(p); return &p->data; }
  T& operator*() const { assert(p); return p->data; }
  simplistiterator<T> prev() const 
    { return simplistiterator<T>(p ? p->prev : 0); }
  simplistiterator<T> next() const 
    { return simplistiterator<T>(p ? p->next : 0); }
  simplistiterator<T> operator--()
    { p = (p ? p->prev : 0); return *this; }
  simplistiterator<T> operator++()
    { p = (p ? p->next : 0); return *this; }
  void insert(const T& d) { p = new simplistnode(d, 0, p); }
  void append(const T& d) { 
    if (p) 
      p->next = new simplistnode(d, p, 0);
    else
      p = new simplistnode(d, 0, 0); 
  }
  void assign_prev(const simplistiterator& pr) { p->prev = pr.p; }
  void assign_next(const simplistiterator& pr) { p->next = pr.p; }
  void remove() { delete p; p = 0; }
};

// __________________________________________________________________________

template <class T>
class simplist
{

 private:
  simplistiterator<T> f, l;
  int len;

 public:
  simplist() : len(0) { }
  ~simplist() { empty(); }
  void makenode() { }
  void empty();
  simplistiterator<T> first() const { return f; }
  simplistiterator<T> last() const { return l; }
  void insert(const T& d);
  void append(const T& d);
  int remove(const T& d);
  int remove(simplistiterator<T> i);
  int length() const { return len; }
};

template <class T>
inline void simplist<T>::insert(const T& d)
{
  l.insert(d);
  if (++len == 1)
    f = l;
  else
    l = l.next();
  
} // simplist:insert

template <class T>
inline void simplist<T>::append(const T& d)
{
  ++len;
  l.insert(d);
  if (++len == 1)
    f = l;
  else
    l = l.next();
  
} // simplist:insert

template <class T>
inline int simplist<T>::remove(const T& d)
{
  for (simplistiterator<T> it = f; it; ++it)
  {
    if (*it == d) {
      --len;
      simplistiterator<T> p = it.prev();
      simplistiterator<T> n = it.next();
      it.remove();
      if (p) 
        p.assign_next(n);
      else
        f = n;
      if (n) 
        n.assign_prev(p);
      else
        l = p;
      return 1;
    }
  }
  return 0;
  
} // simplist::remove

template <class T>
inline int simplist<T>::remove(simplistiterator<T> i)
{
  for (simplistiterator<T> it = f; it; ++it)
  {
    if (it == i) {
      --len;
      simplistiterator<T> p = i.prev();
      simplistiterator<T> n = i.next();
      i.remove();
      if (p) 
        p.assign_next(n);
      else
        f = n;
      if (n) 
        n.assign_prev(p);
      else
        l = p;
      return 1;
    }
  }
  return 0;
  
} // simplist::remove

template <class T>
inline void simplist<T>::empty()
{
  simplistiterator<T> iter = f;
  while (iter) {
    simplistiterator<T> niter = iter.next();
    iter.remove();
    iter = niter;
  }
  f = 0;
  l = 0;
  len = 0;

} // simplist::empty

// __________________________________________________________________________
// simplist.h

#endif // SIMPLIST_H

