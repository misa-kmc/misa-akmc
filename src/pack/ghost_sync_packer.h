//
// Created by genshen on 2019/10/14.
//

#ifndef MISA_KMC_GHOST_SYNC_PACKER_H
#define MISA_KMC_GHOST_SYNC_PACKER_H

#include <algorithms/lattice_region_packer.h>

//  the region type(pack_region_type) is always comm::_type_lattice_coord.
class GhostSyncPacker : public LatticeRegionPacker<int> {
public:
    const unsigned long sendLength(const std::vector<comm::Region<pack_region_type >> send_regions,
                                   const int dimension, const int direction) override;

    void onSend(pack_date_type buffer[], const std::vector<comm::Region<pack_region_type>> send_regions,
                const unsigned long send_len, const int dimension, const int direction) override;

    void onReceive(pack_date_type buffer[], const std::vector<comm::Region<pack_region_type>> recv_regions,
                   const unsigned long receive_len, const int dimension, const int direction) override;

    static MPI_Datatype getMPI_DataType();
};


#endif //MISA_KMC_GHOST_SYNC_PACKER_H
