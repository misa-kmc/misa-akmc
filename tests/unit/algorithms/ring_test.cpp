//
// Created by genshen on 2019/10/8.
//

#include <algorithms/ring.hpp>
#include <array>
#include <gtest/gtest.h>

TEST(ring_itl_test_next, ring_test) {
  ring<int, 2> r{{1, 2}};
  ring<int, 2>::iterator itr = r.begin();

  EXPECT_EQ(itr.next(), 2);
  ++itr;
  EXPECT_EQ(itr.next(), 1);
  EXPECT_EQ(itr.next(), 1);
  ++itr;
  EXPECT_EQ(itr.next(), 2);

  ring<int, 1> r2{{1}};
  ring<int, 1>::iterator itr2 = r2.begin();
  ++itr2;
  EXPECT_EQ(*itr, 1);
  ++itr2;
  EXPECT_EQ(*itr, 1);
  ++itr2;
  EXPECT_EQ(*itr, 1);
}

TEST(ring_itl_test_assignment, ring_test) {
  ring<int, 2> r{{1, 2}};
  ring<int, 2>::iterator itr = r.begin();
  ++itr;

  ring<int, 2>::iterator itr2 = itr;
  EXPECT_EQ(*itr, 2);
  ++itr;
  EXPECT_EQ(*itr, 1);
}

TEST(ring_itl_test_assignment2, ring_test) {
  ring<int, 2> r{{1, 2}};
  ring<int, 2>::iterator itr = r.begin();
  ++itr;

  ring<int, 2> r2{{3, 4}};
  itr = r2.begin();
  EXPECT_EQ(*itr, 3);
  ++itr;
  EXPECT_EQ(*itr, 4);
}

TEST(ring_test_itl_compare, ring_test) {
  ring<int, 2> r{{1, 2}};
  ring<int, 2>::iterator itr = r.begin();
  ring<int, 2>::iterator itr2 = r.begin();
  EXPECT_TRUE(itr == itr2);

  ring<int, 2> r2{{1, 2}};
  ring<int, 2>::iterator itr3 = r2.begin();
  EXPECT_FALSE(itr == itr3);
  EXPECT_TRUE(itr != itr3);
}

TEST(ring_test_size, ring_test) {
  ring<int, 2> r{{1, 2}};
  EXPECT_EQ(r.size(), 2);
}
