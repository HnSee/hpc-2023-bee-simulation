#ifndef BEESIMULATION_UTILS_MULTIKEY_MATRIX_H
#define BEESIMULATION_UTILS_MULTIKEY_MATRIX_H

struct MultiKeyIndex {
  double x;
  double y;
};

template <typename T> class MultiKeyMatrix {
public:
  virtual T *get(const MultiKeyIndex index) = 0;
  virtual void set(const MultiKeyIndex index, T value) = 0;
};

#endif