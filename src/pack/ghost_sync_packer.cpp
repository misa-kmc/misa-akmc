//
// Created by genshen on 2019/10/14.
//

#include "ghost_sync_packer.h"

const unsigned long GhostSyncPacker::sendLength(const std::vector<comm::Region<pack_region_type>> send_regions,
                                                const int dimension, const int direction) {
    return 0;
}

void GhostSyncPacker::onSend(pack_date_type *buffer,
                             const std::vector<comm::Region<pack_region_type>> send_regions,
                             const unsigned long send_len, const int dimension, const int direction) {

}

void GhostSyncPacker::onReceive(pack_date_type *buffer,
                                const std::vector<comm::Region<pack_region_type>> recv_regions,
                                const unsigned long receive_len, const int dimension,
                                const int direction) {

}

MPI_Datatype GhostSyncPacker::getMPI_DataType() {
    return MPI_INT;
}
