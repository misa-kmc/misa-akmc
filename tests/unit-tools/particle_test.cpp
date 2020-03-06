//
// Created by genshen on 2020/1/27.
//

#include "particle.h"
#include <gtest/gtest.h>

TEST(particle_test, particle_test_create) {
  particle p("Fe\t2.0\t2.5\t2.5");
  EXPECT_EQ(p.type, LatticeTypes::Fe);
  EXPECT_EQ(p.x, 4);
  EXPECT_EQ(p.y, 2);
  EXPECT_EQ(p.z, 2);
}
