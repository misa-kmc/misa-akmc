//
// Created by genshen on 2019/12/11.
//

#include <comm/domain/region.hpp>
#include <comm/types_define.h>
#include <gtest/gtest.h>
#include <lattice/period_lattice_list.h>
#include <utils/macros.h>

/**
 * in this test file, the lattice is tested with ghost area.
 */

TEST(lattice_list_ghost_getLatById_test, lattice_list_test) {
  PeriodLatticeList lattice_list(LatListMeta{4, 4, 4, 8, 8, 8});
  // check all id in box.
  for (_type_lattice_id id = 0; id <= lattice_list.maxId(); id++) {
    auto lat = lattice_list.getLat(id);
    EXPECT_EQ(lat.getId(), id);
  }
}

// test lattice index xyz to lattice id
TEST(lattices_list_getId_test, lattices_list) {
  // note, box_x  and ghost_x are not doubled.
  const comm::_type_lattice_size box_x = 2, box_y = 2,
                                 box_z = 2;                 // real box size
  const comm::_type_lattice_size g_x = 8, g_y = 8, g_z = 8; // ghost size
  const comm::Region<comm::_type_lattice_size> tr{BCC_DBX * g_x,           g_y,         g_z,
                                                  BCC_DBX * (g_x + box_x), g_y + box_y, g_z + box_z};
  auto *p_lat_list = new PeriodLatticeList(LatListMeta{box_x, box_y, box_z, g_x, g_y, g_z});

  // compare results of Lattice::getId() and LatticesList::getId(x, y, z)
  // we can also say: test xyz to id
  for (comm::_type_lattice_size z = tr.z_low; z < tr.z_high; z++) {
    for (comm::_type_lattice_size y = tr.y_low; y < tr.y_high; y++) {
      for (comm::_type_lattice_size x = tr.x_low; x < tr.x_high; x++) {
        Lattice &lat = p_lat_list->getLat(x, y, z);
        if (p_lat_list->getId(x, y, z) == 0) {
          EXPECT_EQ(lat.getId(), p_lat_list->getId(x, y, z));
        }
      }
    }
  }

  delete p_lat_list;
}

// test lattice id to lattice 3d index xyz
TEST(lattices_list_getLat_test, lattices_list) {
  // note, box_x  and ghost_x are not doubled.
  const comm::_type_lattice_size box_x = 2, box_y = 2,
                                 box_z = 2;                 // real box size
  const comm::_type_lattice_size g_x = 8, g_y = 8, g_z = 8; // ghost size
  const comm::Region<comm::_type_lattice_size> tr{BCC_DBX * g_x,           g_y,         g_z,
                                                  BCC_DBX * (g_x + box_x), g_y + box_y, g_z + box_z};
  auto *p_lat_list = new PeriodLatticeList(LatListMeta{box_x, box_y, box_z, g_x, g_y, g_z});

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

// tests of get2nn with ghost area
/**
 * \brief get id by relative coordinate to ghost boundary
 * \param meta lattice metadata
 * \param x,y,z the relative coordinate to ghost boundary (not from box
 * boundary) \return local lattice id.
 */
inline _type_lattice_id ID_BOX_WITH_GHOST(LatListMeta meta, comm::_type_lattice_coord x, comm::_type_lattice_coord y,
                                          comm::_type_lattice_coord z) {
  return (x + meta.ghost_x) + meta.size_x * (y + meta.ghost_y) + meta.size_x * meta.size_y * (z + meta.ghost_z);
}

TEST(get2nn_1, lattice_list_ghost_test) {
  Lattice *_2nn[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 6 2nn(s).
  // note: box_x and ghost_x are not doubled
  PeriodLatticeList lattice_list(LatListMeta{4, 4, 4, 8, 8, 8});
  auto count = lattice_list.get2nn(lattice_list.meta.ghost_x + 2, lattice_list.meta.ghost_y + 1,
                                   lattice_list.meta.ghost_z + 1, _2nn);
  // test count
  EXPECT_EQ(count, 6);

  EXPECT_EQ(_2nn[0]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 0, 1, 1));
  EXPECT_EQ(_2nn[1]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 2, 0, 1));
  EXPECT_EQ(_2nn[2]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 2, 1, 0));
  EXPECT_EQ(_2nn[3]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 2, 1, 2));
  EXPECT_EQ(_2nn[4]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 2, 2, 1));
  EXPECT_EQ(_2nn[5]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 4, 1, 1));
}

TEST(get2nn_isGhostLat_low_boundary, lattice_list_ghost_test) {
  Lattice *_2nn[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 6 2nn(s).
  PeriodLatticeList lattice_list(LatListMeta{4, 4, 4, 8, 8, 8});
  auto count = lattice_list.get2nn(lattice_list.meta.ghost_x + 0, lattice_list.meta.ghost_y + 0,
                                   lattice_list.meta.ghost_z + 0, _2nn);
  // test count
  EXPECT_EQ(count, 6);

  // the first three lattices are in ghost area
  EXPECT_TRUE(lattice_list.meta.isGhostLat(_2nn[0]->getId()));
  EXPECT_TRUE(lattice_list.meta.isGhostLat(_2nn[1]->getId()));
  EXPECT_TRUE(lattice_list.meta.isGhostLat(_2nn[2]->getId()));
  EXPECT_EQ(_2nn[3]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 0, 0, 1));
  EXPECT_EQ(_2nn[4]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 0, 1, 0));
  EXPECT_EQ(_2nn[5]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 2, 0, 0));
}

