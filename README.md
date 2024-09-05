[![CMake on multiple platforms](https://github.com/muazsh/SafeCPP/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/muazsh/SafeCPP/actions/workflows/cmake-multi-platform.yml)
# Safe C++
C++ is unsafe by design, and unless it is subsetted or wrapped by some means, softwares developed in C++ will mostly be vulnerable, for that in introduce `SafeCPP.h`:

## `SafeCPP.h`
This header provides:

### Compile-Time Safety 
Preventing the usage of some known unsafe functions and operators like the ones which make no bound-checks.

### `safe_iterator`
A generic container iterator which is:
- Thread safe.
- Out-of-Bounds access safe.
- Invalid container safe.

### `safe_vector`
An std::vector wrapper which is:
- Thread safe.
- Out-of-Bounds access safe.
- Safe against elements invalidation when inserting.
- Uses `safe_iterator`.

## Limitations
The compile-time prevention includes the ones I am aware so far, also the usage of other variations which make bound-checks has a performance overhead, also `safe_iterator` and `safe_vector` have performance overhead obviously. However, to secure C++ such acts are unavoidable, also **due to hardware development nowadays such performance overhead should be neglectable for most usages**.

## Recommendations
- Do not use raw pointers or allocate memory manually, always use a proper smart pointer/container instead.
- Do not expose a smart pointer's raw pointer.
- Do not use language functions which operate on buffers and make no bound-checks.
- Do not use pointer/iterator arithmetic.
- Do not store a container iterator.
- Do not use C-style array.
- Do not use [] operator to access a container element, it makes no bound check.
- Do not assign an rvalue to an std::string_view.
- Always intialize variables.
- Always consider passing arguments by const&.
- Do not store references.
- Be aware when lambda caputring by refernece, they are reference states internally.
- Do not use C-Style cast, const_cast or reinterpret_cast.
- Always check for nullptr after dynamic_cast.
- Be aware when using static_cast to down cast.
- Use ranges and range-based loops instead of raw loops whenever possible. 
