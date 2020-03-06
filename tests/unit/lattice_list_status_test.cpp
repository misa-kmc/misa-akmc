//
// Created by genshen on 2019-01-11.
//

#include <gtest/gtest.h>
#include <lattice/normal_lattice_list.h>

TEST(lattice_list_get1nnStatus, lattice_list_test) {
  NormalLatticeList lattice_list(4, 4, 4);
  EXPECT_EQ(lattice_list.get1nnStatus(2, 1, 1), 0xFF);
  EXPECT_EQ(lattice_list.get1nnStatus(2, 0, 0), 0x88); // 1000 1000
  EXPECT_EQ(lattice_list.get1nnStatus(7, 0, 0), 0x0F);
  EXPECT_EQ(lattice_list.get1nnStatus(7, 3, 3), 0x01);
}

TEST(lattice_list_get2nnStatus, lattice_list_test) {
  NormalLatticeList lattice_list(4, 4, 4);
  EXPECT_EQ(lattice_list.get2nnStatus(2, 1, 1), 0x3F);
  EXPECT_EQ(lattice_list.get2nnStatus(2, 0, 0), 0x39); // 0011 1001
  EXPECT_EQ(lattice_list.get2nnStatus(1, 0, 0), 0x38); // 0011 1000
  EXPECT_EQ(lattice_list.get2nnStatus(0, 0, 0), 0x38); // 0011 1000
  EXPECT_EQ(lattice_list.get2nnStatus(7, 0, 0), 0x19); // 0001 1001
  EXPECT_EQ(lattice_list.get2nnStatus(7, 3, 3), 0x07); // 0000 0111
}
