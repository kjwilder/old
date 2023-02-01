#ifndef AVLGRID_H_
#define AVLGRID_H_

#include <iostream>
#include <vector>

enum balancefactor { LH, EH, RH };

template <class T>
class avlvector : public std::vector<T> {
 private:
  int root;
  std::vector<int> parent, left, right;  
  std::vector<balancefactor> bal;

  void rotate_left(int ind);
  void rotate_right(int ind);
  void balance_right(int ind);
  void balance_left(int ind);
  int compare_node(int curr, const T& newT);
  int first() const;
  int last() const;
  int prev(int ind) const;
  int next(int ind) const;
  // int start_search(T dist, int ind);

  bool familially_consistent() const;
  bool balanced(int ind = -2) const;
  int depth(int ind = -2) const;

  bool invariant() const;

 public:
  avlvector() : root(-1) { }
  explicit avlvector(int x) : root(-1) {
    this->reserve(x);
    parent.reserve(x);
    left.reserve(x);
    right.reserve(x);
    bal.reserve(x);
  }
  ~avlvector() { }

  // bool close(const T& newT, T dist);
  bool insert(const T& newT, int top = -1);

  void dump() const;
  void dump_rev() const;
};

// __________________________________________________________________________

template <class T>
int avlvector<T>::compare_node(int curr, const T& newT) {
  if ((*this)[curr] < newT)
    return 1;
  if ((*this)[curr] > newT)
    return -1;
  return 0;
}

// __________________________________________________________________________
// Insert an element into a tree. Users call this with only one argument,
// the item to insert, and should ignore the return value that indicates if
// the depth of a tree (or sub-tree) increased and is used for rebalancing.

template <class T>
bool avlvector<T>::insert(const T& newT, int ind) {
  // Uncomment the next line only if intensively debugging.
  // assert(invariant());

  // Handle the case when the tree is empty
  if (this->size() == 0) {
    root = 0;
    parent.push_back(-1);
    left.push_back(-1);
    right.push_back(-1);
    bal.push_back(EH);
    this->push_back(newT);
    return true;
  }

  // When not called recursively, the ind is the root of the tree
  if (ind == -1)
    ind = root;

  // Return with no height change if newT is already in the tree
  int compare = compare_node(ind, newT);
  if (compare == 0)
    return false;

  // Recursively insert to the left if newT is less then the current node
  int taller = 0;
  if (compare == -1) {
    if (left[ind] == -1) {
      left[ind] = this->size();
      parent.push_back(ind);
      left.push_back(-1);
      right.push_back(-1);
      bal.push_back(EH);
      this->push_back(newT);
      taller = true;
    } else {
      taller = insert(newT, left[ind]);
    }
    if (!taller)
      return false;
    switch (bal[ind]) {
     case LH:
      balance_left(ind);
      return false;
     case EH:
      bal[ind] = LH;
      return true;
     case RH:
      bal[ind] = EH;
      return false;
    }
  } else {
    if (right[ind] == -1) {
      right[ind] = this->size();
      parent.push_back(ind);
      left.push_back(-1);
      right.push_back(-1);
      bal.push_back(EH);
      this->push_back(newT);
      taller = true;
    } else {
      taller = insert(newT, right[ind]);
    }
    if (!taller)
      return false;
    switch (bal[ind]) {
     case LH:
      bal[ind] = EH;
      return false;
     case EH:
      bal[ind] = RH;
      return true;
     case RH:
      balance_right(ind);
      return false;
    }
  }
  return false;
}

// __________________________________________________________________________
// Check whether newT is within L1 distance 'dist' of existing nodes

/*
template <class T>
int avlvector<T>::start_search(T min, int ind) {
  assert(ind != -1);

  if (right[ind] != -1 && (*this)[ind] < min)
    return start_search(min, right[ind]);

  if (left[ind] != -1 && (*this)[ind] >= min)
    return start_search(min, left[ind]);

  return ind;
}
*/

// __________________________________________________________________________
// Check whether newT is within L1 distance 'dist' of existing nodes

