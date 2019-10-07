//
// Created by genshen on 2019/10/7.
//

#ifndef MISA_KMC_PARALLEL_INTERFACE_H
#define MISA_KMC_PARALLEL_INTERFACE_H

#include <domain/region.hpp>

/**
 * \brief class ParallelInterface is a universe interface for integrating
 * many parallel kmc algorithms.
 */
class ParallelInterface {
public:
    /**
     * \brief start kmc time loop.
     */
    virtual void startTimeLoop() = 0;

    /**
     * \brief calculate rates in a region
     * \return the sum of all rates.
     */
    virtual double calcRates(const comm::Region<comm::_type_lattice_size> region) = 0;

    /**
     * \brief select a event from rates list
     */
    virtual void selectRate() = 0;

    /**
     * \brief perform the kmv event.
     */
    virtual void perform() = 0;

protected:
    const double time_limit = 0.0;
};

#endif //MISA_KMC_PARALLEL_INTERFACE_H
