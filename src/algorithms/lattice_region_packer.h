//
// Created by genshen on 2019/10/13.
//

#ifndef MISA_KMC_LATTICE_REGION_PACKER_H
#define MISA_KMC_LATTICE_REGION_PACKER_H

#include <comm/region_packer.h>
#include <comm/types_define.h>

template <class T> class LatticeRegionPacker : public comm::RegionPacker<T, comm::_type_lattice_coord> {};

#endif // MISA_KMC_LATTICE_REGION_PACKER_H
