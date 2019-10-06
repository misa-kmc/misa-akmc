//
// Created by genshen on 2019/10/6.
//

#include <utils/random/random.h>
#include "creation.h"
#include "type_define.h"

void creation::createRandom(LatticesList *lats, const std::vector<LatticeTypes::lat_type> types,
                            const std::vector<unsigned int> types_ratio, const unsigned long va_count,
                            const comm::ColoredDomain *p_domain) {
    r::initSeed(); // initialize random number seed // todo generate seed here
    // todo skip ghost lattices.
    lats->forAllLattices([&](const _type_lattice_coord x,
                             const _type_lattice_coord y,
                             const _type_lattice_coord z,
                             Lattice &lattice) {
        // set lattice types randomly.
        lattice.type._type = LatticeTypes::randomAtomsType(types.data(), types_ratio.data(), types.size());
        return true;
    });
}
