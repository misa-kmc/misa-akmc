//
// Created by genshen on 2018-12-11.
//

#include <gtest/gtest.h>
#include <lattice/lattice.h>
#include <lattice/normal_lattice_list.h>

/**
 * in this test file, the lattice is tested without ghost area.
 */

#define ID_BOX_4_4_4(x, y, z) (x + y * 8 + z * 4 * 8)

// tests of get1nn
TEST(lattice_list_get1nn_1, lattice_list_test) {
  Lattice *_1nn[8] = {nullptr, nullptr, nullptr, nullptr,
                      nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 8 1nn(s).
  NormalLatticeList lattice_list(4, 4, 4);
  // test when x is even: ([x-1,x+1], [y-1, y], [z-1,z])
  auto count = lattice_list.get1nn(2, 1, 1, _1nn);
  EXPECT_EQ(count, 8);

  EXPECT_EQ(_1nn[0]->getId(), ID_BOX_4_4_4(1, 0, 0));
  EXPECT_EQ(_1nn[1]->getId(), ID_BOX_4_4_4(1, 0, 1));
  EXPECT_EQ(_1nn[2]->getId(), ID_BOX_4_4_4(1, 1, 0));
  EXPECT_EQ(_1nn[3]->getId(), ID_BOX_4_4_4(1, 1, 1));
  EXPECT_EQ(_1nn[4]->getId(), ID_BOX_4_4_4(3, 0, 0));
  EXPECT_EQ(_1nn[5]->getId(), ID_BOX_4_4_4(3, 0, 1));
  EXPECT_EQ(_1nn[6]->getId(), ID_BOX_4_4_4(3, 1, 0));
  EXPECT_EQ(_1nn[7]->getId(), ID_BOX_4_4_4(3, 1, 1));

  // test when x is odd: ([x-1,x+1], [y, y+1], [z,z+1])
  auto count2 = lattice_list.get1nn(3, 1, 1, _1nn);
  EXPECT_EQ(count2, 8);
  EXPECT_EQ(_1nn[0]->getId(), ID_BOX_4_4_4(2, 1, 1));
  EXPECT_EQ(_1nn[1]->getId(), ID_BOX_4_4_4(2, 1, 2));
  EXPECT_EQ(_1nn[2]->getId(), ID_BOX_4_4_4(2, 2, 1));
  EXPECT_EQ(_1nn[3]->getId(), ID_BOX_4_4_4(2, 2, 2));
  EXPECT_EQ(_1nn[4]->getId(), ID_BOX_4_4_4(4, 1, 1));
  EXPECT_EQ(_1nn[5]->getId(), ID_BOX_4_4_4(4, 1, 2));
  EXPECT_EQ(_1nn[6]->getId(), ID_BOX_4_4_4(4, 2, 1));
  EXPECT_EQ(_1nn[7]->getId(), ID_BOX_4_4_4(4, 2, 2));
}

TEST(lattice_list_get1nn_2, lattice_list_test) {
  Lattice *_1nn[8] = {nullptr, nullptr, nullptr, nullptr,
                      nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 8 1nn(s).
  NormalLatticeList lattice_list(4, 4, 4);
  // test low boundary
  auto count = lattice_list.get1nn(0, 0, 0, _1nn);

  EXPECT_EQ(count, 1);
  EXPECT_EQ(_1nn[7]->getId(), ID_BOX_4_4_4(1, 0, 0));
}

TEST(lattice_list_get1nn_3, lattice_list_test) {
  Lattice *_1nn[8] = {nullptr, nullptr, nullptr, nullptr,
                      nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 8 1nn(s).
  NormalLatticeList lattice_list(4, 4, 4);
  // test low boundary
  auto count = lattice_list.get1nn(2, 0, 1, _1nn);

  EXPECT_EQ(count, 4);
  EXPECT_EQ(_1nn[2]->getId(), ID_BOX_4_4_4(1, 0, 0));
  EXPECT_EQ(_1nn[3]->getId(), ID_BOX_4_4_4(1, 0, 1));
  EXPECT_EQ(_1nn[6]->getId(), ID_BOX_4_4_4(3, 0, 0));
  EXPECT_EQ(_1nn[7]->getId(), ID_BOX_4_4_4(3, 0, 1));
}

TEST(lattice_list_get1nn_4, lattice_list_test) {
  Lattice *_1nn[8]; // a lattice has at most 6 2nn(s).
  NormalLatticeList lattice_list(4, 4, 4);
  // test up boundary
  auto count = lattice_list.get1nn(7, 3, 3, _1nn);

  EXPECT_EQ(count, 1);
  EXPECT_EQ(_1nn[0]->getId(), ID_BOX_4_4_4(6, 3, 3));
}

TEST(lattice_list_get1nn_5, lattice_list_test) {
  Lattice *_1nn[8] = {nullptr, nullptr, nullptr, nullptr,
                      nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 8 1nn(s).

  NormalLatticeList lattice_list(4, 4, 4);
  // test up boundary
  auto count = lattice_list.get1nn(4, 3, 3, _1nn);

  EXPECT_EQ(count, 8);
  EXPECT_EQ(_1nn[0]->getId(), ID_BOX_4_4_4(3, 2, 2));
  EXPECT_EQ(_1nn[1]->getId(), ID_BOX_4_4_4(3, 2, 3));
  EXPECT_EQ(_1nn[2]->getId(), ID_BOX_4_4_4(3, 3, 2));
  EXPECT_EQ(_1nn[3]->getId(), ID_BOX_4_4_4(3, 3, 3));
  EXPECT_EQ(_1nn[4]->getId(), ID_BOX_4_4_4(5, 2, 2));
  EXPECT_EQ(_1nn[5]->getId(), ID_BOX_4_4_4(5, 2, 3));
  EXPECT_EQ(_1nn[6]->getId(), ID_BOX_4_4_4(5, 3, 2));
  EXPECT_EQ(_1nn[7]->getId(), ID_BOX_4_4_4(5, 3, 3));
}

// tests of get2nn
TEST(lattice_list_get2nn_1, lattice_list_test) {
  Lattice *_2nn[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 6 2nn(s).
  NormalLatticeList lattice_list(4, 4, 4);
  auto count = lattice_list.get2nn(2, 1, 1, _2nn);
  // test count
  EXPECT_EQ(count, 6);

  EXPECT_EQ(_2nn[0]->getId(), ID_BOX_4_4_4(0, 1, 1));
  EXPECT_EQ(_2nn[1]->getId(), ID_BOX_4_4_4(2, 0, 1));
  EXPECT_EQ(_2nn[2]->getId(), ID_BOX_4_4_4(2, 1, 0));
  EXPECT_EQ(_2nn[3]->getId(), ID_BOX_4_4_4(2, 1, 2));
  EXPECT_EQ(_2nn[4]->getId(), ID_BOX_4_4_4(2, 2, 1));
  EXPECT_EQ(_2nn[5]->getId(), ID_BOX_4_4_4(4, 1, 1));
}

TEST(lattice_list_get2nn_2, lattice_list_test) {
  Lattice *_2nn[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 6 2nn(s).
  NormalLatticeList lattice_list(4, 4, 4);
  auto count = lattice_list.get2nn(0, 0, 0, _2nn);
  // test count
  EXPECT_EQ(count, 3); // test low boundary

  EXPECT_EQ(_2nn[0], nullptr);
  EXPECT_EQ(_2nn[1], nullptr);
  EXPECT_EQ(_2nn[2], nullptr);
  EXPECT_EQ(_2nn[3]->getId(), ID_BOX_4_4_4(0, 0, 1));
  EXPECT_EQ(_2nn[4]->getId(), ID_BOX_4_4_4(0, 1, 0));
  EXPECT_EQ(_2nn[5]->getId(), ID_BOX_4_4_4(2, 0, 0));
}

TEST(lattice_list_get2nn_3, lattice_list_test) {
  Lattice *_2nn[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 6 2nn(s).
  NormalLatticeList lattice_list(4, 4, 4);
  auto count = lattice_list.get2nn(7, 3, 3, _2nn); // test upper boundary
  // test count
  EXPECT_EQ(count, 3);

  EXPECT_EQ(_2nn[0]->getId(), ID_BOX_4_4_4(5, 3, 3));
  EXPECT_EQ(_2nn[1]->getId(), ID_BOX_4_4_4(7, 2, 3));
  EXPECT_EQ(_2nn[2]->getId(), ID_BOX_4_4_4(7, 3, 2));
  EXPECT_EQ(_2nn[3], nullptr);
  EXPECT_EQ(_2nn[4], nullptr);
  EXPECT_EQ(_2nn[5], nullptr);
}

TEST(lattice_list_get2nn_4, lattice_list_test) {
  Lattice *_2nn[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}; // a lattice has at most 6 2nn(s).
  NormalLatticeList lattice_list(4, 4, 4);
  auto count = lattice_list.get2nn(3, 3, 3, _2nn);
  // test count
  EXPECT_EQ(count, 4);

  EXPECT_EQ(_2nn[0]->getId(), ID_BOX_4_4_4(1, 3, 3));
  EXPECT_EQ(_2nn[1]->getId(), ID_BOX_4_4_4(3, 2, 3));
  EXPECT_EQ(_2nn[2]->getId(), ID_BOX_4_4_4(3, 3, 2));
  EXPECT_EQ(_2nn[3], nullptr);
  EXPECT_EQ(_2nn[4], nullptr);
  EXPECT_EQ(_2nn[5]->getId(), ID_BOX_4_4_4(5, 3, 3));
}

TEST(lattice_list_getLatById_test, lattice_list_test) {
  NormalLatticeList lattice_list(4, 4, 4);
  // check all id in box.
  for (_type_lattice_id id = 0; id <= lattice_list.maxId(); id++) {
    auto lat = lattice_list.getLat(id);
    EXPECT_EQ(lat.getId(), id);
  }
}

TEST(lattice_list_MaxId_test, lattice_list_test) {
  NormalLatticeList lattice_list(4, 4, 4);
  // check finding max id (coord max_x: 7, coord max_y: 3, coord max_z: 3).
  auto id = lattice_list.getId(2 * 3 + 1, 3, 3);
  EXPECT_EQ(id, lattice_list.maxId());
}

TEST(lattice_list_walk_test, lattice_list_test) {
  NormalLatticeList lattice_list(4, 4, 4);
  // case 1
  Lattice *lat = lattice_list.walk(ID_BOX_4_4_4(4, 1, 1), 1, 1, 1);
  EXPECT_NE(lat, nullptr);
  EXPECT_EQ(lat->getId(), ID_BOX_4_4_4(5, 1, 1));

  Lattice *lat2 = lattice_list.walk(ID_BOX_4_4_4(5, 1, 1), 2, 2, 2);
  EXPECT_NE(lat2, nullptr);
  EXPECT_EQ(lat2->getId(), ID_BOX_4_4_4(7, 2, 2));

  // invalid input
  Lattice *lat3 = lattice_list.walk(ID_BOX_4_4_4(4, 1, 1), 1, 1, 2);
  EXPECT_EQ(lat3, nullptr);

  // invalid input
  Lattice *lat4 = lattice_list.walk(ID_BOX_4_4_4(4, 1, 1), 2, 1, 2);
  EXPECT_EQ(lat4, nullptr);

  // test out of boundary(down).
  Lattice *lat5 = lattice_list.walk(ID_BOX_4_4_4(4, 1, 1), -1, -3, -3);
  EXPECT_EQ(lat5, nullptr);

  // test out of boundary(down).
  Lattice *lat6 = lattice_list.walk(ID_BOX_4_4_4(5, 3, 3), -1, 1, 1);
  EXPECT_EQ(lat6, nullptr);
}
