//
// Created by genshen on 2019/10/13.
//

#ifndef MISA_KMC_LATTICE_REGION_PACKER_H
#define MISA_KMC_LATTICE_REGION_PACKER_H

#include <comm/types_define.h>
#include <comm/region_packer.h>

template<class T>
class LatticeRegionPacker : public comm::RegionPacker<T, comm::_type_lattice_coord> {
public:
    typedef T pack_date_type;
    typedef comm::_type_lattice_coord pack_region_type;
};

#endif //MISA_KMC_LATTICE_REGION_PACKER_H
