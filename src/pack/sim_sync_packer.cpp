//
// Created by genshen on 2019/11/15.
//

#include <utils/macros.h>
#include "sim_sync_packer.h"

SimSyncPacker::SimSyncPacker(LatticesList *lattice_list) : lats(lattice_list) {

}

const unsigned long
SimSyncPacker::sendLength(const std::vector<comm::Region<pack_region_type>> send_regions, const int dimension,
                          const int direction) {
    unsigned long size_send = 0;
    for (auto r:send_regions) {
        size_send += r.volume();
    }
    return size_send;
}

void SimSyncPacker::onSend(Lattice *buffer, const std::vector<comm::Region<pack_region_type>> send_regions,
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

void SimSyncPacker::onReceive(Lattice *buffer, const std::vector<comm::Region<pack_region_type>> recv_regions,
                              const unsigned long receive_len, const int dimension, const int direction) {
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

