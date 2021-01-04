//
// Created by genshen on 2019-01-29.
//

#ifndef MISA_KMC_ENERGY_DUMBBELL_H
#define MISA_KMC_ENERGY_DUMBBELL_H

#include "../../defect/itl_list.h"
#include "lattice/lattice_types.h"
#include "lattice/lattices_list.h"
#include "type_define.h"

namespace bond {
  // todo tests
  /**
   * \brief calculate energy of dumbbell.
   * \return dumbbell energy.
   */
  double Edumb(LatticesList &lattice_list, ItlList &itl_list);

  /** // todo tests
   * \brief calculate compsol of dumbbell of FeX type (X can be Cu,Ni,Mn)
   * \param id id of lattice
   * \param type type of dumbbell lattice
   * \param itl the inter/dumbbell instance.
   * \param _1nns the 1nn neighbour lattices list.
   * \param _2nns the 2nn neighbour lattices list.
   * \param _1nn_status lattice status of 1nn neighbour lattices
   * \param _2nn_status lattice status of 2nn neighbour lattices
   * \return
   */
  double FeX_comp(const _type_lattice_id id, const LatticeTypes type, const Itl &itl,
                  Lattice *_1nns[LatticesList::MAX_1NN], Lattice *_2nns[LatticesList::MAX_2NN],
                  const _type_neighbour_status _1nn_status, const _type_neighbour_status _2nn_status);
} // namespace bond

#endif // MISA_KMC_ENERGY_DUMBBELL_H
