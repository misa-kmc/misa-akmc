//
// Created by genshen on 2018-12-12.
//

#ifndef MISA_KMC_ITL_LIST_H
#define MISA_KMC_ITL_LIST_H


#include <map>
#include "type_define.h"
#include "atom.h"

class ItlList {
public:
    // a map to correspond the id and the Itl sequence number.
    std::map<_type_atom_id, Itl> mp;

    /*!
     * \brief to find the sequence number in class Ilt
     *        which include the extra information about Itl type atoms
     * \param id use the AtomList getId
     * \return the sequence number in in Itl
     */
    Itl getItlnum(_type_atom_coord id);
};


#endif //MISA_KMC_ITL_LIST_H
