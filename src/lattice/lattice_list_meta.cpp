//
// Created by genshen on 2019/11/25.
//

#include <utils/macros.h>
#include "lattice_list_meta.h"

LatListMeta::LatListMeta(const _type_box_size bx, const _type_box_size by, const _type_box_size bz,
                         const _type_box_size gx, const _type_box_size gy, const _type_box_size gz)
        : size_x(BCC_DBX * bx + BCC_DBX * 2 * gx), size_y(by + 2 * gy),
          size_z(bz + 2 * gz), _max_id(size_z * size_y * size_x - 1),
          box_x(BCC_DBX * bx), box_y(by), box_z(bz) {

}
