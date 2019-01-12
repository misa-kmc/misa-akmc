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
    void updateAvailTranDir(_type_neighbour_status nei_status,
                            Lattice *_1nn_lats[8]) override;
};

class VacancyList {
public:
    /**
     * \brief  a map to correspond the id and the Itl sequence number.
     */
    std::map<_type_lattice_id, Vacancy> mp;
};


#endif //MISA_KMC_VACANCY_LIST_H
