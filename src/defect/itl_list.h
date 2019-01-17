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
     * \brief be called before calling updateRates.
     */
    void beforeRatesUpdate(Lattice *list_1nn[8], _type_neighbour_status status_1nn) override;

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
     * \return the available transition direction
     */
    _type_dirs_status availTranDirs(_type_neighbour_status nei_status,
                                    Lattice *_1nn_lats[8]) override;

    /**
     * \brief calculate the index of rates array by _1nn_id and trans_dirs.
     * for example, tran_dirs is 0b 01101010, _1nn_id is 5,
     * the 5th bit (staring from 0) in tran_dirs (0b 01101010 (from lower bit to higher bit)
     *                      5th bit in tran_dirs  _____^
     * is 2nd (start counting from 0) 'bit 1' in tran_dirs from lower bit to higher bit,
     * so, the rate index will be 2*2nd=4 (if up is false) or 2*2nd+1=5 (if up is true)
     *
     * \param _1nn_id  lattice id of 1nn neighbour, available values from 0 to 7.
     * \param trans_dirs the 4 available transition directions determined by interval orientation, which is recorded by 8-bits.
     * \param up rotate direction
     * \return the index of rates array
     */
    static int ratesIndex(_type_dir_id _1nn_id, _type_dirs_status trans_dirs, bool up);
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
