//
// Created by genshen on 2019/9/21.
//

#ifndef MISA_KMC_RING_HPP
#define MISA_KMC_RING_HPP

#include <vector>

/**
 * \brief  fixed size ring.
 * \tparam T type of element in ring.
 * \tparam N the size of ring.
 */
template<typename T, unsigned int N> // todo size_t
class ring {
    static_assert(N >= 1, "ring size can not be zero.");

public:
    explicit ring(const std::array<T, N> _data);

    /**
     * \brief move the cursor to next element and return its reference after moving to next element.
     * \return self reference after moving to next element.
     */
    ring<T, N> &operator++();

    /**
     * \brief move the cursor to next element and return next element in ring.
     * \return next element in ring.
     */
    T next();

    /**
     * \brief return current element.
     * \return current element.
     */
    inline T operator()() {
        return data[cursor];
    }

    /**
     * \brief get current element.
     * \return current element.
     */
    inline T get() const {
        return data[cursor];
    }

    /**
     * \return size of ring
     */
    inline size_t size() const {
        return N;
    }

private:
    const std::array<T, N> data;
    unsigned long cursor = 0;
};

template<typename T, unsigned int N>
ring<T, N>::ring(const std::array<T, N> _data):data(_data) {}

template<typename T, unsigned int N>
ring<T, N> &ring<T, N>::operator++() {
    cursor = (cursor + 1) % N;
    return *this;
}

template<typename T, unsigned int N>
T ring<T, N>::next() {
    cursor = (cursor + 1) % N;
    return data[cursor];
}

#endif //MISA_KMC_RING_HPP
