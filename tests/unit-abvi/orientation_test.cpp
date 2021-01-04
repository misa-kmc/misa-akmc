//
// Created by genshen on 2019-01-26.
//

#include <abvi/orientation.h>
#include <gtest/gtest.h>

TEST(orientation_availTransDirsHighLow_test, orientation_test) {
  // availTransDirs() is always equals to " availTransDirsHigh |
  // availTransDirsLow()"
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

  // fixme this only be true in debug mode.
#ifdef KMC_DEBUG_MODE
  EXPECT_THROW(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 2), std::runtime_error);
  EXPECT_THROW(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 3), std::runtime_error);
  EXPECT_THROW(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 4), std::runtime_error);
  EXPECT_THROW(ori.tranAtom(LatticeTypes{LatticeTypes::FeCu}, 5), std::runtime_error);
#endif

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

  const orientation ori2 = orientation{orientation::s_011}; // s_011 = 2
  // it will find TransHashTable[s_011-1][1nn_tag][!new_higher][!rotate] =
  // TransHashTable[2][3][0][1]
  EXPECT_EQ(ori2.trans(3, true, false)._ori, orientation::r_1_10);
}

// test random orientation generation
TEST(orientation_random_test, orientation_test) {
  for (int i = 0; i < 100; i++) {
    orientation orient = orientation{orientation::random()};
    // all orients are belongs to [r_10_1,s_10_1] but not equal unknown.
    EXPECT_NE(orient._ori, orientation::unknown);
    EXPECT_LE(orient._ori, orientation::s_10_1);
    EXPECT_GE(orient._ori, orientation::r_10_1);
  }
}
