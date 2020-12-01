//
// Created by genshen on 2019-01-30.
//

#ifndef MISA_KMC_RECOMBINE_H
#define MISA_KMC_RECOMBINE_H

/**
 * \brief recombination between an interstitial and a vacancy takes place
 *  as soon as they become second nearest neighbors.
 *  we will recombine them(a vacancy and an interstitial) into two single atoms.
 */
#include "defect/itl_list.h"
#include "defect/vacancy_list.h"
#include "lattice/lattice.h"
#include "lattice/lattice_types.h"
#include "lattice/lattices_list.h"
#include "type_define.h"

namespace rec {
  /**
   * \brief we recombine a vacancy and an interstitial to two single atoms.
   * This struct describes the information of two single atoms after recombination.
   */
  struct Rec {
    Rec(const _type_lattice_id id_vac, _type_lattice_id id_inter, LatticeTypes type_vac, LatticeTypes type_inter,
        double delta_e)
        : lat_vac(id_vac), lat_inter(id_inter), tp_vac(type_vac), tp_inter(type_inter), delta_energy(delta_e) {}

    /**
     * \brief lattice id of vacancy and interstitial to be recombined.
     */
    _type_lattice_id lat_vac, lat_inter;
    /**
     * \brief lattice type of vacancy and interstitial to be recombined.
     */
    LatticeTypes tp_vac, tp_inter;
    /**
     * \brief system energy difference of this kind of recombination.
     */
    double delta_energy = 0;

    /**
     * \brief recombine the vacancy and the interstitial.
     * \param lat_list lattice list
     * \param itl_list interstitial list.
     */
    void recombine(LatticesList *lat_list, VacancyList *vac_list, ItlList *itl_list);
  };

  /**
   * \brief different combination of Vacancy-Interstitial recombination
   * will be saved into this list.
   * For a vacancy V, consider its 1nn as well as its 2nn lattices.
   * If there are some dumbbell (denote as set D) in 1nn and 2nn, we will calculating
   * the energy of each possible Vacancy-Interstitial recombination.
   * It is also similar for dumbbell.
   *
   * For example, this is a vacancy V, there are 2 dumbbell D1(type FeCu) and D2(type CuNi) in its 1nn and 2nn lattices.
   * the possible Vacancy-Interstitial recombination(s) are:
   * V <-> D1 => Fe <-> Cu
   * V <-> D1 => Cu <-> Fe
   * V <-> D2 => Cu <-> Ni
   * V <-> D2 => Ni <-> Cu
   * In this class, we will record corresponding system energy of those 4 possible combinations.
   *
   */
  struct RecList {
    /**
     * \brief the list of different combination of Vacancy-Interstitial recombination.
     */
    std::vector<Rec> rec_list;

    /**
     * \brief find a rec whose system energy is the minimum in list.
     * \return the rec with minimum system energy.
     */
    Rec pickMinimum();

    /**
     * \brief create recombination list.
     * \param lat_list lattice list.
     * \param itl_list interstitial list.
     * \param id id of the center lattice (we will traversing 1nn and 2nn lattices of this lattice).
     * \param is_vac be true if the lattice specified by \param id is a vacancy.
     */
    void create(LatticesList *lat_list, ItlList *itl_list, const _type_lattice_id id);

  private:
    /**
     * \brief calculate the energy of recombination of \param lac_vac and \param lat_inter.
     * and record the energy and the corresponding combination to \memberof rec_list.
     * \param lat_list lattice list
     * \param itl_list interstitial list
     * \param lat_vac pointer of a vacancy
     * \param lat_inter pointer of a inter
     */
    void stageCombination(LatticesList *lat_list, ItlList *itl_list, Lattice *lat_vac, Lattice *lat_inter);
  };

}; // namespace rec

#endif // MISA_KMC_RECOMBINE_H
