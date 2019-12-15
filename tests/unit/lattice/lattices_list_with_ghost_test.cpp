//
// Created by genshen on 2019/12/11.
//

#include <gtest/gtest.h>
#include <comm/domain/region.hpp>
#include <comm/types_define.h>
#include <lattice/period_lattice_list.h>
#include <utils/macros.h>

/**
 * in this test file, the lattice is tested with ghost area.
 */

// test lattice index xyz to lattice id
TEST(lattices_list_getId_test, lattices_list) {
    // note, box_x  and ghost_x are not doubled.
    const comm::_type_lattice_size box_x = 2, box_y = 2, box_z = 2; // real box size
    const comm::_type_lattice_size g_x = 8, g_y = 8, g_z = 8; // ghost size
    const comm::Region<comm::_type_lattice_size> tr{
            BCC_DBX * g_x, g_y, g_z,
            BCC_DBX * (g_x + box_x), g_y + box_y, g_z + box_z
    };
    auto *p_lat_list = new PeriodLatticeList(box_x, box_y, box_z, g_x, g_y, g_z);

    // compare results of Lattice::getId() and LatticesList::getId(x, y, z)
    // we can also say: test xyz to id
    for (comm::_type_lattice_size z = tr.z_low; z < tr.z_high; z++) {
        for (comm::_type_lattice_size y = tr.y_low; y < tr.y_high; y++) {
            for (comm::_type_lattice_size x = tr.x_low; x < tr.x_high; x++) {
                Lattice &lat = p_lat_list->getLat(x, y, z);
                EXPECT_EQ(lat.getId(), p_lat_list->getId(x, y, z));
            }
        }
    }

    delete p_lat_list;
}

// test lattice id to lattice 3d index xyz
TEST(lattices_list_getLat_test, lattices_list) {
    // note, box_x  and ghost_x are not doubled.
    const comm::_type_lattice_size box_x = 2, box_y = 2, box_z = 2; // real box size
    const comm::_type_lattice_size g_x = 8, g_y = 8, g_z = 8; // ghost size
    const comm::Region<comm::_type_lattice_size> tr{
            BCC_DBX * g_x, g_y, g_z,
            BCC_DBX * (g_x + box_x), g_y + box_y, g_z + box_z
    };
    auto *p_lat_list = new PeriodLatticeList(box_x, box_y, box_z, g_x, g_y, g_z);

    for (comm::_type_lattice_size z = tr.z_low; z < tr.z_high; z++) {
        for (comm::_type_lattice_size y = tr.y_low; y < tr.y_high; y++) {
            for (comm::_type_lattice_size x = tr.x_low; x < tr.x_high; x++) {
                Lattice &lat = p_lat_list->getLat(x, y, z);
                // test function LatticesList::getLat(id)
                // 1. we compare the memory address of the result lattice
                // 2. we compare the id of the result lattice
                EXPECT_EQ(&lat, &(p_lat_list->getLat(lat.getId())));
                EXPECT_EQ(lat.id, p_lat_list->getLat(lat.getId()).id);
            }
        }
    }

    delete p_lat_list;
}
