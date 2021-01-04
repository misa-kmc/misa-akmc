//
// Created by chu genshen on 2019/10/10.
//

#ifndef MISA_KMC_RING_ITERATOR_H
#define MISA_KMC_RING_ITERATOR_H

#include <array>
#include <iterator>

typedef unsigned int type_ring_index; // todo size_t of N
typedef type_ring_index type_ring_size;

/**
 * \brief ring iterator
 * \tparam T element type in ring
 * \tparam N the size of ring
 * \tparam Ref reference type of element in ring
 * \tparam Ptr pointer type of element in ring
 */
template <class T, type_ring_size N, class Ref, class Ptr> class RingIterator {
public:
  typedef RingIterator<T, N, T &, T *> iterator;
  typedef RingIterator<T, N, const T &, const T *> const_iterator;
  typedef RingIterator<T, N, Ref, Ptr> self;

  typedef std::forward_iterator_tag iterator_category;

  typedef T value_type;  // element value type
  typedef Ptr pointer;   // element pointer type
  typedef Ref reference; // element ref type

  typedef std::array<T, N> *link_type;
  typedef std::array<T, N> data_type;

  /**
   * iterator copy.
   */
  RingIterator(const iterator &x);

  RingIterator &operator=(const iterator &x);

  /**
   * \brief initialize iterator using ring dara.
   * set the iterator to the first element in ring.
   * \param ring_data pointer to data in ring.
   */
  explicit RingIterator(data_type *ring_data);

  /**
   * iterator equal is specified by current cursor.
   * @param x another iterator
   * @return true: equal, false: not equal.
   */
  bool operator==(const self &x) const { return cursor == x.cursor && p_ring_data == x.p_ring_data; }

  bool operator!=(const self &x) const { return !(*this == x); }

  /**
   * \brief return reference of current node in ring.
   * \return reference of current node.
   */
  reference operator*() { return (*p_ring_data)[cursor]; }

  /**
   * \brief return pointer of current node
   * \return pointer of current node
   */
  pointer operator->() { return &(operator*()); }

  /**
   * \brief move the cursor to next element and return its reference iterator.
   * \return self reference after moving to next element.
   */
  self &operator++() {
    cursor = (cursor + 1) % N;
    return *this;
  }

  /**
   * \brief return next element in ring without moving cursor to next element.
   * \return next element in ring.
   */
  value_type next() const;

public:
  data_type *p_ring_data;
  type_ring_index cursor;
};

#include "ring_iterator.inc"

#endif // MISA_KMC_RING_ITERATOR_H