/*
template <class T>
bool avlvector<T>::close(const T& newT, T dist) {
  // Return if the tree is empty
  if (root == -1)
    return false;

  // Return if dist == 0.  Don't worry about duplicates as the
  // the insert routine handles them correctly.
  if (dist == 0)
    return false;

  int curr = start_search(newT < dist ? 0 : newT - dist, root);
  while (curr != -1 && (*this)[curr] <= newT + dist)
  {
    T max = 0;
    const int len = dimy();
    for (int i = 0; i < len; ++i)
    {
      T newmax = abs(newT[i] - (*this)(curr, i));
      if (newmax > max)
        max = newmax;
    }
    if (max <= dist)
      return true;
    curr = next(curr);
  }

  return false;
}
*/

// __________________________________________________________________________

template <class T>
void avlvector<T>::rotate_left(int ind) {
  assert(ind >= 0);
  assert(right[ind] >= 0);
  int par = parent[ind];
  int rt = right[ind];
  int lt = left[rt];
  left[rt] = ind;
  right[ind] = lt;
  parent[ind] = rt;
  parent[rt] = par;
  if (lt != -1)
    parent[lt] = ind;
  if (par == -1) {
    root = rt;
  } else {
    assert((right[par] == ind && left[par] != ind) ||
           (right[par] != ind && left[par] == ind));
    if (right[par] == ind)
      right[par] = rt;
    else
      left[par] = rt;
  }
  assert(familially_consistent());
}

// __________________________________________________________________________

template <class T>
void avlvector<T>::rotate_right(int ind) {
  assert(ind >= 0);
  assert(left[ind] >= 0);
  int par = parent[ind];
  int lt = left[ind];
  int rt = right[lt];
  right[lt] = ind;
  left[ind] = rt;
  parent[ind] = lt;
  parent[lt] = par;
  if (rt != -1)
    parent[rt] = ind;
  if (par == -1) {
    root = lt;
  } else {
    assert((right[par] == ind && left[par] != ind) ||
           (right[par] != ind && left[par] == ind));
    if (right[par] == ind)
      right[par] = lt;
    else
      left[par] = lt;
  }
  assert(familially_consistent());
}

// __________________________________________________________________________
// Right-balance the (sub-)tree whose root element has the provided index.

template <class T>
void avlvector<T>::balance_right(int ind) {
  int rt = right[ind];
  int lt = left[rt];
  switch (bal[rt]) {
    case RH:
      bal[ind] = bal[rt] = EH;
      rotate_left(ind);
      break;
    case EH:
      std::cerr << "Attempting to right balance an equal height branch\n";
      exit(1);
      break;
    case LH:
      switch (bal[lt]) {
        case EH:
          bal[ind] = EH;
          bal[rt] = EH;
          break;
        case LH:
          bal[ind] = EH;
          bal[rt] = RH;
          break;
        case RH:
          bal[ind] = LH;
          bal[rt] = EH;
          break;
      }
      bal[lt] = EH;
      rotate_right(rt);
      rotate_left(ind);
      break;
  }
}

// __________________________________________________________________________
// Left-balance the (sub-)tree whose root element has the provided index.

template <class T>
void avlvector<T>::balance_left(int ind) {
  int lt = left[ind];
  int rt = right[lt];
  switch (bal[lt]) {
    case RH:
      switch (bal[rt]) {
        case EH:
          bal[ind] = EH;
          bal[lt] = EH;
          break;
        case RH:
          bal[ind] = EH;
          bal[lt] = LH;
          break;
        case LH:
          bal[ind] = RH;
          bal[lt] = EH;
          break;
      }
      bal[rt] = EH;
      rotate_left(lt);
      rotate_right(ind);
      break;
    case EH:
      std::cerr << "Attempting to left balance an equal height branch\n";
      exit(1);
    case LH:
      bal[ind] = bal[lt] = EH;
      rotate_right(ind);
      break;
  }
}

// __________________________________________________________________________
// Dump an avlvector.

template <class T>
void dumpbase(const T& t) {
  std::cout << t << std::endl;
}

