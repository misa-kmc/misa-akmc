//
// Created by genshen on 2018-12-12.
//

#ifndef MISA_KMC_ITL_LIST_H
#define MISA_KMC_ITL_LIST_H


#include <map>
#include "type_define.h"
#include "lattice/lattice.h"
#include "../direction.h"
#include "defect.hpp"


class Itl : public Defect<8> {
public:

    tran_dir direction; // todo initialization

    Itl() {};

    ~Itl() {};

    /**
     * \brief update transition rates of this interstitial lattice.
     * \param list_1nn 1nn lattices of this lattice.
     * \param status_1nn 1nn status of this lattice.
     */
    void updateRates(Lattice *list_1nn[8], _type_neighbour_status status_1nn) override;

protected:
    /**
     * \brief calculate available transition direction based on the 1nn nearest neighbour lattices
     *
     * basic rules:
     * 1. If the target is not single atom, it cannot "jump to";
     * 2. If the target is not available, it cannot "jump to";
     * 3. It must be in the 4 transition directions determined by source lattice direction.
     *
     * \param nei_status the status of 1nn neighbour lattices
     * \param _1nn_lats pointer of 1nn neighbour lattices
     * \return the updated transition direction
     */
    _type_dirs_status updateAvailTranDir(_type_neighbour_status nei_status,
                                         Lattice *_1nn_lats[8]) override;

    /**
     * \brief set transition rate for the direction whose 1nn neighbour id is @param dir_id.
     *
     * find rates array index by parameter dir_id, and set rate to the array element.
     * for example: avail_trans_dir is 0b00110101, and dir_id is 4,
     *                      dir_id:  4 -----^
     * the it will set value for rates[3].
     *
     * \param rate transition rate
     * \param dir_id 1nn neighbour lattice id, available values from 0 to 7.
     */
    void setRate(double rate, _type_dir_id dir_id);
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
