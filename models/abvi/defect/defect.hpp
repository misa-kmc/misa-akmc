//
// Created by genshen on 2019-01-12.
//

#ifndef MISA_KMC_DEFECT_JLIST_H
#define MISA_KMC_DEFECT_JLIST_H

#include "../orientation.h"
#include "../rate/rates_types.h"
#include "def_types.h"
#include "lattice/lattices_list.h"
#include "type_define.h"
#include <array>

/**
 * In KMC defect includes interval(or calling dumbbell) and vacancy.
 * This class is the base class of interval defect and vacancy defect
 */
template <unsigned int SIZE> class Defect {
public:
  /**
   * \brief the bit length of \var variable avail_trans_dir.
   * A lattice can jump to its 8 1nn neighbour lattices.
   */
  static const int TRANS_DIRS_BITS_SIZE = 8;

  /**
   * \brief size of transition rates array.
   */
  static const int RATES_SIZE = SIZE;

  /**
   * \brief the transition rates of lattice in each available transition direction.
   * If the transition rate is not available for some direction,
   * it will leave it the array element untouched.
   */
  std::array<_type_rate, SIZE> rates;

  /**
   * \brief available transition direction of the defect.
   */
  _type_avail_trans_dir avail_trans_dir;

  /**
   * \brief this method will be call before calling updateRates.
   * by default(in this base class), this method would clear rates array
   * (set each element's value in rates array to 0.0).
   *
   * \param list_1nn 1nn lattices of this lattice.
   * \param status_1nn 1nn status of this lattice.
   */
  virtual void beforeRatesUpdate(Lattice *list_1nn[LatticesList::MAX_1NN], _type_neighbour_status status_1nn);

  /**
   * \brief update transition rates to each direction of this lattice.
   * the rates calculating will be done by calling function pointer @param callback.
   *
   * \param lattice the lattice reference of current defect.
   * \param list_1nn 1nn lattices of this lattice.
   * \param status_1nn 1nn status of this lattice.
   * \param callback callback function to get transition rate.
   */
  virtual void updateRates(Lattice &lattice, Lattice *list_1nn[LatticesList::MAX_1NN],
                           _type_neighbour_status status_1nn, rateCallback callback) = 0;

  /**
   * \brief calculate available transition direction based on neighbour lattice's status and types.
   *
   * \param nei_status the status of 1nn neighbour lattices
   * \param _1nn_lats pointer of 1nn neighbour lattices
   * \return the available transition direction
   */
  virtual _type_dirs_status availTranDirs(_type_neighbour_status nei_status, Lattice **_1nn_lats) = 0;
};

template <unsigned int SIZE>
void Defect<SIZE>::beforeRatesUpdate(Lattice *list_1nn[LatticesList::MAX_1NN], _type_neighbour_status status_1nn) {
  // zero rates array
  // todo set zero using memset function
  for (_type_rate &rate : rates) {
    rate = 0;
  }
}

#endif // MISA_KMC_DEFECT_JLIST_H
