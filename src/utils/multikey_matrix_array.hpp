#ifndef BEESIMULATION_UTILS_MULTIKEY_MATRIX_ARRAY_H
#define BEESIMULATION_UTILS_MULTIKEY_MATRIX_ARRAY_H

#include "multikey_matrix.hpp"

// Actual implementation
bool sortby_x(const MultiKeyIndex a, const MultiKeyIndex b) {
  return a.x < b.x;
}

bool sortby_y(const MultiKeyIndex a, const MultiKeyIndex b) {
  return a.y < b.y;
}

int findelement(int xtofind, int ytofind, int left, int right,
                MultiKeyIndex *res) {
  if (right - left == 1) {
    if (xtofind == res[left].x && ytofind == res[left].y) {
      return left;
    }
    return -1;
  }

  int middle = (right + left) / 2;

  if (xtofind < res[middle].x) {
    return findelement(xtofind, ytofind, left, middle, res);
  }

  if (xtofind > res[middle].x) {
    return findelement(xtofind, ytofind, middle, right, res);
  }

  if (ytofind < res[middle].y) {
    return findelement(xtofind, ytofind, left, middle, res);
  }

  if (ytofind > res[middle].y) {
    return findelement(xtofind, ytofind, middle, right, res);
  }

  return middle;
}
// Actual implementation end

template <typename T> class MultiKeyMatrixArray : public MultiKeyMatrix<T> {
public:
  MultiKeyMatrixArray(std::size_t size) { this->data = new T[size]; }

  ~MultiKeyMatrixArray() { delete[] this->data; }

  T *get(const MultiKeyIndex index) {
    int indexOfElement =
        findelement(index.x, index.y, 0, this->size, this->data);

    if (indexOfElement == -1) {
      return nullptr;
    } else {
      return this->data[indexOfElement];
    }
  }

  void set(const MultiKeyIndex index, T value) {
    int indexOfElement =
        findelement(index.x, index.y, 0, this->size, this->data);

    this->data[indexOfElement] = value;
  }

private:
  std::size_t size;
  T *data;
};

#endif
