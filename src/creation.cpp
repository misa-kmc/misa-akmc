//
// Created by genshen on 2019/10/6.
//

#include "creation.h"
#include "type_define.h"
#include "utils/macros.h"
#include "utils/random/random.h"
#include "utils/random/rng_type.h"
#include "utils/simulation_domain.h"
#include <cassert>
#include <fstream>
#include <utils/bundle.h>
#include <utils/mpi_utils.h>

void creation::createAtomsRandom(uint32_t seed_create_types, LatticesList *lats,
                                 const std::vector<LatticeTypes::lat_type> types,
                                 const std::vector<unsigned int> types_ratio, const comm::ColoredDomain *p_domain) {
  unsigned int ratio_total = 0;
  for (int i = 0; i < types.size(); i++) {
    ratio_total += types_ratio[i];
  }
  std::uniform_int_distribution<> types_dis(1, ratio_total);
  r::type_rng types_rng(seed_create_types);
  // todo skip ghost lattices.
  lats->forAllLattices(
      [&](const _type_lattice_coord x, const _type_lattice_coord y, const _type_lattice_coord z, Lattice &lattice) {
        // set lattice types randomly.
        const unsigned int rand_hit = types_dis(types_rng);
        lattice.type._type = LatticeTypes::randomAtomsType(types.data(), types_ratio.data(), types.size(), rand_hit);
        return true;
      });
}

void creation::createRandom(uint32_t seed_create_types, uint32_t seed_create_vacancy, LatticesList *lats,
                            VacancyList *va_list, const std::vector<LatticeTypes::lat_type> types,
                            const std::vector<unsigned int> types_ratio, const unsigned long va_count,
                            const comm::ColoredDomain *p_domain) {
  // create lattice types
  createAtomsRandom(seed_create_types, lats, types, types_ratio, p_domain);

  // create vacancy
  // to use SimulationDomain, make sure this function is called after
  // simulation::createDomain()
  const unsigned long va_local =
      va_count / SimulationDomain::comm_sim_pro.all_ranks +
      (SimulationDomain::comm_sim_pro.own_rank < (va_count % SimulationDomain::comm_sim_pro.all_ranks) ? 1 : 0);
  const comm::_type_lattice_size max_lattice_size = lats->meta.box_x * lats->meta.box_y * lats->meta.box_z;
  std::uniform_int_distribution<> va_dis(0, max_lattice_size - 1);
  r::type_rng va_rng(seed_create_vacancy);
#ifdef KMC_DEBUG_MODE
  assert(max_lattice_size >= 1);
#endif
  for (unsigned long va_i = 0; va_i < va_local;) {
    unsigned long local_id = va_dis(va_rng);
    // get the coordinate of the lattice in simulation box
    _type_lattice_coord x = local_id % lats->meta.box_x;
    local_id = local_id / lats->meta.box_x;
    _type_lattice_coord y = local_id % lats->meta.box_y;
    _type_lattice_coord z = local_id / lats->meta.box_y;
    Lattice &lat = lats->getLat(x + lats->meta.ghost_x, y + lats->meta.ghost_y, z + lats->meta.ghost_z);
    if (!lat.type.isVacancy()) {
      lat.type._type = LatticeTypes::V;
      va_i++;
    }
  }
  // todo init interval list
  va_list->reindex(lats, p_domain->local_sub_box_lattice_region);
}

void creation::setGlobalId(LatticesList *lats_list, const comm::Region<comm::_type_lattice_coord> lbr,
                           const comm::Region<comm::_type_lattice_coord> gbr,
                           std::array<uint64_t, comm::DIMENSION_SIZE> phase_space) {
  for (comm::_type_lattice_coord z = lbr.z_low; z < lbr.z_high; z++) {
    for (comm::_type_lattice_coord y = lbr.y_low; y < lbr.y_high; y++) {
      for (comm::_type_lattice_coord x = BCC_DBX * lbr.x_low; x < BCC_DBX * lbr.x_high; x++) {
        Lattice &lattice = lats_list->getLat(x, y, z);
        const comm::_type_lattice_coord gx = (x - lats_list->meta.ghost_x) + BCC_DBX * gbr.x_low;
        const comm::_type_lattice_coord gy = (y - lats_list->meta.ghost_y) + gbr.y_low;
        const comm::_type_lattice_coord gz = (z - lats_list->meta.ghost_z) + gbr.z_low;
        // convert xyz to global id.
        const comm::_type_lattice_coord gid = gx + BCC_DBX * phase_space[0] * (gy + gz * phase_space[1]);
        lattice.id = gid;
      }
    }
  }
}

void creation::createFromPile(const std::string pipe_file, uint32_t seed_create_types, LatticesList *lats,
                              VacancyList *va_list, const std::vector<LatticeTypes::lat_type> types,
                              const std::vector<unsigned int> types_ratio, const comm::ColoredDomain *p_domain) {
  createAtomsRandom(seed_create_types, lats, types, types_ratio, p_domain);

  // open vacancies file
  std::vector<std::array<_type_lattice_coord, 3>> vac_positions{};
  if (kiwi::mpiUtils::global_process.own_rank == MASTER_PROCESSOR) {
    std::ifstream vac_file;
    vac_file.open(pipe_file);
    _type_lattice_coord x, y, z;
    while (vac_file >> x >> y >> z) {
      vac_positions.push_back({x, y, z});
    }
  }

  kiwi::Bundle bundle = kiwi::Bundle();
  bundle.newPackBuffer(1024 * 1024); // make sure it is large enough

  if (kiwi::mpiUtils::global_process.own_rank == MASTER_PROCESSOR) {
    bundle.put(vac_positions);
  }
  MPI_Bcast(bundle.getPackedData(), bundle.getPackedDataCap(), MPI_BYTE, MASTER_PROCESSOR,
            MPI_COMM_WORLD); // synchronize data

  if (kiwi::mpiUtils::global_process.own_rank != MASTER_PROCESSOR) { // unpack
    int cursor = 0;
    bundle.get(cursor, vac_positions);
  }
  bundle.freePackBuffer();

  // place vacancies
  for (auto vac : vac_positions) {
    _type_lattice_coord x = vac[0]; // vac[0] is doubled.
    _type_lattice_coord y = vac[1];
    _type_lattice_coord z = vac[2];
    if (p_domain->local_sub_box_lattice_region.isIn(x / 2, y, z)) {
      Lattice &lat = lats->getLat(x + lats->meta.ghost_x, y + lats->meta.ghost_y, z + lats->meta.ghost_z);
      if (!lat.type.isVacancy()) {
        lat.type._type = LatticeTypes::V;
      }
    }
  }

  // update vacancies list
  va_list->reindex(lats, p_domain->local_sub_box_lattice_region);
}
