//
// Created by genshen on 2018-12-18.
//

#ifndef MISA_KMC_VACANCY_LIST_H
#define MISA_KMC_VACANCY_LIST_H

#include <array>
#include <comm/domain/region.hpp>
#include <comm/types_define.h>
#include <map>

#include "defect.hpp"
#include "type_define.h"

class Vacancy : public Defect<8> {
public:
  /**
   * \brief calculate available transition direction based on neighbour lattice's status and types.
   */
  inline _type_neighbour_status availTranDirs(_type_neighbour_status nei_status,
                                              Lattice *_1nn_lats[LatticesList::MAX_1NN]) override;

  /**
   * \brief be called before calling updateRates.
   */
  void beforeRatesUpdate(Lattice *list_1nn[LatticesList::MAX_1NN], _type_neighbour_status status_1nn) override;

  /**
   * \brief update transition rate for each possible 1nn neighbour lattices of this vacancy.
   *
   * \param lattice the lattice reference of current defect.
   * \param list_1nn 1nn lattices of this lattice.
   * \param status_1nn 1nn status of this lattice.
   * \param callback callback function to get transition rate.
   */
  void updateRates(Lattice &lattice, Lattice *list_1nn[LatticesList::MAX_1NN], _type_neighbour_status status_1nn,
                   rateCallback callback) override;
};

class VacancyList {
public:
  /**
   * \brief  a map to correspond the id and the Itl sequence number.
   */
  std::map<_type_lattice_id, Vacancy> mp;

  /**
   * \brief replace an existed old vacancy with new vacancy.
   *  remove the old one and add the new one.
   * \param old_lat_id the lattice id of the old vacancy
   * \param new_lat_id the lattice id of the new vacancy
   *  \param p_meta metadata of lattice list
   */
  void replace(const _type_lattice_id old_lat_id, const _type_lattice_id new_lat_id, const LatListMeta *p_meta);

  /**
   * \brief reindex the vacancy list in a given region.
   * \param lats lattice list
   * \param region region
   */
  void reindex(LatticesList *lats, const comm::Region<comm::_type_lattice_size> region);
};

#endif // MISA_KMC_VACANCY_LIST_H
