//
// Created by genshen on 2019-01-21.
//


#include <gtest/gtest.h>
#include <lattice/normal_lattice_list.h>
#include <rate/bonds/bonds_counter.h>

class BondsCounterTester : public bonds::BondsCounter {
    FRIEND_TEST(bonds_counter_count_test, bonds_counter_test);
};

TEST(bonds_counter_count_test, bonds_counter_test) {
    NormalLatticeList lattice_list(4, 4, 4);
    for (unsigned int id = 0; id < 2 * 4 * 4 * 4; id++) {
        lattice_list.getLat(id).type._type = LatticeTypes::Fe;
    }

    // case 1
    auto energy1 = BondsCounterTester::count(&lattice_list, lattice_list.getId(4, 2, 2),
                                             LatticeTypes{LatticeTypes::Fe});
    EXPECT_EQ(energy1, BondsCounterTester::_1nn_bonds.at(bonds::PairBond::FeFe) * 8 +
                       BondsCounterTester::_2nn_bonds.at(bonds::PairBond::FeFe) * 6);

    // case 2
    auto energy2 = BondsCounterTester::count(&lattice_list, lattice_list.getId(0, 0, 0),
                                             LatticeTypes{LatticeTypes::Fe});
    EXPECT_EQ(energy2, BondsCounterTester::_1nn_bonds.at(bonds::PairBond::FeFe) * 1 +
                       BondsCounterTester::_2nn_bonds.at(bonds::PairBond::FeFe) * 3);
}
