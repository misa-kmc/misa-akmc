//
// Created by genshen on 2018/11/13.
//

#include <gtest/gtest.h>
#include <lattice_types.h>

TEST(lattice_type_test_case1, lattice_type_test) {
    LatticeTypes type(LatticeTypes::Fe);
    EXPECT_EQ(type.isAtom(), true);
    EXPECT_EQ(type.isDumbbell(), false);

    LatticeTypes type2(LatticeTypes::FeFe);
    EXPECT_EQ(type2.isAtom(), false);
    EXPECT_EQ(type2.isDumbbell(), true);

    LatticeTypes type3(LatticeTypes::V);
    EXPECT_EQ(type3.isAtom(), false);
    EXPECT_EQ(type3.isDumbbell(), false);
}
