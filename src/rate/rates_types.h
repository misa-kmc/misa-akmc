//
// Created by genshen on 2019-01-19.
//

#ifndef MISA_KMC_RATES_TYPES_H
#define MISA_KMC_RATES_TYPES_H

#include "type_define.h"
#include "../orientation.h"

const double BoltzmannConstant = 1.3806485279; // unit: 10^{-23} J/K

typedef std::function<_type_rate(const _type_dir_id _1nn_id)> rateCallback;
//typedef _type_rate(*rateCallback)(_type_dir_id _1nn_id); // A lambda can only be converted to a function pointer if it does not capture

#endif //MISA_KMC_RATES_TYPES_H
