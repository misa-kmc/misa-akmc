//
// Created by genshen on 2019/10/6.
//

#ifndef MISA_KMC_CREATION_H
#define MISA_KMC_CREATION_H

#include <comm/domain/colored_domain.h>
#include "abvi/defect/vacancy_list.h"
#include "lattice/lattices_list.h"


class creation {
public:
    /**
     * \brief create lattices property randomly
     * \param lats pointer of lattice list
     * \param va_list the vacancy defects list.
     * \param types lattice types list to be created
     * \param types_ratio ratio of each lattice types.
     * \param va_count count of vacancy
     * \param p_domain pointer of domain.
     */
    static void createRandom(LatticesList *lats, VacancyList *va_list,
                             const std::vector<LatticeTypes::lat_type> types,
                             const std::vector<unsigned int> types_ratio, const unsigned long va_count,
                             const comm::ColoredDomain *p_domain);
};

#endif //MISA_KMC_CREATION_H
