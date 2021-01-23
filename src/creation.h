//
// Created by genshen on 2019/10/6.
//

#ifndef MISA_KMC_CREATION_H
#define MISA_KMC_CREATION_H

#include "abvi/defect/vacancy_list.h"
#include "lattice/lattices_list.h"
#include <comm/domain/colored_domain.h>
#include <string>

class creation {
public:
  /**
   * \brief create lattices property randomly
   * \param seed_create_types random seed for creating lattice types
   * \param seed_create_vacancy random seed for creating vacancies
   * \param lats pointer of lattice list
   * \param va_list the vacancy defects list.
   * \param types lattice types list to be created
   * \param types_ratio ratio of each lattice types.
   * \param va_count count of vacancy
   * \param p_domain pointer of domain.
   */
  static void createRandom(uint32_t seed_create_types, uint32_t seed_create_vacancy, LatticesList *lats,
                           VacancyList *va_list, const std::vector<LatticeTypes::lat_type> types,
                           const std::vector<unsigned int> types_ratio, const unsigned long va_count,
                           const comm::ColoredDomain *p_domain);

  /**
   * \brief set global id for lattices in simulation box on current process.
   * \param lats_list lattice list
   * \param lbr region of current sub-box in local coordinate system
   * \param gbr region of current sub-box in global coordinate system
   * \param phase_space the total lattice size at each dimension
   */
  static void setGlobalId(LatticesList *lats_list, const comm::Region<comm::_type_lattice_coord> lbr,
                          const comm::Region<comm::_type_lattice_coord> gbr,
                          std::array<uint64_t, comm::DIMENSION_SIZE> phase_space);
};

#endif // MISA_KMC_CREATION_H
