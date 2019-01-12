//
// Created by genshen on 2019-01-12.
//

#ifndef MISA_KMC_DEFECT_JLIST_H
#define MISA_KMC_DEFECT_JLIST_H

#include <array>
#include "../lattice/lattices_list.h"
#include "../type_define.h"
#include "def_types.h"

/**
 * In KMC defect includes interval(or calling dumbbell) and vacancy.
 * This class is the base class of interval defect and vacancy defect
 */
template<unsigned int SIZE>
class Defect {
public:
    /**
     * \brief the transition rates of lattice in each available transition direction.
     */
    std::array<_type_rate, SIZE> rates;

    /**
     * \brief available transition direction of the defect.
     */
    _type_avail_trans_dir avail_trans_dir;

    /**
     * \brief calculate available transition direction based on neighbour lattice's status and types.
     */
    virtual void updateAvailTranDir(_type_neighbour_status nei_status,
                                    Lattice *_1nn_lats[8]) = 0;
};


#endif //MISA_KMC_DEFECT_JLIST_H
