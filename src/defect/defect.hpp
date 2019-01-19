//
// Created by genshen on 2019-01-12.
//

#ifndef MISA_KMC_DEFECT_JLIST_H
#define MISA_KMC_DEFECT_JLIST_H

#include <array>
#include "../lattice/lattices_list.h"
#include "../type_define.h"
#include "../orientation.h"
#include "def_types.h"
#include "rate/rates_types.h"

/**
 * In KMC defect includes interval(or calling dumbbell) and vacancy.
 * This class is the base class of interval defect and vacancy defect
 */
template<unsigned int SIZE>
class Defect {
public:
    /**
     * \brief the transition rates of lattice in each available transition direction.
     * If the transition rate is not available for some direction,
     * it will leave it the array element untouched.
     */
    std::array<_type_rate, SIZE> rates;

    /**
     * \brief available transition direction of the defect.
     */
    _type_avail_trans_dir avail_trans_dir;

    /**
     * \brief this method will be call before calling updateRates.
     * \param list_1nn 1nn lattices of this lattice.
     * \param status_1nn 1nn status of this lattice.
     */
    virtual void beforeRatesUpdate(Lattice *list_1nn[8], _type_neighbour_status status_1nn);

    /**
     * \brief update transition rates to each direction of this lattice.
     * the rates calculating will be done by calling function pointer @param callback.
     * \param list_1nn 1nn lattices of this lattice.
     * \param status_1nn 1nn status of this lattice.
     */
    virtual void updateRates(Lattice *list_1nn[8], _type_neighbour_status status_1nn,
                             rateCallback callback) = 0;

    /**
     * \brief calculate available transition direction based on neighbour lattice's status and types.
     *
     * \param nei_status the status of 1nn neighbour lattices
     * \param _1nn_lats pointer of 1nn neighbour lattices
     * \return the available transition direction
     */
    virtual _type_dirs_status availTranDirs(_type_neighbour_status nei_status,
                                            Lattice **_1nn_lats) = 0;
};

template<unsigned int SIZE>
void Defect<SIZE>::beforeRatesUpdate(Lattice *list_1nn[8], _type_neighbour_status status_1nn) {}


#endif //MISA_KMC_DEFECT_JLIST_H
