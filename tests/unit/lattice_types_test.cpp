//
// Created by genshen on 2018/11/13.
//

#include <gtest/gtest.h>
#include <lattice/lattice_types.h>

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

TEST(lattice_type_combineToInter_test, lattice_type_test) {
  auto tp_new = LatticeTypes::combineToInter(LatticeTypes::Fe, LatticeTypes::Cu);
  EXPECT_EQ(tp_new, LatticeTypes::FeCu);

  auto tp_new2 = LatticeTypes::combineToInter(LatticeTypes::Cu, LatticeTypes::Fe);
  EXPECT_EQ(tp_new2, LatticeTypes::FeCu);

  auto tp_new3 = LatticeTypes::combineToInter(LatticeTypes::Mn, LatticeTypes::Mn);
  EXPECT_EQ(tp_new3, LatticeTypes::MnMn);

  auto tp_new4 = LatticeTypes::combineToInter(LatticeTypes::Cu, LatticeTypes::Mn);
  EXPECT_EQ(tp_new4, LatticeTypes::CuMn);
}

// tests for special cases of combineToInter: combine v
TEST(lattice_type_combineToInter_V_test, lattice_type_test) {
  auto tp_new = LatticeTypes::combineToInter(LatticeTypes::V, LatticeTypes::Cu);
  EXPECT_EQ(tp_new, LatticeTypes::Cu);

  auto tp_new2 = LatticeTypes::combineToInter(LatticeTypes::V, LatticeTypes::V);
  EXPECT_EQ(tp_new2, LatticeTypes::V);

  auto tp_new3 = LatticeTypes::combineToInter(LatticeTypes::Fe, LatticeTypes::V);
  EXPECT_EQ(tp_new3, LatticeTypes::Fe);
}

// tests for special cases of diff: diff v
TEST(lattice_type_diff_V_test, lattice_type_test) {
  EXPECT_EQ(LatticeTypes{LatticeTypes::V}.diff(LatticeTypes{LatticeTypes::V}), LatticeTypes::V);
  EXPECT_EQ(LatticeTypes{LatticeTypes::FeCu}.diff(LatticeTypes{LatticeTypes::V}), LatticeTypes::FeCu);
  EXPECT_EQ(LatticeTypes{LatticeTypes::Fe}.diff(LatticeTypes{LatticeTypes::Cu}),
            LatticeTypes::Fe); // not change
  EXPECT_EQ(LatticeTypes{LatticeTypes::V}.diff(LatticeTypes{LatticeTypes::Cu}), LatticeTypes::V);
}

TEST(lattice_type_getHighLowEnd_test, lattice_type_test) {
  LatticeTypes type1(LatticeTypes::FeCu);
  EXPECT_EQ(type1.getHighEnd(), LatticeTypes::Fe);
  EXPECT_EQ(type1.getLowEnd(), LatticeTypes::Cu);

  LatticeTypes type2(LatticeTypes::Cu);
  EXPECT_EQ(type2.getHighEnd(), LatticeTypes::V);
  EXPECT_EQ(type2.getLowEnd(), LatticeTypes::Cu);

  LatticeTypes type3(LatticeTypes::V);
  EXPECT_EQ(type3.getHighEnd(), LatticeTypes::V);
  EXPECT_EQ(type3.getLowEnd(), LatticeTypes::V);
}

TEST(lattice_type_isHighLowEnd_test, lattice_type_test) {
  LatticeTypes type1(LatticeTypes::FeCu);
  EXPECT_EQ(type1.isHighEnd(LatticeTypes::Fe), true);
  EXPECT_EQ(type1.isHighEnd(LatticeTypes::V), false);
  EXPECT_EQ(type1.isHighEnd(LatticeTypes::Cu), false);
  EXPECT_EQ(type1.isLowEnd(LatticeTypes::Fe), false);
  EXPECT_EQ(type1.isLowEnd(LatticeTypes::V), false);
  EXPECT_EQ(type1.isLowEnd(LatticeTypes::Cu), true);

  LatticeTypes type2(LatticeTypes::CuCu);
  EXPECT_EQ(type2.isHighEnd(LatticeTypes::Cu), true);
  EXPECT_EQ(type2.isLowEnd(LatticeTypes::Cu), true);
}

TEST(lattice_type_FirstSecondAtom_test, lattice_type_test) {
  LatticeTypes type1(LatticeTypes::FeCu);
  EXPECT_EQ(type1.getFirst(true), LatticeTypes::Fe);
  EXPECT_EQ(type1.getSecond(true), LatticeTypes::Cu);

  EXPECT_EQ(type1.getFirst(false), LatticeTypes::Cu);
  EXPECT_EQ(type1.getSecond(false), LatticeTypes::Fe);

  LatticeTypes type3(LatticeTypes::CuCu);
  EXPECT_EQ(type3.getFirst(true), LatticeTypes::Cu);
  EXPECT_EQ(type3.getSecond(true), LatticeTypes::Cu);
  EXPECT_EQ(type3.getFirst(false), LatticeTypes::Cu);
  EXPECT_EQ(type3.getSecond(false), LatticeTypes::Cu);
}

TEST(lattice_type_diff_test, lattice_type_test) {
  LatticeTypes type1(LatticeTypes::FeCu);
  LatticeTypes type2(LatticeTypes::Cu);
  EXPECT_EQ(type1.diff(type2), LatticeTypes::Fe);

  LatticeTypes type3(LatticeTypes::V);
  EXPECT_EQ(type1.diff(type3), LatticeTypes::FeCu);

  LatticeTypes type4(LatticeTypes::Ni);
  EXPECT_EQ(type1.diff(type4), LatticeTypes::FeCu);
}
