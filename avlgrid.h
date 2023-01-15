#ifndef AVLGRID_H
#define AVLGRID_H

// _________________________________________________________________________
// Includes

#include "grid.h"

// _________________________________________________________________________

const int defavlsize = 100;

enum balfactor { LH, EH, RH };

template <class T>
class avlgrid : public grid<T>
{
 private:
  int numnodes;
  int rootnode;
  igrid parent, left, right;
  vector<balfactor> bal;

  void rotleft(int currnode);
  void rotright(int currnode);
  void rightbalance(int currnode);
  void leftbalance(int currnode);
  int comparenode(int curr, vector<T>& newT);
  int first() const;
  int last() const;
  int prev(int curr) const;
  int next(int curr) const;
  int startsearch(T dist, int currnode);

  int checkfamily() const;
  int checkbalances(int currnode = -2) const;
  int depth(int currnode = -2) const;

  int invariant() const;

 public:
  avlgrid() : numnodes(0), rootnode(-1) { }
  avlgrid(int x) : grid<T>(x), numnodes(0), rootnode(-1), 
    parent(x), left(x), right(x), bal(x) { }
  avlgrid(int x, int  y) : grid<T>(x, y), numnodes(0), rootnode(-1),
    parent(x), left(x), right(x), bal(x) { }
  ~avlgrid() { }

  void init(int x, int y = 0) { grid<T>::init(x, y); numnodes = 0; rootnode = -1; 
    parent.init(x); left.init(x); right.init(x); bal.init(x); }
  void reinit() { numnodes = 0; rootnode = -1; }

  int tooclose(vector<T>& newT, T dist);
  int insert(vector<T>& newT, int allowdups = 0, int top = -1);

  void dump() const;
  void dump_rev() const;

}; // class avlgrid<T>

// __________________________________________________________________________

template <class T>
int avlgrid<T>::comparenode(int curr, vector<T>& newT)
{
  assert(this->cols() == newT.dim());
  for (int i = 0; i < this->cols(); ++i) {
    if ((*this)(curr, i) < newT[i])
      return 1;
    if ((*this)(curr, i) > newT[i])
      return -1;
  }
  return 0;

} // avlgrid::comparenode

// __________________________________________________________________________

template <class T>
int avlgrid<T>::insert(vector<T>& newT, int allowdups, int currnode)
{
  if (this->rows() == 0)
    init(defavlsize, newT.dim());
  
  assert(newT.dim() == this->cols());

  // Expand the grid if necessary
  if (numnodes == this->rows()) {
    grid<T> tmp(this->rows() * 2, this->cols());
    igrid nparent(this->rows() * 2);
    igrid nleft(this->rows() * 2);
    igrid nright(this->rows() * 2);
    vector<balfactor> nbal(this->rows() * 2);

    for (int i = 0; i < this->rows(); ++i) {
      for (int j = 0; j < this->cols(); ++j)
	tmp(i, j) = (*this)(i, j);
      nparent(i) = parent(i);
      nleft(i) = left(i);
      nright(i) = right(i);
      nbal(i) = bal(i);
    }
    (*this) << tmp;
    (*this).parent << nparent;
    (*this).left << nleft;
    (*this).right << nright;
    (*this).bal << nbal;
  }
  
  // Handle the case when the tree is empty
  if (numnodes == 0) {
    rootnode = 0;
    parent(0) = left(0) = right(0) = -1;
    bal[0] = EH;
    for (int i = 0; i < this->cols(); ++i)
      (*this)(0, i) = newT[i];
    numnodes = 1;
    return 1;
  }

  // When not called recursively, the currnode is the root of the tree
  if (currnode == -1)
    currnode = rootnode;

  // Return with no height change if newT is already in the tree
  int compare = comparenode(currnode, newT);
  if (compare == 0 && allowdups == 0)
    return 0;

  // Recursively insert to the left if newT is less then the current node
  int taller = 0;
  if (compare == -1)
  {
    if (left(currnode) == -1)
    {
      left(currnode) = numnodes;
      parent(numnodes) = currnode;
      left(numnodes) = right(numnodes) = -1;
      bal[numnodes] = EH;
      for (int i = 0; i < this->cols(); ++i)
	(*this)(numnodes, i) = newT[i];
      ++numnodes;
      taller = 1;
    }
    else
      taller = insert(newT, allowdups, left(currnode));
    if (taller)
    {
      switch (bal[currnode])
      {
       case LH:
	leftbalance(currnode);
	return 0;
       case EH:
	bal[currnode] = LH;
	return 1;
       case RH:
	bal[currnode] = EH;
	return 0;
      }
    }
    else
      return 0;
  }
  else
  {
    if (right(currnode) == -1)
    {
      right(currnode) = numnodes;
      parent(numnodes) = currnode;
      left(numnodes) = right(numnodes) = -1;
      bal[numnodes] = EH;
      for (int i = 0; i < this->cols(); ++i)
	(*this)(numnodes, i) = newT[i];
      ++numnodes;
      taller = 1;
    }
    else
      taller = insert(newT, allowdups, right(currnode));
    if (taller)
    {
      switch (bal[currnode])
      {
       case LH:
	bal[currnode] = EH;
	return 0;
       case EH:
	bal[currnode] = RH;
	return 1;
       case RH:
	rightbalance(currnode);
	return 0;
      }
    }
    else
      return 0;
  }

  return 0;

} // avlgrid::insert

