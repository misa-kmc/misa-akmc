//
// Created by genshen on 2019/10/6.
//

#ifndef MISA_KMC_CREATION_H
#define MISA_KMC_CREATION_H

#include "lattice/lattices_list.h"
#include <domain/colored_domain.h>


class creation {
public:
    static void create(LatticesList *lats, const comm::ColoredDomain *p_domain);
};


#endif //MISA_KMC_CREATION_H
