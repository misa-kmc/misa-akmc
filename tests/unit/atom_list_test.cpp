//
// Created by genshen on 2018-12-11.
//

#include <gtest/gtest.h>
#include <atom_list.h>
#include "atom.h"

// test of get2nn
TEST(atom_list_get2nn_1, atom_list_test) {
    Atoms *_2nn[6]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    auto count = atom_list.get2nn(2, 1, 1, _2nn);
    // test count
    EXPECT_EQ(count, 6);

    EXPECT_EQ(_2nn[0]->getId(), 0 + 1 * 8 + 1 * 4 * 8); // lattice at (0,1,1)
    EXPECT_EQ(_2nn[1]->getId(), 2 + 0 * 8 + 1 * 4 * 8); // lattice at (2,0,1)
    EXPECT_EQ(_2nn[2]->getId(), 2 + 1 * 8 + 0 * 4 * 8); // lattice at (2,1,0)
    EXPECT_EQ(_2nn[3]->getId(), 2 + 1 * 8 + 2 * 4 * 8); // lattice at (2,1,2)
    EXPECT_EQ(_2nn[4]->getId(), 2 + 2 * 8 + 1 * 4 * 8); // lattice at (2,2,1)
    EXPECT_EQ(_2nn[5]->getId(), 4 + 1 * 8 + 1 * 4 * 8); // lattice at (4,1,1)
}

TEST(atom_list_get2nn_2, atom_list_test) {
    Atoms *_2nn[6]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    auto count = atom_list.get2nn(0, 0, 0, _2nn);
    // test count
    EXPECT_EQ(count, 3); // test low boundary

    EXPECT_EQ(_2nn[0]->getId(), 0 + 0 * 8 + 1 * 4 * 8); // lattice at (0,0,1)
    EXPECT_EQ(_2nn[1]->getId(), 0 + 1 * 8 + 0 * 4 * 8); // lattice at (0,1,0)
    EXPECT_EQ(_2nn[2]->getId(), 2 + 0 * 8 + 0 * 4 * 8); // lattice at (2,0,0)
}

TEST(atom_list_get2nn_3, atom_list_test) {
    Atoms *_2nn[6]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    auto count = atom_list.get2nn(7, 3, 3, _2nn); // test upper boundary
    // test count
    EXPECT_EQ(count, 3);

    EXPECT_EQ(_2nn[0]->getId(), 5 + 3 * 8 + 3 * 4 * 8); // lattice at (5,3,3)
    EXPECT_EQ(_2nn[1]->getId(), 7 + 2 * 8 + 3 * 4 * 8); // lattice at (7,2,3)
    EXPECT_EQ(_2nn[2]->getId(), 7 + 3 * 8 + 2 * 4 * 8); // lattice at (7,3,2)
}

TEST(atom_list_get2nn_4, atom_list_test) {
    Atoms *_2nn[6]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    auto count = atom_list.get2nn(3, 3, 3, _2nn);
    // test count
    EXPECT_EQ(count, 4);

    EXPECT_EQ(_2nn[0]->getId(), 1 + 3 * 8 + 3 * 4 * 8); // lattice at (1,3,3)
    EXPECT_EQ(_2nn[1]->getId(), 3 + 2 * 8 + 3 * 4 * 8); // lattice at (3,2,3)
    EXPECT_EQ(_2nn[2]->getId(), 3 + 3 * 8 + 2 * 4 * 8); // lattice at (3,3,2)
    EXPECT_EQ(_2nn[3]->getId(), 5 + 3 * 8 + 3 * 4 * 8); // lattice at (5,3,3)
}
