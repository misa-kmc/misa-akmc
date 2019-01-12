//
// Created by genshen on 2018-12-12.
//

#ifndef MISA_KMC_ITL_LIST_H
#define MISA_KMC_ITL_LIST_H


#include <map>
#include "type_define.h"
#include "lattice/lattice.h"
#include "direction.h"
#include "defect.hpp"


class Itl : public Defect<8> {
public:

    tran_dir direction; // todo initialization

    Itl() {};

    ~Itl() {};

    void updateAvailTranDir(_type_neighbour_status nei_status,
                            Lattice *_1nn_lats[8]) override;
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
