//
// Created by genshen on 2018-12-12.
//

#ifndef MISA_KMC_ITL_LIST_H
#define MISA_KMC_ITL_LIST_H

#include "../orientation.h"
#include "../rate/rates_types.h"
#include "defect.hpp"
#include "lattice/lattice.h"
#include "type_define.h"
#include <map>

class Itl : public Defect<8> {
public:
  orientation orient; // todo initialization

  Itl() = default;

  ~Itl() = default;

  /**
   * \brief be called before calling updateRates.
   */
  void beforeRatesUpdate(Lattice *list_1nn[LatticesList::MAX_1NN], _type_neighbour_status status_1nn) override;

  /**
   * \brief update transition rates of this interstitial lattice.
   * \param lattice the lattice of current itl.
   * \param list_1nn 1nn lattices of this lattice.
   * \param status_1nn 1nn status of this lattice.
   * \param callback callback function to get transition rate.
   */
  void updateRates(Lattice &lattice, Lattice *list_1nn[LatticesList::MAX_1NN], _type_neighbour_status status_1nn,
                   rateCallback callback) override;

  /**
   * \brief get status of array rates.
   * If a rate exists for some transition direction and rotate in array rates,
   * then the bit flag in return status will be set to 1, otherwise bit flag will be set to 0.
   * \return rates status.
   */
  _type_rates_status getRatesStatus();

  /**
   * \brief calculate the index of rates array by _1nn_id and trans_dirs.
   * for example, tran_dirs is 0b 01101010, _1nn_id is 5,
   * the 5th bit (staring from 0) in tran_dirs (0b 01101010 (from lower bit to higher bit)
   *                      5th bit in tran_dirs  -----^
   * is 2nd (start counting from 0) 'bit 1' in tran_dirs from lower bit to higher bit,
   * so, the rate index will be 2*2nd=4 (if up is false) or 2*2nd+1=5 (if up is true)
   *
   * \param _1nn_id  lattice id of 1nn neighbour, available values from 0 to 7.
   * \param trans_dirs the 4 available transition directions determined by interval orientation, which is recorded by
   * 8-bits. \param up rotate direction \return the index of rates array
   */
  static int ratesIndex(_type_dir_id _1nn_id, _type_dirs_status trans_dirs, bool up);

  /**
   * \brief given a index of rates array, it returns the 1nn neighbour id of current lattice.
   *
   * for example, tran_dirs is 0b 01101010, rate_index is 4,
   * this function will return 5, which points to:
   * 0b 01101010
   *      ^----- 5th bit from right to left starting from 0.
   *
   * \param rate_index index of rates array from 0 to 7.
   * \param trans_dirs the 4 available transition directions determined by interval orientation,
   * which is recorded by 8-bits.
   * \return the 1nn neighbour id of current lattice for @param rate_index
   */
  static _type_dir_id get1nnIdByRatesIndex(int rate_index, _type_dirs_status trans_dirs);

protected:
  /**
   * \brief calculate available transition direction based on the 1nn nearest neighbour lattices
   *
   * basic rules:
   * 1. If the target is not single atom, it cannot "jump to";
   * 2. If the target is not available, it cannot "jump to";
   * 3. It must be in the 4 transition directions determined by source lattice orientation.
   *
   * \param nei_status the status of 1nn neighbour lattices
   * \param _1nn_lats pointer of 1nn neighbour lattices
   * \return the available transition directions
   */
  _type_dirs_status availTranDirs(_type_neighbour_status nei_status,
                                  Lattice *_1nn_lats[LatticesList::MAX_1NN]) override;
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

  /**
   * \brief replace an existed old interstitial with new interstitial.
   *  remove the old one and add the new one.
   * \param old_lat_id lattice id of old interstitial.
   * \param new_lat_id lattice id of new interstitial.
   * \param p_meta metadata of lattice list
   * \param new_itl reference of new interstitial.
   */
  void replace(const _type_lattice_id old_lat_id, const _type_lattice_id new_lat_id, const LatListMeta *p_meta,
               const Itl &new_itl);
};

#endif // MISA_KMC_ITL_LIST_H
