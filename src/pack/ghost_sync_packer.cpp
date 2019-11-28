//
// Created by genshen on 2019/10/14.
//

#include "utils/macros.h"
#include "ghost_sync_packer.h"

GhostSyncPacker::GhostSyncPacker(LatticesList *lats_list) : lats(lats_list) {

}

const unsigned long GhostSyncPacker::sendLength(const std::vector<comm::Region<pack_region_type>> send_regions,
                                                const int dimension, const int direction) {
    unsigned long size_ghost = 0;
    for (auto r:send_regions) {
        size_ghost += r.volume();
    }
    return size_ghost;
}

void GhostSyncPacker::onSend(pack_date_type *buffer,
                             const std::vector<comm::Region<pack_region_type>> send_regions,
                             const unsigned long send_len, const int dimension, const int direction) {
    unsigned long len = 0;
    for (auto &r:send_regions) {
        for (int z = r.z_low; z < r.z_high; z++) {
            for (int y = r.y_low; y < r.y_high; y++) {
                for (int x = r.x_low; x < r.x_high; x++) {
                    buffer[len++] = lats->_lattices[z][y][BCC_DBX * x];
                    buffer[len++] = lats->_lattices[z][y][BCC_DBX * x + 1];
                }
            }
        }
    }
}

void GhostSyncPacker::onReceive(pack_date_type *buffer,
                                const std::vector<comm::Region<pack_region_type>> recv_regions,
                                const unsigned long receive_len, const int dimension,
                                const int direction) {
    unsigned long len = 0;
    for (auto &r:recv_regions) {
        for (int z = r.z_low; z < r.z_high; z++) {
            for (int y = r.y_low; y < r.y_high; y++) {
                for (int x = r.x_low; x < r.x_high; x++) {
                    lats->_lattices[z][y][BCC_DBX * x] = buffer[len++];
                    lats->_lattices[z][y][BCC_DBX * x + 1] = buffer[len++];
                }
            }
        }
    }
}
