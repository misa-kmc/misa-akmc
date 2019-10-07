//
// Created by genshen on 2019/10/8.
//

#include <array>
#include <gtest/gtest.h>
#include <algorithms/ring.hpp>

TEST(ring_test_next, ring_test) {
    ring<int, 2> r{{1, 2}};
    EXPECT_EQ(r.next(), 2);
    EXPECT_EQ(r.next(), 1);
    EXPECT_EQ(r.next(), 2);
    EXPECT_EQ(r.next(), 1);

    ring<int, 1> r2{{1}};
    EXPECT_EQ(r2.next(), 1);
    EXPECT_EQ(r2.next(), 1);
    EXPECT_EQ(r2.next(), 1);
}

TEST(ring_test_next_itl, ring_test) {
    ring<int, 2> r{{1, 2}};
    EXPECT_EQ(r.get(), 1);
    EXPECT_EQ((++r).get(), 2);
    EXPECT_EQ((++r).get(), 1);
}

TEST(ring_test_get, ring_test) {
    ring<int, 2> r{{1, 2}};
    EXPECT_EQ(r.get(), r());
    EXPECT_EQ(r(), 1);
    ++r;
    EXPECT_EQ(r.get(), r());
    EXPECT_EQ(r(), 2);
}

TEST(ring_test_size, ring_test) {
    ring<int, 2> r{{1, 2}};
    EXPECT_EQ(r.size(), 2);
}
