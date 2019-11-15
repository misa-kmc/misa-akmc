//
// Created by genshen on 2019/11/15.
//

#ifndef MISA_KMC_SIM_SYNC_PACKER_H
#define MISA_KMC_SIM_SYNC_PACKER_H


#include <lattice/lattices_list.h>
#include "utils/mpi_types.h"
#include "lattice/lattice.h"
#include "algorithms/lattice_region_packer.h"


class SimSyncPacker : public LatticeRegionPacker<Lattice> {

public:
    explicit SimSyncPacker(LatticesList *lattice_list);

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


#endif //MISA_KMC_SIM_SYNC_PACKER_H
