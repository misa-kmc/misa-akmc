//
// Created by genshen on 2019-03-04.
//

#include <gtest/gtest.h>
#include <lattice/normal_lattice_list.h>
#include <rate/vacancy_rates_solver.h>
#include <rate/bonds/bonds_counter.h>

class bondDataForTest : public bonds::BondsCounter {
public:
    FRIEND_TEST(vac_rate_test_src_1nn_neighbour, vac_rates_solver_tests);
};

// test vacancy rates calculation using bonds counting.
TEST(vac_rate_test, vac_rates_solver_tests) {
    NormalLatticeList lattice_list(4, 4, 4);
    for (unsigned int id = 0; id < 2 * 4 * 4 * 4; id++) {
        lattice_list.getLat(id).type._type = LatticeTypes::Fe;
    }

    Lattice &src_lat = lattice_list.getLat(4, 3, 3);
    Lattice &tar_lat = lattice_list.getLat(5, 3, 3);
    src_lat.type._type = LatticeTypes::V;

    VacRatesSolver vac_rate(lattice_list);
    const auto rate = vac_rate.rate(src_lat, tar_lat, tar_lat.type._type, 1);
    EXPECT_EQ(rate, 0.0);
}

// make a neighbour lattice different, who is neighbour of both src lattice and target lattice.
TEST(vac_rate_test_both_neighbour, vac_rates_solver_tests) {
    NormalLatticeList lattice_list(4, 4, 4);
    for (unsigned int id = 0; id < 2 * 4 * 4 * 4; id++) {
        lattice_list.getLat(id).type._type = LatticeTypes::Fe;
    }

    Lattice &src_lat = lattice_list.getLat(4, 2, 2);
    Lattice &tar_lat = lattice_list.getLat(5, 2, 2);
    lattice_list.getLat(4, 2, 3).setType(LatticeTypes::Ni); // it is both 1nn neighbour of src and tar.
    src_lat.type._type = LatticeTypes::V;

    VacRatesSolver vac_rate(lattice_list);
    const auto rate = vac_rate.rate(src_lat, tar_lat, tar_lat.type._type, 1);
    EXPECT_EQ(rate, 0.0);
}

// see bonds_counter_count_test_with_V::bonds_counter_test and
// bonds_counter_count_test_with_V_2::bonds_counter_test
// for bonds tests of the same input.
TEST(vac_rate_test_src_1nn_neighbour, vac_rates_solver_tests) {
    NormalLatticeList lattice_list(4, 4, 4);
    for (unsigned int id = 0; id < 2 * 4 * 4 * 4; id++) {
        lattice_list.getLat(id).type._type = LatticeTypes::Fe;
    }

    Lattice &src_lat = lattice_list.getLat(4, 2, 2);
    Lattice &tar_lat = lattice_list.getLat(5, 2, 2);
    lattice_list.getLat(3, 1, 2).setType(LatticeTypes::Ni); // it is 1nn neighbour of src.
    src_lat.type._type = LatticeTypes::V;

    VacRatesSolver vac_rate(lattice_list);
    // bonds:  src_1nn      | src_2nn | tar_1nn | tar_2nn
    // before: 7VFe+VNi       6VFe    | FeV+7FeFe 6FeFe
    // after: 6FeFe+FeV+FeNi  6FeFe   | VFe+7VFe  6VFe
    // diff: after-before = (1nnFeV + 1nnFeNi) - (1nnVNi + 1nnFeFe)

    const auto rate = vac_rate.deltaE(src_lat, tar_lat, tar_lat.type._type);
    // use user defined precision.
    ASSERT_NEAR(rate, (bondDataForTest::_1nn_bonds.at(bonds::PairBond::VFe) +
                       bondDataForTest::_1nn_bonds.at(bonds::PairBond::FeNi)) -
                      (bondDataForTest::_1nn_bonds.at(bonds::PairBond::VNi) +
                       bondDataForTest::_1nn_bonds.at(bonds::PairBond::FeFe)), 1e-10);
}
