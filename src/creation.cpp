//
// Created by genshen on 2019/10/6.
//

#include <utils/random/random.h>
#include "utils/simulation_domain.h"
#include "utils/macros.h"
#include "creation.h"
#include "type_define.h"

void creation::createRandom(LatticesList *lats, VacancyList *va_list,
                            const std::vector<LatticeTypes::lat_type> types,
                            const std::vector<unsigned int> types_ratio, const unsigned long va_count,
                            const comm::ColoredDomain *p_domain) {
    r::initSeed(0); // initialize random number seed // todo generate seed here
    // todo skip ghost lattices.
    lats->forAllLattices([&](const _type_lattice_coord x,
                             const _type_lattice_coord y,
                             const _type_lattice_coord z,
                             Lattice &lattice) {
        // set lattice types randomly.
        lattice.type._type = LatticeTypes::randomAtomsType(types.data(), types_ratio.data(), types.size());
        return true;
    });

    // create vacancy
    // to use SimulationDomain, make sure this function is called after simulation::createDomain()
    const unsigned long va_local = va_count / SimulationDomain::comm_sim_pro.all_ranks +
                                   SimulationDomain::comm_sim_pro.own_rank <
                                   va_count % SimulationDomain::comm_sim_pro.all_ranks ? 1 : 0;
    const comm::_type_lattice_size max_lattice_size = BCC_DBX * p_domain->sub_box_lattice_size[0] *
                                                      p_domain->sub_box_lattice_size[1] *
                                                      p_domain->sub_box_lattice_size[2];
    for (unsigned long va_i = 0; va_i < va_local;) {
        unsigned long local_id = r::rand32(0, max_lattice_size);
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
