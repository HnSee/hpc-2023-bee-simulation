#ifndef BEESIMULATION_UTILS_CTD_ARRAY_H
#define BEESIMULATION_UTILS_CTD_ARRAY_H

#include <cstddef>
#include <memory>
#include <utility>

/// @brief Contigous, two dimensional (CTD) array
/// @tparam T Inner type of the array.
template <typename T> class CTDArray {
public:
  CTDArray() = default;
  CTDArray(std::size_t rows, std::size_t columns) {
    this->allocate(rows, columns);
  }

  void allocate(std::size_t rows, std::size_t columns) {
    this->data = std::make_unique<T[]>(rows * columns);

    this->rs = rows;
    this->cs = columns;
  }

  void updateDimensions(std::size_t rows, std::size_t columns) {
    this->rs = rows;
    this->cs = columns;
  }

  std::pair<std::size_t, std::size_t> dimensions() const {
    return std::make_pair(this->rs, this->cs);
  }

  std::size_t size() const { return sizeof(T) * this->rs * this->cs; }
  std::size_t count() const { return this->rs * this->cs; }

  T *operator[](std::size_t row) { return row * this->cs + this->data.get(); }

  T *get() { return this->data.get(); }

private:
  std::unique_ptr<T[]> data;
  std::size_t rs = 0;
  std::size_t cs = 0;
};

#endif
