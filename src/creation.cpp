//
// Created by genshen on 2019/10/6.
//

#include <utils/random/random.h>
#include "creation.h"
#include "type_define.h"

void creation::create(LatticesList *lats, const comm::ColoredDomain *p_domain) {
    // init
    r::initSeed(); // initialize random number seed // todo generate seed here

    lats->forAllLattices([&](const _type_lattice_coord x,
                             const _type_lattice_coord y,
                             const _type_lattice_coord z,
                             Lattice &lattice) {
        // set lattice types randomly.
        //  todo p_config->configValues.create.types;
        //  todo p_config->configValues.create.types;
        //  todo  p_config->configValues.create.types_ratio
        const LatticeTypes::lat_type src_types[] = {LatticeTypes::Fe, LatticeTypes::Cu,
                                                    LatticeTypes::Ni, LatticeTypes::Mn};
        const unsigned int ratio[] = {100, 0, 0, 0}; // Fe based lattice

        lattice.type._type = LatticeTypes::randomAtomsType(src_types, ratio, 4);
        return true;
    });
}
