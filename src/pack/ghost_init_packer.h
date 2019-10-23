//
// Created by runchu on 2019-10-16.
//

#ifndef MISA_KMC_GHOST_INIT_PACKER_H
#define MISA_KMC_GHOST_INIT_PACKER_H

#include<preset/sector_forwarding_region.h>
#include <lattice/normal_lattice_list.h>
#include "lattice/lattices_list.h"

class GhostInitPacker {
public:

    const unsigned long sendLength(const unsigned int sector_id, const unsigned int dim,
                                   const _type_lattice_size ghost_size[comm::DIMENSION_SIZE],
                                   const _type_lattice_coord split_coord[comm::DIMENSION_SIZE],
                                   const comm::Region<comm::_type_lattice_coord> local_box_region);

    void onSend(Lattice buffer[], const unsigned long send_len, LatticesList *lats,
                const unsigned int sector_id, const unsigned int dim,
                const _type_lattice_size ghost_size[comm::DIMENSION_SIZE],
                const _type_lattice_coord split_coord[comm::DIMENSION_SIZE],
                const comm::Region<comm::_type_lattice_coord> local_box_region);

    void onReceive(Lattice buffer[], const unsigned long receive_len, LatticesList *lats,
                   const unsigned int sector_id, const unsigned int dim,
                   const _type_lattice_size ghost_size[comm::DIMENSION_SIZE],
                   const _type_lattice_coord split_coord[comm::DIMENSION_SIZE],
                   const comm::Region<comm::_type_lattice_coord> local_box_region);

private:

};


#endif //MISA_KMC_GHOST_INIT_PACKER_H
