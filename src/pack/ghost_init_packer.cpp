//
// Created by runchu on 2019-10-16.
//

#include "ghost_init_packer.h"
#include "type_define.h"


const unsigned long GhostInitPacker::sendLength(const unsigned int sector_id, const unsigned int dim,
                                                const _type_lattice_size ghost_size[comm::DIMENSION_SIZE],
                                                const _type_lattice_coord split_coord[comm::DIMENSION_SIZE],
                                                const comm::Region<comm::_type_lattice_coord> local_box_region) {
    comm::type_region_array region = comm::fwCommSectorSendRegion(
            sector_id, dim,
            reinterpret_cast<const comm::_type_lattice_size *>(ghost_size),
            reinterpret_cast<const comm::_type_lattice_coord *>(split_coord),
            local_box_region);

    unsigned long len = 0;
    for (auto iter = region.cbegin(); iter != region.cend(); iter++) {
        len += ((*iter).x_high - (*iter).x_low) * ((*iter).y_high - (*iter).y_low) * ((*iter).z_high - (*iter).z_low);
    }
    return len;
}

void GhostInitPacker::onSend(Lattice buffer[], const unsigned long send_len, LatticesList *lats,
                             const unsigned int sector_id, const unsigned int dim,
                             const _type_lattice_size ghost_size[comm::DIMENSION_SIZE],
                             const _type_lattice_coord split_coord[comm::DIMENSION_SIZE],
                             const comm::Region<comm::_type_lattice_coord> local_box_region) {
    comm::type_region_array region = comm::fwCommSectorSendRegion(
            sector_id, dim,
            reinterpret_cast<const comm::_type_lattice_size *>(ghost_size),
            reinterpret_cast<const comm::_type_lattice_coord *>(split_coord),
            local_box_region);

    unsigned long len = 0;
    for (auto iter = region.cbegin(); iter != region.cend(); iter++) {
        len += ((*iter).x_high - (*iter).x_low) * ((*iter).y_high - (*iter).y_low) * ((*iter).z_high - (*iter).z_low);
        for (int x = (*iter).x_low; x <= (*iter).x_high; x++) {
            for (int y = (*iter).y_low; y <= (*iter).y_high; y++) {
                for (int z = (*iter).z_low; z <= (*iter).z_high; z++) {
                    buffer[len++] = lats->_lattices[x][y][z];
                }
            }
        }
    }
}

void GhostInitPacker::onReceive(Lattice buffer[], const unsigned long receive_len, LatticesList *lats,
                                const unsigned int sector_id, const unsigned int dim,
                                const _type_lattice_size ghost_size[comm::DIMENSION_SIZE],
                                const _type_lattice_coord split_coord[comm::DIMENSION_SIZE],
                                const comm::Region<comm::_type_lattice_coord> local_box_region) {

    comm::type_region_array region = comm::fwCommSectorRecvRegion(
            sector_id, dim,
            reinterpret_cast<const comm::_type_lattice_size *>(ghost_size),
            reinterpret_cast<const comm::_type_lattice_coord *>(split_coord),
            local_box_region);

    unsigned long len = 0;
    for (auto iter = region.cbegin(); iter != region.cend(); iter++) {
        len += ((*iter).x_high - (*iter).x_low) * ((*iter).y_high - (*iter).y_low) * ((*iter).z_high - (*iter).z_low);
        for (int x = (*iter).x_low; x <= (*iter).x_high; x++) {
            for (int y = (*iter).y_low; y <= (*iter).y_high; y++) {
                for (int z = (*iter).z_low; z <= (*iter).z_high; z++) {
                    lats->_lattices[x][y][z] = buffer[len++];
                }
            }
        }
    }
}

