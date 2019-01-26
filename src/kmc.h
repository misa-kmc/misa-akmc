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
    double random();

    /**
     * \brief calculate the transition rates of each lattice.
     *
     * The calculating methods depends on the lattice type(single atom, vacancy and dumbbell).
     * Different types of lattice have different methods or formulas to calculate the rate.
     * see the implementation for more details.
     *
     * After this step, the rate of every transition direction of each lattice will be set.
     * \return return the sum of rates of all KMC events,
     * including dumbbell transition and vacancy transition and defect generation).
     */
    _type_rate updateRates(double v, double T);


    /**
     * \brief select an event randomly and execute this event.
     *
     * \param random random number.
     */
    void execute(double random);

protected:
    double time = 0;

private:
    Box *box = nullptr; // todo init box pointer
};


#endif //MISA_KMC_KMC_H
