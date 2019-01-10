//
// Created by genshen on 2018-12-12.
//

#ifndef MISA_KMC_ITL_LIST_H
#define MISA_KMC_ITL_LIST_H


#include <map>
#include "type_define.h"
#include "lattice.h"


class Itl {
public:
    Itl() {};

    ~Itl();

    /**
     * \brief transition rates of vacancy.
     */
    _type_rate rate;

    int first, second;

    /*direction*/
    int dir1, dir2, dir3, dir4;
};

class ItlList {
public:
    // a map to correspond the id and the Itl object.
    std::map<_type_lattice_id, Itl> mp;

    /*!
     * \deprecated
     * \brief to find the sequence number in class Ilt
     *        which include the extra information about Itl type todo spell? atoms
     * \param id use the LatticeList getId
     * \return the sequence number in in Itl
     */
    Itl getItlnum(_type_lattice_id id);
};


#endif //MISA_KMC_ITL_LIST_H
