//
// Created by genshen on 2019/11/25.
//

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
          g_box_x(BCC_DBX * gbx), g_box_y(BCC_DBX * gby), g_box_z(BCC_DBX * gbz),
          g_base_x(BCC_DBX * g_base_x), g_base_y(g_base_y), g_base_z(g_base_z) {}

LatListMeta::LatListMeta(const _type_box_size box_x, const _type_box_size box_y, const _type_box_size box_z,
                         const _type_box_size ghost_x, const _type_box_size ghost_y, const _type_box_size ghost_z)
        : LatListMeta(box_x, box_y, box_z, ghost_x, ghost_y, ghost_z, box_x, box_y, box_z, 0, 0, 0) {}
