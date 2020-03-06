//
// Created by genshen on 2019/12/10.
//

#include <gtest/gtest.h>
#include <lattice/lattice_types.h>

TEST(lattice_types_random_test, lattice_types_test) {
  const std::vector<LatticeTypes::lat_type> types{
      LatticeTypes::V, LatticeTypes::Fe, LatticeTypes::Cu, LatticeTypes::Mn};
  const std::vector<unsigned int> types_ratio{4, 4, 8, 8};
  const unsigned int ratio_sum = 24;
  const LatticeTypes::lat_type default_type = LatticeTypes::Fe;

  // 1,2,3,4
  for (unsigned int i = 1; i <= 4; i++) {
    auto _type = LatticeTypes::randomAtomsType(types.data(), types_ratio.data(),
                                               types.size(), i);
    EXPECT_EQ(_type, LatticeTypes::V);
  }
  // 5,6,7,8
  for (unsigned int i = 5; i <= 8; i++) {
    auto _type = LatticeTypes::randomAtomsType(types.data(), types_ratio.data(),
                                               types.size(), i);
    EXPECT_EQ(_type, LatticeTypes::Fe);
  }
  // 9, 10, ..., 16
  for (unsigned int i = 9; i <= 16; i++) {
    auto _type = LatticeTypes::randomAtomsType(types.data(), types_ratio.data(),
                                               types.size(), i);
    EXPECT_EQ(_type, LatticeTypes::Cu);
  }
  // 17,..., 24
  for (unsigned int i = 17; i <= 24; i++) {
    auto _type = LatticeTypes::randomAtomsType(types.data(), types_ratio.data(),
                                               types.size(), i);
    EXPECT_EQ(_type, LatticeTypes::Mn);
  }

  auto _type_over = LatticeTypes::randomAtomsType(
      types.data(), types_ratio.data(), types.size(), 25);
  EXPECT_EQ(_type_over, default_type);
}

TEST(lattice_types_random_zero_prefix_test, lattice_types_test) {
  const std::vector<LatticeTypes::lat_type> types{
      LatticeTypes::V, LatticeTypes::Fe, LatticeTypes::Cu, LatticeTypes::Mn};
  const std::vector<unsigned int> types_ratio{0, 0, 8, 8};

  // 1,2,...,8
  for (unsigned int i = 1; i <= 8; i++) {
    auto _type = LatticeTypes::randomAtomsType(types.data(), types_ratio.data(),
                                               types.size(), i);
    EXPECT_EQ(_type, LatticeTypes::Cu);
  }
  // 9, 10, ..., 16
  for (unsigned int i = 9; i <= 16; i++) {
    auto _type = LatticeTypes::randomAtomsType(types.data(), types_ratio.data(),
                                               types.size(), i);
    EXPECT_EQ(_type, LatticeTypes::Mn);
  }
}
