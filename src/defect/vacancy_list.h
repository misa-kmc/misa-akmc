//
// Created by genshen on 2018-12-18.
//

#ifndef MISA_KMC_VACANCY_LIST_H
#define MISA_KMC_VACANCY_LIST_H


#include <array>
#include <map>
#include "type_define.h"
#include "defect.hpp"

class Vacancy : public Defect<8> {
public:
    /**
     * \brief calculate available transition direction based on neighbour lattice's status and types.
     */
    _type_neighbour_status availTranDirs(_type_neighbour_status nei_status,
                                         Lattice *_1nn_lats[8]) override;

    /**
     * \brief be called before calling updateRates.
     */
    void beforeRatesUpdate(Lattice *list_1nn[8], _type_neighbour_status status_1nn) override;

    /**
     * \brief update transition rate for each possible 1nn neighbour lattices of this vacancy.
     * \param list_1nn 1nn lattices of this lattice.
     * \param status_1nn 1nn status of this lattice.
     */
    void updateRates(Lattice *list_1nn[8], _type_neighbour_status status_1nn,
                     rateCallback callback) override;
};

class VacancyList {
public:
    /**
     * \brief  a map to correspond the id and the Itl sequence number.
     */
    std::map<_type_lattice_id, Vacancy> mp;
};


#endif //MISA_KMC_VACANCY_LIST_H
