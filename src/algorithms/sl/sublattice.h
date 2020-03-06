//
// Created by genshen on 2019/9/21.
//

#ifndef MISA_KMC_SUBLATTICE_H
#define MISA_KMC_SUBLATTICE_H

#include "../selector.h"
#include "hook/event_hooks.hpp"
#include "models/model_adapter.h"
#include "utils/random/rng_type.h"
#include <array>
#include <comm/domain/colored_domain.h>

struct SectorMeta {
  explicit SectorMeta();

  // collection of sectors.
  type_sectors_ring sectors{{
      type_sector{comm::X_LOW | comm::Y_LOW | comm::Z_LOW, 0.0},
      type_sector{comm::X_HIGH | comm::Y_HIGH | comm::Z_HIGH, 0.0},
      type_sector{comm::X_LOW | comm::Y_HIGH | comm::Z_LOW, 0.0},
      type_sector{comm::X_HIGH | comm::Y_LOW | comm::Z_HIGH, 0.0},
      type_sector{comm::X_HIGH | comm::Y_HIGH | comm::Z_LOW, 0.0},
      type_sector{comm::X_LOW | comm::Y_LOW | comm::Z_HIGH, 0.0},
      type_sector{comm::X_HIGH | comm::Y_LOW | comm::Z_LOW, 0.0},
      type_sector{comm::X_LOW | comm::Y_HIGH | comm::Z_HIGH, 0.0},
  }};

  type_sectors_ring::iterator sector_itl;
};

class SubLattice {
public:
  /**
   * \brief initialize sub-lattice algorithm with a specific kMC model.
   * \param p_domain simulation domain.
   * \param seed_time_inc random number generation seed for kmc time
   * increasing. \param time_limit the max evolution time. \param T threshold
   * time for communication.
   */
  explicit SubLattice(const comm::ColoredDomain *p_domain,
                      const uint32_t seed_time_inc, const double time_limit,
                      const double T);

  /**
   * \brief start kmc time loop.
   * \tparam PKg packer used to sync ghost regions.
   * \tparam PKs packer used to sync simulation regions.
   * \tparam Ins packer instance creator.
   * \tparam E type of event in kmc model.
   * \param pk_inst to instant the packer for communication.
   * \param p_model kmc model to perform event selecting and execution.
   * \param event_hooks pointer of event hooks or callbacks for handing each
   * algorithm event. todo make sure RegionPacker is PKf, PKg, PKs's base
   * class at compiling time.
   */
  template <class PKg, class PKs, class Ins, typename E>
  void startTimeLoop(Ins pk_inst, ModelAdapter<E> *p_model,
                     EventHooks *p_event_hooks);

  /**
   * \brief calculate rates in a region
   * \tparam E type of event in kmc model.
   * \param sector_id sector id
   * \return the total rates
   */
  template <typename E>
  _type_rate calcRatesWrapper(ModelAdapter<E> *p_model,
                              const type_sector_id sector_id);

  /**
   * \brief wrapper function of event selection and performing
   * \tparam E model event type
   * \param p_model model pointer
   * \param total_rates the sum of rates
   * \param sector_id sector id
   */
  template <typename E>
  void selectPerformWrapper(ModelAdapter<E> *p_model,
                            const _type_rate total_rates,
                            const type_sector_id sector_id);

private:
  /**
   * \brief communicate ghost area of current process to sync simulation
   * regions of neighbor process. \tparam PKs packer used to sync simulation
   * regions. \tparam Ins packer instance creator. \param pk_inst to instant
   * the packer for communication.
   */
  template <class PKs, class Ins> void syncSimRegions(Ins &pk_inst);

  /**
   * \brief communicate ghost area data of next sector in current process.
   * \tparam PKg packer used to sync ghost regions.
   * \tparam Ins packer instance creator.
   * \param pk_inst to instant the packer for communication.
   */
  template <class PKg, class Ins> void syncNextSectorGhostRegions(Ins &pk_inst);

  /**
   * \brief some post operations after moved to next sector.
   */
  void nextSector();

public:
  // todo move this type to other file
  typedef std::array<std::vector<comm::Region<comm::_type_lattice_coord>>,
                     comm::DIMENSION_SIZE>
      type_comm_lat_regions;

private:
  /**
   * \brief distribution to produce random numbers in range (0,1) for kmc time
   * increasing.
   */
  std::uniform_real_distribution<double> time_inc_dis;
  /**
   * \brief random number generation for kmc time increasing
   */
  r::type_rng time_inc_rng;

  const comm::ColoredDomain *p_domain = nullptr;

  // threshold time for communication (the T can be changing in simulation).
  const double T;
  const double time_limit = 0.0;

  SectorMeta sec_meta;
};

#include "sublattice.inl"

#endif // MISA_KMC_SUBLATTICE_H
