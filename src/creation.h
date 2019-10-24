//
// Created by genshen on 2019/10/6.
//

#ifndef MISA_KMC_CREATION_H
#define MISA_KMC_CREATION_H

#include <comm/domain/colored_domain.h>
#include "lattice/lattices_list.h"


class creation {
public:
    /**
     * \brief create lattices property randomly
     * \param lats pointer of lattice list
     * \param types lattice types list to be created
     * \param types_ratio ratio of each lattice types.
     * \param va_count count of vacancy
     * \param p_domain pointer of domain.
     */
    static void createRandom(LatticesList *lats, const std::vector<LatticeTypes::lat_type> types,
                             const std::vector<unsigned int> types_ratio, const unsigned long va_count,
                             const comm::ColoredDomain *p_domain);
};

#endif //MISA_KMC_CREATION_H
