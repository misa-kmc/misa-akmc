//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_RATES_H
#define MISA_KMC_RATES_H

#include <array>
#include "type_define.h"

/**
 * \brief the transition rates of lattice in each direction
 */
template<unsigned int RATE_SIZE>
class Rates {
public:
    std::array<_type_rate, RATE_SIZE> r;

};


#endif //MISA_KMC_RATES_H
