//
// Created by genshen on 2019/9/21.
//

#include <utils/random/random.h>
#include "sublattice.h"

SectorMeta::SectorMeta() : sector_itl(sectors.begin()) {}

SubLattice::SubLattice(const comm::ColoredDomain *p_domain, const double T)
        : p_domain(p_domain), T(T), sec_meta() { // todo T from parameter.

}

void SubLattice::startTimeLoop() {
    const unsigned long time_steps = ceil(time_limit / T);
    for (unsigned long step = 0; step < time_steps; step++) { // time steps loop
        for (int sect = 0; sect < SECTORS_NUM; sect++) { // sector loop
            const double init_overflow_time = sec_meta.sector_itl->evolution_time - step * T;
            double sector_time = init_overflow_time;
            while (sector_time < T) {
                comm::Region<comm::_type_lattice_size> region; // todo calculate region.
                const double total_rates = calcRates(region);
                selectRate();
                perform();
                const double delta_t = -log(r::random() / total_rates);
                sector_time += delta_t;
            }
            // kmc simulation on this sector is finished
            // we store evolution time of current sector when the sector loop finishes.
            sec_meta.sector_itl->evolution_time += sector_time - init_overflow_time;
            beforeNextSector(); // prepare for next sector(e.g communication ghost area data).
            ++sec_meta.sector_itl; // update sector id.
            nextSector(); // move to next sector.
        }
    }
}

double SubLattice::calcRates(const comm::Region<comm::_type_lattice_size> region) {
    return 0;
}

void SubLattice::selectRate() {

}

void SubLattice::perform() {

}

void SubLattice::nextSector() {
}

void SubLattice::beforeNextSector() {
    // todo communication current sector.
    // todo sync ghost for next ghost.
}
