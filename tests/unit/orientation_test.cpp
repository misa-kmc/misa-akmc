//
// Created by genshen on 2019-01-26.
//

#include <gtest/gtest.h>
#include <orientation.h>

TEST(orientation_availTransDirsHighLow_test, orientation_test) {
    // availTransDirs() is always equals to " availTransDirsHigh | availTransDirsLow()"
    for (int i = -6; i < 6; i++) {
        const orientation ori = orientation{static_cast<orientation::_type_dire>(i)};
        EXPECT_EQ(ori.availTransDirsLow() | ori.availTransDirsHigh(), ori.availTransDirs());
    }

    for (int i = 1; i < 6; i++) {
        const orientation ori_s = orientation{static_cast<orientation::_type_dire>(i)};
        const orientation ori_r = orientation{static_cast<orientation::_type_dire>(-i)};
        EXPECT_EQ(ori_s.availTransDirsHigh(), ori_r.availTransDirsLow());
        EXPECT_EQ(ori_s.availTransDirsLow(), ori_r.availTransDirsHigh());
    }
    for (int i = 1; i < 6; i++) {
        const orientation ori_s = orientation{static_cast<orientation::_type_dire>(i)};
        const orientation ori_r = orientation{static_cast<orientation::_type_dire>(-i)};
        EXPECT_EQ(ori_s.availTransDirsLow() | ori_r.availTransDirsLow(), ori_s.availTransDirs());
        EXPECT_EQ(ori_s.availTransDirsHigh() | ori_r.availTransDirsHigh(), ori_s.availTransDirs());
    }
}

TEST(orientation_availTransDirs_test, orientation_test) {
    const orientation ori = orientation{orientation::s_110};
    EXPECT_EQ(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 0)._type, LatticeTypes::Fe);
    EXPECT_EQ(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 1)._type, LatticeTypes::Fe);
    EXPECT_EQ(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 6)._type, LatticeTypes::Cu);
    EXPECT_EQ(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 7)._type, LatticeTypes::Cu);

    EXPECT_THROW(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 2), std::runtime_error);
    EXPECT_THROW(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 3), std::runtime_error);
    EXPECT_THROW(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 4), std::runtime_error);
    EXPECT_THROW(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 5), std::runtime_error);

    const orientation ori2 = orientation{orientation::r_110};
    EXPECT_EQ(ori2.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 0)._type, LatticeTypes::Cu);
    EXPECT_EQ(ori2.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 1)._type, LatticeTypes::Cu);
    EXPECT_EQ(ori2.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 6)._type, LatticeTypes::Fe);
    EXPECT_EQ(ori2.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 7)._type, LatticeTypes::Fe);
}

TEST(orientation_trans_test, orientation_test) {
    const orientation ori = orientation{orientation::s_110};
    EXPECT_EQ(ori.trans(0, true, true)._ori, orientation::r_011);
    EXPECT_EQ(ori.trans(1, true, true)._ori, orientation::r_01_1);
}
