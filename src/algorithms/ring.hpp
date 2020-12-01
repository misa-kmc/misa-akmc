//
// Created by genshen on 2019/9/21.
//

#ifndef MISA_KMC_RING_HPP
#define MISA_KMC_RING_HPP

#include "ring_iterator.h"
#include <array>
#include <vector>

/**
 * \brief  fixed size ring.
 * \tparam T type of element in ring.
 * \tparam N the size of ring.
 */
template <typename T, type_ring_size N> class ring {
  static_assert(N >= 1, "ring size can not be zero.");

  friend class iterator;

public:
  typedef RingIterator<T, N, T &, T *> iterator;

  explicit ring(const std::array<T, N> _data);

  /**
   * \return size of ring
   */
  inline size_t size() const { return N; }

  iterator begin();

private:
  std::array<T, N> data;
};

template <typename T, type_ring_size N> ring<T, N>::ring(const std::array<T, N> _data) : data(_data) {}

template <typename T, type_ring_size N> typename ring<T, N>::iterator ring<T, N>::begin() {
  return ring::iterator(&data);
}

#endif // MISA_KMC_RING_HPP
