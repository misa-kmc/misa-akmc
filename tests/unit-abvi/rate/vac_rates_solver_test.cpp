//
// Created by genshen on 2019-03-04.
//

#include <cmath>
#include <gtest/gtest.h>

#include <abvi/rate/bonds/bonds_counter.h>
#include <abvi/rate/vacancy_rates_solver.h>
#include <lattice/normal_lattice_list.h>

class bondDataForTest : public bonds::BondsCounter {
public:
  FRIEND_TEST(vac_deltaE_test_src_1nn_neighbour, vac_rates_solver_deltaE_tests);

  FRIEND_TEST(vac_deltaE_test_both_neighbour, vac_rates_solver_deltaE_tests);
};

// test vacancy rates calculation using bonds counting.
TEST(vac_deltaE_test_null, vac_rates_solver_deltaE_tests) {
  const double attempt_freq = 6E12;
  const double temperature = 700;

  NormalLatticeList lattice_list(4, 4, 4);
  for (unsigned int id = 0; id < 2 * 4 * 4 * 4; id++) {
    lattice_list.getLat(id).type._type = LatticeTypes::Fe;
  }

  Lattice &src_lat = lattice_list.getLat(4, 1, 1);
  Lattice &tar_lat = lattice_list.getLat(5, 1, 1);
  src_lat.type._type = LatticeTypes::V;

  VacRatesSolver vac_rate(lattice_list, attempt_freq, temperature);
  // test delta e and rate.
  const auto delta_e = vac_rate.deltaE(src_lat.getId(), tar_lat.getId(), src_lat, tar_lat, tar_lat.type._type);
  ASSERT_NEAR(delta_e, 0.0, 1e-10);
  // delta E is 0, but rate may not be 0.
  const auto rate = vac_rate.rate(src_lat, tar_lat, tar_lat.type._type, 7);
  const double expected_rate = attempt_freq * exp(-vac_rate.e0(tar_lat.type._type) / (BoltzmannConstant * temperature));
  // we use relative error here, because the absolute numerical error is too
  // big(can be 1e-6).
  ASSERT_NEAR(rate / expected_rate, 1.0, 1e-10);
}

// todo more tests: src:4,3,3, target:5,3,3

// make a neighbour lattice different, who is neighbour of both src lattice and
// target lattice.
TEST(vac_deltaE_test_both_neighbour, vac_rates_solver_deltaE_tests) {
  NormalLatticeList lattice_list(6, 6, 6);
  for (unsigned int id = 0; id < 2 * 6 * 6 * 6; id++) {
    lattice_list.getLat(id).type._type = LatticeTypes::Fe;
  }

  Lattice &src_lat = lattice_list.getLat(4, 2, 2);
  Lattice &tar_lat = lattice_list.getLat(5, 2, 2);
  lattice_list.getLat(4, 2, 3).setType(LatticeTypes::Ni); // it is 2nn neighbour of src and 1nn of tar.
  src_lat.type._type = LatticeTypes::V;

  VacRatesSolver vac_rate(lattice_list, 6E12, 700);
  // bonds:  src_1nn      | src_2nn | tar_1nn | tar_2nn
  // before: 8VFe          5VFe+VNi | FeNi+7FeFe 6FeFe
  // after:  8FeFe        5FeFe+FeNi| VNi+7VFe   6VFe
  // diff: after-before = (1nnFeFe+1nnVNi+2nnFeNi+2nnVFe) - (1nnVFe + 1nnFeNi
  // +2nnVNi+2nnFeFe)

  // test delta e and rate.
  const auto delta_e = vac_rate.deltaE(src_lat.getId(), tar_lat.getId(), src_lat, tar_lat, tar_lat.type._type);
  ASSERT_NEAR(
      delta_e,
      bondDataForTest::_1nn_bonds.at(bonds::PairBond::FeFe) + bondDataForTest::_1nn_bonds.at(bonds::PairBond::VNi) +
          bondDataForTest::_2nn_bonds.at(bonds::PairBond::FeNi) + bondDataForTest::_2nn_bonds.at(bonds::PairBond::VFe) -
          bondDataForTest::_1nn_bonds.at(bonds::PairBond::VFe) - bondDataForTest::_1nn_bonds.at(bonds::PairBond::FeNi) -
          bondDataForTest::_2nn_bonds.at(bonds::PairBond::VNi) - bondDataForTest::_2nn_bonds.at(bonds::PairBond::FeFe),
      1e-10);
}

// see bonds_counter_count_test_with_V::bonds_counter_test and
// bonds_counter_count_test_with_V_2::bonds_counter_test
// for bonds tests of the same input.
TEST(vac_deltaE_test_src_1nn_neighbour, vac_rates_solver_deltaE_tests) {
  NormalLatticeList lattice_list(4, 4, 4);
  for (unsigned int id = 0; id < 2 * 4 * 4 * 4; id++) {
    lattice_list.getLat(id).type._type = LatticeTypes::Fe;
  }

  Lattice &src_lat = lattice_list.getLat(4, 2, 2);
  Lattice &tar_lat = lattice_list.getLat(5, 2, 2);
  lattice_list.getLat(3, 1, 2).setType(LatticeTypes::Ni); // it is 1nn neighbour of src.
  src_lat.type._type = LatticeTypes::V;

  VacRatesSolver vac_rate(lattice_list, 6E12, 700);
  // bonds:  src_1nn      | src_2nn | tar_1nn | tar_2nn
  // before: 7VFe+VNi       6VFe    | FeV+7FeFe 6FeFe
  // after: 6FeFe+FeV+FeNi  6FeFe   | VFe+7VFe  6VFe
  // diff: after-before = (1nnFeV + 1nnFeNi) - (1nnVNi + 1nnFeFe)

  const auto delta_e = vac_rate.deltaE(src_lat.getId(), tar_lat.getId(), src_lat, tar_lat, tar_lat.type._type);
  // use user defined precision.
  ASSERT_NEAR(
      delta_e,
      (bondDataForTest::_1nn_bonds.at(bonds::PairBond::VFe) + bondDataForTest::_1nn_bonds.at(bonds::PairBond::FeNi)) -
          (bondDataForTest::_1nn_bonds.at(bonds::PairBond::VNi) +
           bondDataForTest::_1nn_bonds.at(bonds::PairBond::FeFe)),
      1e-10);
}
