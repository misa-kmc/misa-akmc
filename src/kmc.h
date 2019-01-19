//
// Created by genshen on 2018-12-12.
//

#ifndef MISA_KMC_KMC_H
#define MISA_KMC_KMC_H

#include "type_define.h"
#include "box.h"

/*!
 * \brief the main routine of KMC simulation.
 */
class kmc {
public:
    /*!
     * \brief
     * \return
     */
    _type_rate random();

    /**
     * \brief calculate the transition rates of each lattice.
     *
     * The calculating methods depends on the lattice type(single atom, vacancy and dumbbell).
     * Different types of lattice have different methods or formulas to calculate the rate.
     * see the implementation for more details.
     *
     * After this step, the rate of every transition direction of each lattice will be set.
     */
    void updateRates(double v, double T);

private:
    Box *box; // todo init box pointer
};


#endif //MISA_KMC_KMC_H