// __________________________________________________________________________
// Check whether newT is within L1 distance 'dist' of existing nodes

template <class T>
int avlgrid<T>::startsearch(T min, int currnode)
{
  assert(currnode != -1);

  if (right(currnode) != -1 && (*this)(currnode, 0) < min)
    return startsearch(min, right(currnode));

  if (left(currnode) != -1 && (*this)(currnode, 0) >= min)
    return startsearch(min, left(currnode));

  return currnode;

} // avlgrid::startsearch

// __________________________________________________________________________
// Check whether newT is within L1 distance 'dist' of existing nodes

template <class T>
int avlgrid<T>::tooclose(vector<T>& newT, T dist)
{
  // Return if the tree is empty
  if (rootnode == -1)
    return 0;

  // Return if dist == 0.  Don't worry about duplicates as the
  // the insert routine handles them correctly.
  if (dist == 0)
    return 0;

  T max, newmax;
  int curr = startsearch(newT[0] < dist ? 0 : newT[0] - dist, rootnode);
  while (curr != -1 && (*this)(curr, 0) <= newT[0] + dist)
  {
    max = 0;
    const int len = this->cols();
    for (int i = 0; i < len; ++i) {
      newmax = abs(newT[i] - (*this)(curr, i));
      if (newmax > max)
	max = newmax;
    }
    if (max <= dist)
      return 1;
    curr = next(curr);
  }

  return 0;

} // avlgrid::tooclose

// __________________________________________________________________________

template <class T>
void avlgrid<T>::rotleft(int currnode)
{
  assert(currnode >= 0);
  assert(right(currnode) >= 0);
  int par = parent(currnode);
  int rt = right(currnode);
  int lt = left(rt);
  left(rt) = currnode;
  right(currnode) = lt;
  parent(currnode) = rt;
  parent(rt) = par;
  if (lt != -1)
    parent(lt) = currnode;
  if (par == -1)
    rootnode = rt;
  else {
    assert((right(par) == currnode && left(par) != currnode) ||
	   (right(par) != currnode && left(par) == currnode));
    if (right(par) == currnode)
      right(par) = rt;
    else
      left(par) = rt;
  }
  assert(checkfamily());

} // avlgrid::rotleft

// __________________________________________________________________________

template <class T>
void avlgrid<T>::rotright(int currnode)
{
  assert(currnode >= 0);
  assert(left(currnode) >= 0);
  int par = parent(currnode);
  int lt = left(currnode);
  int rt = right(lt);
  right(lt) = currnode;
  left(currnode) = rt;
  parent(currnode) = lt;
  parent(lt) = par;
  if (rt != -1)
    parent(rt) = currnode;
  if (par == -1)
    rootnode = lt;
  else {
    assert((right(par) == currnode && left(par) != currnode) ||
	   (right(par) != currnode && left(par) == currnode));
    if (right(par) == currnode)
      right(par) = lt;
    else
      left(par) = lt;
  }
  assert(checkfamily());

} // avlgrid::rotright

// __________________________________________________________________________

template <class T>
void avlgrid<T>::rightbalance(int currnode)
{
  int rt = right(currnode);
  int lt = left(rt);
  switch (bal[rt])
  {
   case RH:
    bal[currnode] = bal[rt] = EH;
    rotleft(currnode);
    break;
   case EH:
    cerr << "Attempting to right balance an equal height branch\n";
    exit(1);
    break;
   case LH:
    switch(bal[lt])
    {
     case EH:
      bal[currnode] = EH;
      bal[rt] = EH;
      break;
     case LH: 
      bal[currnode] = EH;
      bal[rt] = RH;
      break;
     case RH:
      bal[currnode] = LH;
      bal[rt] = EH;
      break;
    }
    bal[lt] = EH;
    rotright(rt);
    rotleft(currnode);
    break;
  }

} // avlgrid::rightbalance

// __________________________________________________________________________

template <class T>
void avlgrid<T>::leftbalance(int currnode)
{
  int lt = left(currnode);
  int rt = right(lt);
  switch (bal[lt])
  {
   case RH:
    switch(bal[rt])
    {
     case EH:
      bal[currnode] = EH;
      bal[lt] = EH;
      break;
     case RH: 
      bal[currnode] = EH;
      bal[lt] = LH;
      break;
     case LH:
      bal[currnode] = RH;
      bal[lt] = EH;
      break;
    }
    bal[rt] = EH;
    rotleft(lt);
    rotright(currnode);
    break;
   case EH:
    cerr << "Attempting to left balance an equal height branch\n";
    exit(1);
   case LH:
    bal[currnode] = bal[lt] = EH;
    rotright(currnode);
    break;
  }

} // avlgrid::leftbalance

