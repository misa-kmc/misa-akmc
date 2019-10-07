//
// Created by genshen on 2019/9/21.
//

#include <utils/random/random.h>
#include "sublattice.h"

void SubLattice::startTimeLoop() {
    double current_time = 0;
    unsigned long step = 0;
    while (current_time < time_limit) { // time loop for
        comm::Region<comm::_type_lattice_size> region; // todo calculate region.
        const double total_rates = calcRates(region);
        selectRate();
        perform();
        double delta_t = -log(r::random() / total_rates);
        current_time += delta_t;
        sec_meta.sector_time += delta_t;
        step++;
        if (sec_meta.sector_time >= sec_meta.T) {
            // todo communication current sector.
            beforeNextSector(); // prepare for next sector(e.g communication ghost area data).
            nextSector(); // move to next sector.
        }
    }
}

double SubLattice::calcRates(const comm::Region<comm::_type_lattice_size> region) {

}

void SubLattice::selectRate() {

}

void SubLattice::perform() {

}

void SubLattice::nextSector() {
    // todo update sector id
    // update time
    sec_meta.sector_time -= sec_meta.T;
}

void SubLattice::beforeNextSector() {
// todo sync ghost for next ghost.
    ++cur_sector;
}
