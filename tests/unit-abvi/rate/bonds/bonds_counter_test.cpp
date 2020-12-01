//
// Created by genshen on 2019-01-21.
//

#include <abvi/rate/bonds/bonds_counter.h>
#include <gtest/gtest.h>
#include <lattice/normal_lattice_list.h>

class BondsCounterTester : public bonds::BondsCounter {
  FRIEND_TEST(bonds_counter_count_test, bonds_counter_test);

  FRIEND_TEST(bonds_counter_count_test_with_V, bonds_counter_test);

  FRIEND_TEST(bonds_counter_count_test_with_V_2, bonds_counter_test);
};

TEST(bonds_counter_count_test, bonds_counter_test) {
  NormalLatticeList lattice_list(4, 4, 4);
  for (unsigned int id = 0; id < 2 * 4 * 4 * 4; id++) {
    lattice_list.getLat(id).type._type = LatticeTypes::Fe;
  }

  // case 1
  auto energy1 = BondsCounterTester::count(&lattice_list, lattice_list.getId(4, 2, 2), LatticeTypes{LatticeTypes::Fe});
  EXPECT_EQ(energy1, BondsCounterTester::_1nn_bonds.at(bonds::PairBond::FeFe) * 8 +
                         BondsCounterTester::_2nn_bonds.at(bonds::PairBond::FeFe) * 6);

  // case 2
  auto energy2 = BondsCounterTester::count(&lattice_list, lattice_list.getId(0, 0, 0), LatticeTypes{LatticeTypes::Fe});
  EXPECT_EQ(energy2, BondsCounterTester::_1nn_bonds.at(bonds::PairBond::FeFe) * 1 +
                         BondsCounterTester::_2nn_bonds.at(bonds::PairBond::FeFe) * 3);
}

// the same input case as: TEST(vac_rate_test_src_1nn_neighbour,
// vac_rates_solver_tests)
TEST(bonds_counter_count_test_with_V, bonds_counter_test) {
  NormalLatticeList lattice_list(4, 4, 4);
  for (unsigned int id = 0; id < 2 * 4 * 4 * 4; id++) {
    lattice_list.getLat(id).type._type = LatticeTypes::Fe;
  }

  Lattice &src_lat = lattice_list.getLat(4, 2, 2);
  Lattice &tar_lat = lattice_list.getLat(5, 2, 2);
  Lattice &neigh_lat = lattice_list.getLat(3, 1, 2);
  neigh_lat.setType(LatticeTypes::Ni); // it is 1nn neighbour of src.
  src_lat.setType(LatticeTypes::V);

  // item:  src_1nn  | src_2nn | tar_1nn | tar_2nn
  // bonds: 7VFe+VNi   6VFe    | FeV+7FeFe 6FeFe

  // rate for src
  const double rate_src = bonds::BondsCounter::count(&lattice_list, src_lat.getId(), src_lat.type);

  const double excepted_rate_src = BondsCounterTester::_1nn_bonds.at(bonds::PairBond::VFe) * 7 +
                                   BondsCounterTester::_1nn_bonds.at(bonds::PairBond::VNi) +
                                   BondsCounterTester::_2nn_bonds.at(bonds::PairBond::VFe) * 6;
  EXPECT_DOUBLE_EQ(rate_src, excepted_rate_src);

  // rate for target/tar
  const double rate_tar = bonds::BondsCounter::count(&lattice_list, tar_lat.getId(), tar_lat.type);

  const double excepted_rate_tar = BondsCounterTester::_1nn_bonds.at(bonds::PairBond::VFe) +
                                   BondsCounterTester::_1nn_bonds.at(bonds::PairBond::FeFe) * 7 +
                                   BondsCounterTester::_2nn_bonds.at(bonds::PairBond::FeFe) * 6;
  EXPECT_DOUBLE_EQ(rate_tar, excepted_rate_tar);
}

// exchange atom of src and tar in the above test case:
// bonds_counter_count_test_with_V.
TEST(bonds_counter_count_test_with_V_2, bonds_counter_test) {
  NormalLatticeList lattice_list(4, 4, 4);
  for (unsigned int id = 0; id < 2 * 4 * 4 * 4; id++) {
    lattice_list.getLat(id).type._type = LatticeTypes::Fe;
  }

  Lattice &src_lat = lattice_list.getLat(4, 2, 2);
  Lattice &tar_lat = lattice_list.getLat(5, 2, 2);
  Lattice &neigh_lat = lattice_list.getLat(3, 1, 2);
  neigh_lat.setType(LatticeTypes::Ni); // it is 1nn neighbour of src.
  tar_lat.setType(LatticeTypes::V);

  // item:  src_1nn        | src_2nn | tar_1nn | tar_2nn
  // bonds: 6FeFe+FeV+FeNi   6FeFe   | VFe+7VFe  6VFe
  // rate for src
  const double rate_src = bonds::BondsCounter::count(&lattice_list, src_lat.getId(), src_lat.type);

  const double excepted_rate_src = BondsCounterTester::_1nn_bonds.at(bonds::PairBond::FeFe) * 6 +
                                   BondsCounterTester::_1nn_bonds.at(bonds::PairBond::VFe) +
                                   BondsCounterTester::_1nn_bonds.at(bonds::PairBond::FeNi) +
                                   BondsCounterTester::_2nn_bonds.at(bonds::PairBond::FeFe) * 6;
  EXPECT_DOUBLE_EQ(rate_src, excepted_rate_src);

  // rate for target/tar
  const double rate_tar = bonds::BondsCounter::count(&lattice_list, tar_lat.getId(), tar_lat.type);

  const double excepted_rate_tar = BondsCounterTester::_1nn_bonds.at(bonds::PairBond::VFe) +
                                   BondsCounterTester::_1nn_bonds.at(bonds::PairBond::VFe) * 7 +
                                   BondsCounterTester::_2nn_bonds.at(bonds::PairBond::VFe) * 6;
  EXPECT_DOUBLE_EQ(rate_tar, excepted_rate_tar);
}
