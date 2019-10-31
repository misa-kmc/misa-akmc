//
// Created by runchu on 2019-10-16.
//

#include <comm/preset/comm_forwarding_region.h>
#include "utils/macros.h"
#include "ghost_init_packer.h"

GhostInitPacker::GhostInitPacker(const comm::ColoredDomain *p_domain, LatticesList *lats_list)
        : p_domain(p_domain), lats(lats_list) {
}

const unsigned long GhostInitPacker::sendLength(const int dimension, const int direction) {
    comm::Region<comm::_type_lattice_size> send_region = comm::fwCommLocalSendRegion(
            p_domain->lattice_size_ghost, p_domain->local_sub_box_lattice_region,
            dimension, direction);
    return BCC_DBX * send_region.volume();
}

void GhostInitPacker::onSend(buffer_data_type *buffer, const unsigned long send_len,
                             const int dimension, const int direction) {
    const comm::Region<comm::_type_lattice_size> send_region = comm::fwCommLocalSendRegion(
            p_domain->lattice_size_ghost, p_domain->local_sub_box_lattice_region,
            dimension, direction);

    unsigned long len = 0;
    for (int x = send_region.x_low; x < send_region.x_high; x++) {
        for (int y = send_region.y_low; y < send_region.y_high; y++) {
            for (int z = send_region.z_low; z < send_region.z_high; z++) {
                buffer[len++] = lats->_lattices[z][y][BCC_DBX * x];
                buffer[len++] = lats->_lattices[z][y][BCC_DBX * x + 1];
            }
        }
    }
}

void GhostInitPacker::onReceive(GhostInitPacker::buffer_data_type *buffer, const unsigned long receive_len,
                                const int dimension, const int direction) {
    const comm::Region<comm::_type_lattice_size> recv_region = comm::fwCommLocalRecvRegion(
            p_domain->lattice_size_ghost, p_domain->local_sub_box_lattice_region,
            dimension, direction);

    unsigned long len = 0;
    for (int z = recv_region.z_low; z < recv_region.z_high; z++) {
        for (int y = recv_region.y_low; y < recv_region.y_high; y++) {
            for (int x = recv_region.x_low; x < recv_region.x_high; x++) {
                lats->_lattices[z][y][BCC_DBX * x] = buffer[len++];
                lats->_lattices[z][y][BCC_DBX * x + 1] = buffer[len++];
            }
        }
    }
    // todo set more information if the lattice is vacancy or dumbbell.
}
