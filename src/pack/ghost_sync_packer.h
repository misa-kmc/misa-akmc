//
// Created by genshen on 2019/10/14.
//

#ifndef MISA_KMC_GHOST_SYNC_PACKER_H
#define MISA_KMC_GHOST_SYNC_PACKER_H

#include "lattice/lattices_list.h"
#include "lattice/lattice.h"
#include "algorithms/lattice_region_packer.h"
#include "utils/mpi_types.h"

/**
 * \brief ghost sync means: before performing computing on the simulation area,
 *       the ghost area must be received for its neighbor processes.
 * \note: the region type(pack_region_type) is always comm::_type_lattice_coord.
 */
class GhostSyncPacker : public LatticeRegionPacker<Lattice> {
public:
    explicit GhostSyncPacker(LatticesList *lats_list);

    const unsigned long sendLength(const std::vector<comm::Region<pack_region_type >> send_regions,
                                   const int dimension, const int direction) override;

    void onSend(pack_date_type buffer[], const std::vector<comm::Region<pack_region_type>> send_regions,
                const unsigned long send_len, const int dimension, const int direction) override;

    void onReceive(pack_date_type buffer[], const std::vector<comm::Region<pack_region_type>> recv_regions,
                   const unsigned long receive_len, const int dimension, const int direction) override;

    static inline MPI_Datatype getMPI_DataType() {
        return mpi_types::_mpi_type_lattice_data;
    }

private:
    LatticesList *lats = nullptr;
};


#endif //MISA_KMC_GHOST_SYNC_PACKER_H