template <class V>
void dumpbase(const std::vector<V> v) {
  for (typename std::vector<V>::size_type i = 0; i < v.size() - 1; ++i) {
    std::cout << v[i] << " ";
  }
  if (v.size() > 0) {
    std::cout << v[v.size() - 1];
  }
  std::cout << std::endl;
}

template <class T>
void avlvector<T>::dump() const {
  for (int i = first(); i != -1; i = next(i)) {
    dumpbase((*this)[i]);
  }
}

// __________________________________________________________________________
// Dump an avlvector in reverse order.

template <class T>
void avlvector<T>::dump_rev() const {
  for (int i = last(); i != -1; i = prev(i)) {
    std::cout << (*this)[i] << std::endl;
  }
}

// __________________________________________________________________________
// Consistency checks.

template <class T>
bool avlvector<T>::invariant() const {
  return familially_consistent() && balanced();
}

// __________________________________________________________________________
// Check that parents and children are consistent.

template <class T>
bool avlvector<T>::familially_consistent() const {
  for (typename std::vector<T>::size_type i = 0; i < this->size(); ++i) {
    const auto& p = parent[i];
    if (p != -1 &&
        static_cast<typename std::vector<T>::size_type>(left[p]) != i &&
        static_cast<typename std::vector<T>::size_type>(right[p]) != i)
      return false;

    const auto& l = left[i];
    if (l != -1 &&
        static_cast<typename std::vector<T>::size_type>(parent[l]) != i)
      return false;

    const auto& r = right[i];
    if (r != -1 &&
        static_cast<typename std::vector<T>::size_type>(parent[r]) != i)
      return false;
  }
  return true;
}

// __________________________________________________________________________
// Check the tree is properly recursively balanced.  This function is
// intended for debugging and is computationally expensive.

template <class T>
bool avlvector<T>::balanced(int ind) const {
  if (ind == -2)
    ind = root;

  if (ind == -1)
    return true;

  int left_depth = depth(left[ind]);
  int right_depth = depth(right[ind]);
  if (abs(left_depth - right_depth) > 1)
    return false;

  if (right_depth - left_depth + 1 != bal[ind])
    return false;

  return balanced(right[ind]) && balanced(left[ind]);
}

// __________________________________________________________________________
// Calculate the depth of an element given its index.

template <class T>
int avlvector<T>::depth(int ind) const {
  if (ind == -2)
    ind = root;

  if (ind == -1)
    return 0;

  int left_depth = 1 + depth(left[ind]);
  int right_depth = 1 + depth(right[ind]);

  return (left_depth > right_depth ? left_depth : right_depth);
}

// __________________________________________________________________________
// Calculate the index of the first (smallest) element in the tree.

template <class T>
int avlvector<T>::first() const {
  if (root == -1)
    return -1;

  int i = root;
  while (left[i] != -1)
    i = left[i];

  return i;
}

// __________________________________________________________________________
// Calculate the index of the last (largest) element in the tree.

template <class T>
int avlvector<T>::last() const {
  if (root == -1)
    return -1;

  int i = root;
  while (right[i] != -1)
    i = right[i];

  return i;
}

// __________________________________________________________________________
// Calculate the index of the next element in a tree.

template <class T>
int avlvector<T>::next(int ind) const {
  assert(ind >= 0);

  if (right[ind] != -1) {
    ind = right[ind];
    while (left[ind] != -1)
      ind = left[ind];
    return ind;
  }

  if (parent[ind] != -1) {
    while (parent[ind] != -1 && right[parent[ind]] == ind)
      ind = parent[ind];
    return parent[ind];
  }

  return -1;
}

// __________________________________________________________________________
// Calculate the index of the previous element in a tree.

template <class T>
int avlvector<T>::prev(int ind) const {
  assert(ind >= 0);

  if (left[ind] != -1) {
    ind = left[ind];
    while (right[ind] != -1)
      ind = right[ind];
    return ind;
  }

  if (parent[ind] != -1) {
    while (parent[ind] != -1 && left[parent[ind]] == ind)
      ind = parent[ind];
    return parent[ind];
  }

  return -1;
}

#endif  // AVLGRID_H_