TEST(get2nn_isGhostLat_upper_boundary, lattice_list_ghost_test) {
  Lattice *_2nn[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 6 2nn(s).
  PeriodLatticeList lattice_list(LatListMeta{4, 4, 4, 8, 8, 8});
  auto count = lattice_list.get2nn(lattice_list.meta.ghost_x + 7, lattice_list.meta.ghost_y + 3,
                                   lattice_list.meta.ghost_z + 3, _2nn);
  // test count
  EXPECT_EQ(count, 6);

  EXPECT_EQ(_2nn[0]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 5, 3, 3));
  EXPECT_EQ(_2nn[1]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 7, 2, 3));
  EXPECT_EQ(_2nn[2]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 7, 3, 2));
  EXPECT_TRUE(lattice_list.meta.isGhostLat(_2nn[3]->getId()));
  EXPECT_TRUE(lattice_list.meta.isGhostLat(_2nn[4]->getId()));
  EXPECT_TRUE(lattice_list.meta.isGhostLat(_2nn[5]->getId()));
}

TEST(get2nn_isGhostLat_normal_boundary, lattice_list_ghost_test) {
  Lattice *_2nn[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 6 2nn(s).
  PeriodLatticeList lattice_list(LatListMeta{4, 4, 4, 8, 8, 8});
  auto count = lattice_list.get2nn(lattice_list.meta.ghost_x + 3, lattice_list.meta.ghost_y + 3,
                                   lattice_list.meta.ghost_z + 3, _2nn);
  // test count
  EXPECT_EQ(count, 6);

  EXPECT_EQ(_2nn[0]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 1, 3, 3));
  EXPECT_EQ(_2nn[1]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 3, 2, 3));
  EXPECT_EQ(_2nn[2]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 3, 3, 2));
  EXPECT_TRUE(lattice_list.meta.isGhostLat(_2nn[3]->getId()));
  EXPECT_TRUE(lattice_list.meta.isGhostLat(_2nn[4]->getId()));
  EXPECT_EQ(_2nn[5]->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 5, 3, 3));
}

TEST(lattice_list_ghost_MaxId_test, lattice_list_ghost_test) {
  PeriodLatticeList lattice_list(LatListMeta{4, 4, 4, 8, 8, 8});
  // check finding max id (coord max_x: 7, coord max_y: 3, coord max_z: 3).
  auto id = lattice_list.getId(2 * (4 + 8 + 8) - 1, 4 + 8 + 8 - 1, 4 + 8 + 8 - 1);
  EXPECT_EQ(id, lattice_list.maxId());
}

TEST(lattice_list_ghost_walk_test, lattice_list_ghost_test) {
  PeriodLatticeList lattice_list(LatListMeta{4, 4, 4, 8, 8, 8});
  // case 1
  Lattice *lat = lattice_list.walk(ID_BOX_WITH_GHOST(lattice_list.meta, 4, 1, 1), 1, 1, 1);
  EXPECT_NE(lat, nullptr);
  EXPECT_EQ(lat->getId(), ID_BOX_WITH_GHOST(lattice_list.meta, 5, 1, 1));

  Lattice *lat2 = lattice_list.walk(ID_BOX_WITH_GHOST(lattice_list.meta, 5, 1, 1), 2, 2, 2);
  EXPECT_NE(lat2, nullptr);
  EXPECT_EQ(lat2, &(lattice_list.getLat(ID_BOX_WITH_GHOST(lattice_list.meta, 7, 2, 2))));

  // invalid input
  Lattice *lat3 = lattice_list.walk(ID_BOX_WITH_GHOST(lattice_list.meta, 4, 1, 1), 1, 1, 2);
  EXPECT_EQ(lat3, nullptr);

  // invalid input
  Lattice *lat4 = lattice_list.walk(ID_BOX_WITH_GHOST(lattice_list.meta, 4, 1, 1), 2, 1, 2);
  EXPECT_EQ(lat4, nullptr);

  // test out of boundary(down).
  Lattice *lat5 = lattice_list.walk(ID_BOX_WITH_GHOST(lattice_list.meta, 4, 1, 1), -lattice_list.meta.ghost_x - 1,
                                    -2 * lattice_list.meta.ghost_y - 3, -2 * lattice_list.meta.ghost_z - 3);
  EXPECT_EQ(lat5, nullptr);

  // test out of boundary(up).
  Lattice *lat6 = lattice_list.walk(ID_BOX_WITH_GHOST(lattice_list.meta, lattice_list.meta.size_x,
                                                      lattice_list.meta.size_y, lattice_list.meta.size_z),
                                    1, 1, 1);
  EXPECT_EQ(lat6, nullptr);
}
