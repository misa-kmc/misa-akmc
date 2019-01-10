//
// Created by genshen on 2018-12-18.
//

#ifndef MISA_KMC_VACANCY_LIST_H
#define MISA_KMC_VACANCY_LIST_H


#include <array>
#include <map>
#include "type_define.h"
#include "rates.h"

class Vacancy {
public:
    /**
    * \brief transition rates of vacancy.
    */
    Rates<8> rates;
};

class VacancyList {
public:
    /**
     * \brief  a map to correspond the id and the Itl sequence number.
     */
    std::map<_type_lattice_id, Vacancy> mp;
};


#endif //MISA_KMC_VACANCY_LIST_H
