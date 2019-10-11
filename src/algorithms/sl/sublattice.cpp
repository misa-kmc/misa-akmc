//
// Created by genshen on 2019/9/21.
//

#include <preset/sector_forwarding_region.h>
#include <utils/random/random.h>
#include "sublattice.h"

SectorMeta::SectorMeta() : sector_itl(sectors.begin()) {}

SubLattice::SubLattice(const comm::ColoredDomain *p_domain, ModelAdapter *p_model,
                       const double time_limit, const double T) :
        p_domain(p_domain), T(T), time_limit(time_limit),
        sec_meta(), p_model(p_model) {}

void SubLattice::startTimeLoop() {
    const unsigned long time_steps = ceil(time_limit / T);
    for (unsigned long step = 0; step < time_steps; step++) { // time steps loop
        for (int sect = 0; sect < SECTORS_NUM; sect++) { // sector loop
            const double init_overflow_time = sec_meta.sector_itl->evolution_time - step * T;
            double sector_time = init_overflow_time;
            while (sector_time < T) {
                const double total_rates = calcRates((*sec_meta.sector_itl).id);
                p_model->selectRate();
                p_model->perform();
                const double delta_t = -log(r::random() / total_rates);
                sector_time += delta_t;
            }
            // kmc simulation on this sector is finished
            // we store evolution time of current sector when the sector loop finishes.
            sec_meta.sector_itl->evolution_time += sector_time - init_overflow_time;
            syncSimRegions(); // communicate ghost area of current process to sync simulation regions of neighbor process.
            syncNextSectorGhostRegions(); // communicate ghost area data of next sector in current process.
            ++sec_meta.sector_itl; // update sector id.
            nextSector(); // some post operations after moved to next sector.
        }
    }
}

double SubLattice::calcRates(const type_sector_id sector_id) {
    comm::Region<comm::_type_lattice_size> region; // todo calculate region.
    return p_model->calcRates(region);
}

void SubLattice::syncSimRegions() {
    // todo communication current sector.
    const type_sector cur_sector = (*sec_meta.sector_itl);

}

void SubLattice::syncNextSectorGhostRegions() {
    const type_sector next_sector = sec_meta.sector_itl.next();
    const int dim_x = 0;
    comm::type_region_array send_regions = comm::fwCommSectorSendRegion(
            next_sector.id, dim_x, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
}

void SubLattice::nextSector() {
}
