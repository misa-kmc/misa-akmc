//
// Created by genshen on 2019/11/25.
//

#include <cassert>
#include <utils/macros.h>
#include "lattice_list_meta.h"

LatListMeta::LatListMeta(const _type_box_size box_x, const _type_box_size box_y, const _type_box_size box_z,
                         const _type_box_size ghost_x, const _type_box_size ghost_y, const _type_box_size ghost_z,
                         const _type_box_size gbx, const _type_box_size gby, const _type_box_size gbz,
                         const _type_box_size g_base_x, const _type_box_size g_base_y, const _type_box_size g_base_z)
        : size_x(BCC_DBX * box_x + BCC_DBX * 2 * ghost_x), size_y(box_y + 2 * ghost_y),
          size_z(box_z + 2 * ghost_z), _max_id(size_z * size_y * size_x - 1),
          box_x(BCC_DBX * box_x), box_y(box_y), box_z(box_z),
          ghost_x(BCC_DBX * ghost_x), ghost_y(ghost_y), ghost_z(ghost_z),
          g_box_x(BCC_DBX * gbx), g_box_y(gby), g_box_z(gbz),
          g_base_x(BCC_DBX * g_base_x), g_base_y(g_base_y), g_base_z(g_base_z) {}

LatListMeta::LatListMeta(const _type_box_size box_x, const _type_box_size box_y, const _type_box_size box_z,
                         const _type_box_size ghost_x, const _type_box_size ghost_y, const _type_box_size ghost_z)
        : LatListMeta(box_x, box_y, box_z, ghost_x, ghost_y, ghost_z, box_x, box_y, box_z, 0, 0, 0) {}


_type_lattice_id LatListMeta::getIdBy1nnOffset(const _type_lattice_id lat_id, const _type_dir_id _1nn_offset) const {
    _type_lattice_coord x, y, z;
    getCoordByLId(lat_id, &x, &y, &z);
    if (x % 2 == 0) {
        switch (_1nn_offset) {
            case 0:
                return getLId(x - 1, y - 1, z - 1);
            case 1:
                return getLId(x - 1, y - 1, z + 0);
            case 2:
                return getLId(x - 1, y + 0, z - 1);
            case 3:
                return getLId(x - 1, y + 0, z + 0);
            case 4:
                return getLId(x + 1, y - 1, z - 1);
            case 5:
                return getLId(x + 1, y - 1, z + 0);
            case 6:
                return getLId(x + 1, y + 0, z - 1);
            case 7:
                return getLId(x + 1, y + 0, z + 0);
            default:
                assert(false);
        }

    } else {
        switch (_1nn_offset) {
            case 0:
                return getLId(x - 1, y + 0, z + 0);
            case 1:
                return getLId(x - 1, y + 0, z + 1);
            case 2:
                return getLId(x - 1, y + 1, z + 0);
            case 3:
                return getLId(x - 1, y + 1, z + 1);
            case 4:
                return getLId(x + 1, y + 0, z + 0);
            case 5:
                return getLId(x + 1, y + 0, z + 1);
            case 6:
                return getLId(x + 1, y + 1, z + 0);
            case 7:
                return getLId(x + 1, y + 1, z + 1);
            default:
                assert(false);
        }
    }
}
