//
// Created by genshen on 2019-03-08.
//

#include <counter.h>
#include <gtest/gtest.h>
#include <lattice/lattice_types.h>

TEST(counter_add_test, counter_test) {
  counter c;
  c.add(LatticeTypes::Fe);
  EXPECT_EQ(c.get(LatticeTypes::Fe), 1);
  c.add(LatticeTypes::Fe);
  EXPECT_EQ(c.get(LatticeTypes::Fe), 2);

  c.set(LatticeTypes::Fe, 1024);
  c.add(LatticeTypes::Fe);
  EXPECT_EQ(c.get(LatticeTypes::Fe), 1025);

  c.set(LatticeTypes::Cu, 1024);
  c.add(LatticeTypes::Cu);
  EXPECT_EQ(c.get(LatticeTypes::Cu), 1025);
}

TEST(counter_sub_test, counter_test) {
  counter c;
  c.add(LatticeTypes::Fe);
  c.subtract(LatticeTypes::Fe);
  EXPECT_EQ(c.get(LatticeTypes::Fe), 0);

  c.set(LatticeTypes::Fe, 1024);
  c.subtract(LatticeTypes::Fe);
  EXPECT_EQ(c.get(LatticeTypes::Fe), 1023);

  c.set(LatticeTypes::Cu, 1025);
  c.subtract(LatticeTypes::Cu);
  EXPECT_EQ(c.get(LatticeTypes::Cu), 1024);
}

TEST(counter_getAtomCount_test, counter_test) {
  counter c;
  c.set(LatticeTypes::V, 1);
  c.set(LatticeTypes::Fe, 2);
  c.set(LatticeTypes::Cu, 4);
  c.set(LatticeTypes::CuCu, 8);
  c.set(LatticeTypes::CuNi, 16);
  c.set(LatticeTypes::FeFe, 32);

  EXPECT_EQ(c.getAtomCount(LatticeTypes::V), 1);
  EXPECT_EQ(c.getAtomCount(LatticeTypes::Fe), 2 + 2 * 32);
  EXPECT_EQ(c.getAtomCount(LatticeTypes::Cu), 4 + 2 * 8 + 16);
  EXPECT_EQ(c.getAtomCount(LatticeTypes::Ni), 16);
}
