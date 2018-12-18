//
// Created by genshen on 2018-12-11.
//

#include <gtest/gtest.h>
#include <atom_list.h>
#include "atom.h"

#define ID_BOX_4_4_4(x, y, z) (x + y*8+z*4*8)

// tests of get1nn
TEST(atom_list_get1nn_1, atom_list_test) {
    Atoms *_1nn[8]; // a lattice has at most 8 1nn(s).
    AtomList atom_list(4, 4, 4);
    // test when x is even: ([x-1,x+1], [y-1, y], [z-1,z])
    auto count = atom_list.get1nn(2, 1, 1, _1nn);
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
    auto count2 = atom_list.get1nn(3, 1, 1, _1nn);
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

TEST(atom_list_get1nn_2, atom_list_test) {
    Atoms *_1nn[8]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    // test low boundary
    auto count = atom_list.get1nn(0, 0, 0, _1nn);

    EXPECT_EQ(count, 1);
    EXPECT_EQ(_1nn[0]->getId(), ID_BOX_4_4_4(1, 0, 0));
}

TEST(atom_list_get1nn_3, atom_list_test) {
    Atoms *_1nn[8]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    // test low boundary
    auto count = atom_list.get1nn(2, 0, 1, _1nn);

    EXPECT_EQ(count, 4);
    EXPECT_EQ(_1nn[0]->getId(), ID_BOX_4_4_4(1, 0, 0));
    EXPECT_EQ(_1nn[1]->getId(), ID_BOX_4_4_4(1, 0, 1));
    EXPECT_EQ(_1nn[2]->getId(), ID_BOX_4_4_4(3, 0, 0));
    EXPECT_EQ(_1nn[3]->getId(), ID_BOX_4_4_4(3, 0, 1));
}

TEST(atom_list_get1nn_4, atom_list_test) {
    Atoms *_1nn[8]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    // test up boundary
    auto count = atom_list.get1nn(7, 3, 3, _1nn);

    EXPECT_EQ(count, 1);
    EXPECT_EQ(_1nn[0]->getId(), ID_BOX_4_4_4(6, 3, 3));
}

TEST(atom_list_get1nn_5, atom_list_test) {
    Atoms *_1nn[8]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    // test up boundary
    auto count = atom_list.get1nn(4, 3, 3, _1nn);

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
TEST(atom_list_get2nn_1, atom_list_test) {
    Atoms *_2nn[6]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    auto count = atom_list.get2nn(2, 1, 1, _2nn);
    // test count
    EXPECT_EQ(count, 6);

    EXPECT_EQ(_2nn[0]->getId(), ID_BOX_4_4_4(0, 1, 1));
    EXPECT_EQ(_2nn[1]->getId(), ID_BOX_4_4_4(2, 0, 1));
    EXPECT_EQ(_2nn[2]->getId(), ID_BOX_4_4_4(2, 1, 0));
    EXPECT_EQ(_2nn[3]->getId(), ID_BOX_4_4_4(2, 1, 2));
    EXPECT_EQ(_2nn[4]->getId(), ID_BOX_4_4_4(2, 2, 1));
    EXPECT_EQ(_2nn[5]->getId(), ID_BOX_4_4_4(4, 1, 1));
}

TEST(atom_list_get2nn_2, atom_list_test) {
    Atoms *_2nn[6]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    auto count = atom_list.get2nn(0, 0, 0, _2nn);
    // test count
    EXPECT_EQ(count, 3); // test low boundary

    EXPECT_EQ(_2nn[0]->getId(), ID_BOX_4_4_4(0, 0, 1));
    EXPECT_EQ(_2nn[1]->getId(), ID_BOX_4_4_4(0, 1, 0));
    EXPECT_EQ(_2nn[2]->getId(), ID_BOX_4_4_4(2, 0, 0));
}

TEST(atom_list_get2nn_3, atom_list_test) {
    Atoms *_2nn[6]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    auto count = atom_list.get2nn(7, 3, 3, _2nn); // test upper boundary
    // test count
    EXPECT_EQ(count, 3);

    EXPECT_EQ(_2nn[0]->getId(), ID_BOX_4_4_4(5, 3, 3));
    EXPECT_EQ(_2nn[1]->getId(), ID_BOX_4_4_4(7, 2, 3));
    EXPECT_EQ(_2nn[2]->getId(), ID_BOX_4_4_4(7, 3, 2));
}

TEST(atom_list_get2nn_4, atom_list_test) {
    Atoms *_2nn[6]; // a lattice has at most 6 2nn(s).
    AtomList atom_list(4, 4, 4);
    auto count = atom_list.get2nn(3, 3, 3, _2nn);
    // test count
    EXPECT_EQ(count, 4);

    EXPECT_EQ(_2nn[0]->getId(), ID_BOX_4_4_4(1, 3, 3));
    EXPECT_EQ(_2nn[1]->getId(), ID_BOX_4_4_4(3, 2, 3));
    EXPECT_EQ(_2nn[2]->getId(), ID_BOX_4_4_4(3, 3, 2));
    EXPECT_EQ(_2nn[3]->getId(), ID_BOX_4_4_4(5, 3, 3));
}
