//
// Created by genshen on 2019-01-12.
//

#include <defect/itl_list.h>
#include <gtest/gtest.h>

class ItlTester : public Itl {
public:
    FRIEND_TEST(itl_availTranDirs_test, itl_test);

    FRIEND_TEST(itl_availTranDirs_status_test, itl_test);

    FRIEND_TEST(itl_ratesIndex_test, itl_test);
};

TEST(itl_availTranDirs_test, itl_test) {
    ItlTester itl;
    itl.direction.dir._d = dirs::dir__110;
    itl.direction.reversed = false;

    _type_neighbour_status nei_status = 0xFF;
    Lattice data[8];
    Lattice *_1nn[8] = {&data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7]};
    data[0].type._type = LatticeTypes::Fe;
    data[1].type._type = LatticeTypes::Fe;
    data[2].type._type = LatticeTypes::Fe;
    data[3].type._type = LatticeTypes::Fe;
    data[4].type._type = LatticeTypes::Fe;
    data[5].type._type = LatticeTypes::Fe;
    data[6].type._type = LatticeTypes::Fe;
    data[7].type._type = LatticeTypes::Fe;

    // case 1
    itl.availTranDirs(nei_status, _1nn);
    EXPECT_EQ(itl.avail_trans_dir, 0xC3); // 0b 1100 0011

    // case 2
    data[0].type._type = LatticeTypes::FeFe; // not single atom.
    itl.availTranDirs(nei_status, _1nn);
    EXPECT_EQ(itl.avail_trans_dir, 0xC2); // 0b 1100 0010

    // case 3
    data[0].type._type = LatticeTypes::V; // not single atom.
    itl.availTranDirs(nei_status, _1nn);
    EXPECT_EQ(itl.avail_trans_dir, 0xC2); // 0b 1100 0010

    // case 4
    data[0].type._type = LatticeTypes::Fe; // recover
    data[3].type._type = LatticeTypes::FeCu;
    itl.availTranDirs(nei_status, _1nn);
    EXPECT_EQ(itl.avail_trans_dir, 0xC3); // no effect, 0b 1100 0010
    data[3].type._type = LatticeTypes::Fe;
}


TEST(itl_availTranDirs_status_test, itl_test) {
    ItlTester itl;
    itl.direction.dir._d = dirs::dir__110;
    itl.direction.reversed = false;

    Lattice data[8];
    data[0].type._type = LatticeTypes::Fe;
    data[1].type._type = LatticeTypes::FeCu;
    data[2].type._type = LatticeTypes::Fe;
    data[3].type._type = LatticeTypes::Fe;
    data[4].type._type = LatticeTypes::Fe;
    data[5].type._type = LatticeTypes::Fe;
    data[6].type._type = LatticeTypes::FeCu;
    data[7].type._type = LatticeTypes::Fe;

    // case 1
    _type_neighbour_status nei_status = 0xFF;
    Lattice *_1nn_1[8] = {&data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7]};
    itl.availTranDirs(nei_status, _1nn_1);
    EXPECT_EQ(itl.avail_trans_dir, 0x81);

    // case 2
    nei_status = 0xF0; // 4 valid neighbour lattices.
    Lattice *_1nn_2[8] = {nullptr, nullptr, nullptr, nullptr, &data[4], &data[5], &data[6], &data[7]};
    itl.availTranDirs(nei_status, _1nn_2);
    EXPECT_EQ(itl.avail_trans_dir, 0x80); // 0b 1000 0000

    // case 3
    nei_status = 0xF3; // 6 valid neighbour lattices.
    Lattice *_1nn_3[8] = {&data[0], &data[1], nullptr, nullptr, &data[4], &data[5], &data[6], &data[7]};
    itl.availTranDirs(nei_status, _1nn_3);
    EXPECT_EQ(itl.avail_trans_dir, 0x81); // 0b 1000 0001

    // case 4
    nei_status = 0xFC; // 6 valid neighbour lattices.
    Lattice *_1nn_4[8] = {nullptr, nullptr, &data[2], &data[3], &data[4], &data[5], &data[6], &data[7]};
    itl.availTranDirs(nei_status, _1nn_4);
    EXPECT_EQ(itl.avail_trans_dir, 0x80);

    // case 5
    nei_status = 0x0C; // 2 valid neighbour lattices.
    Lattice *_1nn_5[8] = {nullptr, nullptr, &data[2], &data[3], nullptr, nullptr, nullptr, nullptr,};
    itl.availTranDirs(nei_status, _1nn_5);
    EXPECT_EQ(itl.avail_trans_dir, 0x00);
}

TEST(itl_ratesIndex_test, itl_test) {
    // 0xC3 0b 1100 0011
    EXPECT_EQ(ItlTester::ratesIndex(0, 0xC3, false), 0);
    EXPECT_EQ(ItlTester::ratesIndex(0, 0xC3, true), 1);

    EXPECT_EQ(ItlTester::ratesIndex(1, 0xC3, false), 2);
    EXPECT_EQ(ItlTester::ratesIndex(1, 0xC3, true), 3);

    // better undefined behavior (use latter index)
    EXPECT_EQ(ItlTester::ratesIndex(2, 0xC3, false), 4);
    EXPECT_EQ(ItlTester::ratesIndex(2, 0xC3, true), 5);
    EXPECT_EQ(ItlTester::ratesIndex(3, 0xC3, false), 4);
    EXPECT_EQ(ItlTester::ratesIndex(3, 0xC3, true), 5);

    EXPECT_EQ(ItlTester::ratesIndex(6, 0xC3, false), 4);
    EXPECT_EQ(ItlTester::ratesIndex(6, 0xC3, true), 5);

    EXPECT_EQ(ItlTester::ratesIndex(7, 0xC3, false), 6);
    EXPECT_EQ(ItlTester::ratesIndex(7, 0xC3, true), 7);

    // 0xC0 0b 1100 0000
    EXPECT_EQ(ItlTester::ratesIndex(0, 0xC0, false), 0);
    EXPECT_EQ(ItlTester::ratesIndex(0, 0xC0, true), 1);

    EXPECT_EQ(ItlTester::ratesIndex(6, 0xC0, false), 0);
    EXPECT_EQ(ItlTester::ratesIndex(6, 0xC0, true), 1);
    EXPECT_EQ(ItlTester::ratesIndex(7, 0xC0, false), 2);
    EXPECT_EQ(ItlTester::ratesIndex(7, 0xC0, true), 3);
}