// __________________________________________________________________________
// Dump an avlgrid.

template <class T>
void avlgrid<T>::dump() const
{
  for (int i = first(); i != -1; i = next(i)) {
    for (int j = 0; j < this->cols(); ++j)
      cout << (*this)(i, j) << " ";
    cout << endl;
  }

} // avlgrid::dump

// __________________________________________________________________________
// Dump an avlgrid in reverse order.

template <class T>
void avlgrid<T>::dump_rev() const
{
  for (int i = last(); i != -1; i = prev(i)) {
    for (int j = 0; j < this->cols(); ++j)
      cout << (*this)(i, j) << " ";
    cout << endl;
  }

} // avlgrid::dump_rev

// __________________________________________________________________________
// Consistency checks.

template <class T>
int avlgrid<T>::invariant() const
{
  if (checkfamily() && checkbalances())
    return 1;

  return 0;

} // avlgrid::invariant

// __________________________________________________________________________
// Check that parents and children are consistent.

template <class T>
int avlgrid<T>::checkfamily() const
{
  for (int i = 0; i < numnodes; ++i)
  {
    int par = parent(i);
    if (par != -1 && left(par) != i && right(par) != i)
      return 0;

    int lef = left(i);
    if (lef != -1 && parent(lef) != i)
      return 0;

    int rig = right(i);
    if (rig != -1 && parent(rig) != i)
      return 0;

  }
  return 1;

} // avlgrid::checkfamily

// __________________________________________________________________________
// Check that avl tree is properly balanced
// This is a very expensive routine.

template <class T>
int avlgrid<T>::checkbalances(int currnode) const
{
  if (currnode == -2)
    currnode = rootnode;

  if (currnode == -1)
    return 1;

  int leftdepth = depth(left(currnode));
  int rightdepth = depth(right(currnode));
  if (abs(leftdepth - rightdepth) > 1)
    return 0;

  if (rightdepth - leftdepth + 1 != bal[currnode])
    return 0;

  if (!checkbalances(right(currnode)) || !checkbalances(left(currnode)))
    return 0;

  return 1;

} // avlgrid::checkbalances

// __________________________________________________________________________
// Calculate depth of a node

template <class T>
int avlgrid<T>::depth(int currnode) const
{
  if (currnode == -2)
    currnode = rootnode;

  if (currnode == -1)
    return 0;

  int leftdepth = 1 + depth(left(currnode));
  int rightdepth = 1 + depth(right(currnode));

  return (leftdepth > rightdepth ? leftdepth : rightdepth);

} // avlgrid::depth

// __________________________________________________________________________
// Calculate the smallest element in the tree

template <class T>
int avlgrid<T>::first() const
{
  if (rootnode == -1)
    return -1;

  int i = rootnode;
  while (left(i) != -1)
    i = left(i);

  return i;

} // avlgrid::first

// __________________________________________________________________________
// Calculate the largest element in the tree

template <class T>
int avlgrid<T>::last() const
{
  if (rootnode == -1)
    return -1;

  int i = rootnode;
  while (right(i) != -1)
    i = right(i);

  return i;

} // avlgrid::last

// __________________________________________________________________________
// Calculate the next element in a tree

template <class T>
int avlgrid<T>::next(int curr) const
{
  assert(curr >= 0);

  if (right(curr) != -1) {
    curr = right(curr);
    while (left(curr) != -1)
      curr = left(curr);
    return curr;
  }

  if (parent(curr) != -1) {
    while (parent(curr) != -1 && right(parent(curr)) == curr)
      curr = parent(curr);
    return parent(curr);
  }

  return -1;

} // avlgrid::next

// __________________________________________________________________________
// Calculate the previous element in a tree

template <class T>
int avlgrid<T>::prev(int curr) const
{
  assert(curr >= 0);

  if (left(curr) != -1) {
    curr = left(curr);
    while (right(curr) != -1)
      curr = right(curr);
    return curr;
  }

  if (parent(curr) != -1) {
    while (parent(curr) != -1 && left(parent(curr)) == curr)
      curr = parent(curr);
    return parent(curr);
  }

  return -1;

} // avlgrid::prev

// _________________________________________________________________________

typedef avlgrid<char> cavlgrid;
typedef avlgrid<uchar> ucavlgrid;
typedef avlgrid<int> iavlgrid;
typedef avlgrid<uint> uiavlgrid;
typedef avlgrid<long> lavlgrid;
typedef avlgrid<float> favlgrid;
typedef avlgrid<double> davlgrid;

// __________________________________________________________________________
// avlgrid.h

#endif // AVLGRID_H

