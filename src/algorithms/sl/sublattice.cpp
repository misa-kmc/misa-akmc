//
// Created by genshen on 2019/9/21.
//

#include <comm/preset/sector_forwarding_region.h>
#include "sublattice.h"

SectorMeta::SectorMeta() : sector_itl(sectors.begin()) {}

SubLattice::SubLattice(const comm::ColoredDomain *p_domain, ModelAdapter *p_model,
                       const double time_limit, const double T) :
        p_domain(p_domain), T(T), time_limit(time_limit),
        sec_meta(), p_model(p_model) {}

double SubLattice::calcRates(const type_sector_id sector_id) {
    return p_model->calcRates(p_domain->local_sector_region[sector_id]);
}

void SubLattice::nextSector() {
}
