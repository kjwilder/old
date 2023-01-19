## Obsolete somewhat historically interesting C++ code.
- I wrote these libraries circa 1996 but mostly switched to using STL
  implementations in the early 2000's.
- I wrote these libraries after g++ supported templates but before it had
  a good and widely available STL implementation.
- The initial commit contains exact copies of my working code from the 1990's.
  It will not compile with a modern C++ compiler.
- Later commits include tests and updates to make the code compile with newer
  compilers.

### About `avlgrid.h`
- The standard library `set` provides equivalent and additional functionality
  with better performance.
- This implementation was designed for a specific purpose: Insert and store a
  large list of items. The items are kept sorted, and insertions are O(log(n)).
- The main function is `insert`, which will insert a new item iff it is not
  equal to any item previously inserted.
