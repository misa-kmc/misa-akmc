//
// Created by chu genshen on 2019/10/10.
//

#include "ring_iterator.h"

template <class T, type_ring_size N, class Ref, class Ptr>
RingIterator<T, N, Ref, Ptr>::RingIterator(const RingIterator::iterator &x)
    : p_ring_data(x.p_ring_data), cursor(x.cursor) {}

template <class T, type_ring_size N, class Ref, class Ptr>
RingIterator<T, N, Ref, Ptr> &RingIterator<T, N, Ref, Ptr>::operator=(const RingIterator::iterator &x) {
  p_ring_data = x.p_ring_data;
  cursor = x.cursor;
  return *this;
}

template <class T, type_ring_size N, class Ref, class Ptr>
RingIterator<T, N, Ref, Ptr>::RingIterator(RingIterator::data_type *p_ring_data)
    : p_ring_data(p_ring_data), cursor(0) {}

template <class T, type_ring_size N, class Ref, class Ptr>
typename RingIterator<T, N, Ref, Ptr>::value_type RingIterator<T, N, Ref, Ptr>::next() const {
  return (*p_ring_data)[(cursor + 1) % N];
}
