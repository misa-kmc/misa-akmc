//
// Created by genshen on 2019-01-10.
//

#include "orientation.h"

const char orientation::JumpAtomHashTable[orientation::orientation_count][LatticesList::MAX_1NN]{
    {_, Y, _, Y, X, _, X, _}, // -6: -<10-1>
    {Y, _, Y, _, _, X, _, X}, // -5: -<101>
    {_, Y, X, _, _, Y, X, _}, // -4: -<01-1>
    {Y, _, _, X, Y, _, _, X}, // -3: -<011>
    {_, _, Y, Y, X, X, _, _}, // -2: -<1-10>
    {Y, Y, _, _, _, _, X, X}, // -1: -<110>
    {_, _, _, _, _, _, _, _}, // 0: unknown
    {X, X, _, _, _, _, Y, Y}, // 1: <110>
    {_, _, X, X, Y, Y, _, _}, // 2: <1-10>
    {X, _, _, Y, X, _, _, Y}, // 3: <011>
    {_, X, Y, _, _, X, Y, _}, // 4: <01-1>
    {X, _, X, _, _, Y, _, Y}, // 5: <101>
    {_, X, _, X, Y, _, Y, _}, // 6: <10-1>
};

const orientation::_type_dire orientation::TransHashTable[single_orient_count][LatticesList::MAX_1NN][2][2]{
    // we only handle a half transition table,
    // because the negative part is the same as positive part.
    // 1: <110>
    {// 1nn 0
     {{r_011, r_101}, {s_011, s_101}},
     // 1nn 1
     {{r_01_1, r_10_1}, {s_01_1, s_10_1}},
     // 1nn 2
     {}, // default is 0: unknown
     // 1nn 3
     {},
     // 1nn 4
     {},
     // 1nn 5
     {},
     // 1nn 6
     {{s_01_1, s_10_1}, {r_01_1, r_10_1}},
     // 1nn 7
     {{s_011, s_101}, {r_011, r_101}}},

    // 2: <1-10>
    {// 1nn 0
     {},
     // 1nn 1
     {},
     // 1nn 2
     {{s_01_1, r_101}, {r_01_1, s_101}},
     // 1nn 3
     {{s_011, r_10_1}, {r_011, s_10_1}},
     // 1nn 4
     {{r_011, s_10_1}, {s_011, r_10_1}},
     // 1nn 5
     {{r_01_1, s_101}, {s_01_1, r_101}},
     // 1nn 6
     {},
     // 1nn 7
     {}},

    // 3: <011>
    {// 1nn 0
     {{r_110, r_101}, {s_110, s_101}},
     // 1nn 1
     {},
     // 1nn 2
     {},
     // 1nn 3
     {{r_10_1, r_1_10}, {s_10_1, s_1_10}},
     // 1nn 4
     {{s_10_1, s_1_10}, {r_10_1, r_1_10}},
     // 1nn 5
     {},
     // 1nn 6
     {},
     // 1nn 7
     {{s_110, s_101}, {r_110, r_101}}},

    // 4: <01-1>
    {// 1nn 0
     {},
     // 1nn 1
     {{r_110, r_10_1}, {s_110, s_10_1}},
     // 1nn 2
     {{r_1_10, r_101}, {s_1_10, s_101}},
     // 1nn 3
     {},
     // 1nn 4
     {},
     // 1nn 5
     {{s_1_10, s_101}, {r_1_10, r_101}},
     // 1nn 6
     {{s_110, s_10_1}, {r_110, r_10_1}},
     // 1nn 7
     {}},

    // 5: <101>
    {// 1nn 0
     {{r_011, r_110}, {s_011, s_110}},
     // 1nn 1
     {},
     // 1nn 2
     {{r_01_1, s_1_10}, {s_01_1, r_1_10}},
     // 1nn 3
     {},
     // 1nn 4
     {},
     // 1nn 5
     {{s_01_1, r_1_10}, {r_01_1, s_1_10}},
     // 1nn 6
     {},
     // 1nn 7
     {{s_011, s_110}, {r_011, r_110}}},

    // 6: <10-1>
    {// 1nn 0
     {},
     // 1nn 1
     {{r_01_1, r_110}, {s_01_1, s_110}},
     // 1nn 2
     {},
     // 1nn 3
     {{r_011, s_1_10}, {s_011, r_1_10}},
     // 1nn 4
     {{s_011, r_1_10}, {r_011, s_1_10}},
     // 1nn 5
     {},
     // 1nn 6
     {{s_01_1, s_110}, {r_01_1, r_110}},
     // 1nn 7
     {}},
};

_type_dirs_status orientation::availTransDirs() const {
  switch (_ori) {
  case s_110:
  case r_110:
    return 0xC3;
  case s_1_10:
  case r_1_10:
    return 0x3C;
  case s_011:
  case r_011:
    return 0x99;
  case s_01_1:
  case r_01_1:
    return 0x66;
  case s_101:
  case r_101:
    return 0xA5;
  case s_10_1:
  case r_10_1:
    return 0x5A;
  case unknown:
    return 0;
  }
  return 0;
}

_type_dirs_status orientation::availTransDirsHigh() const {
  // assuming dumbbell type is XY
  // returns atom X's available transition directions
  switch (_ori) {
  case s_110:
    return 0x03;
  case r_110:
    return 0xC0;
  case s_1_10:
    return 0x0C;
  case r_1_10:
    return 0x30;
  case s_011:
    return 0x11;
  case r_011:
    return 0x88;
  case s_01_1:
    return 0x22;
  case r_01_1:
    return 0x44;
  case s_101:
    return 0x05;
  case r_101:
    return 0xA0;
  case s_10_1:
    return 0x0A;
  case r_10_1:
    return 0x50;
  case unknown:
    return 0;
  }
  return 0;
}

_type_dirs_status orientation::availTransDirsLow() const {
  // atom Y's available transition directions
  switch (_ori) {
  case s_110:
    return 0xC0;
  case r_110:
    return 0x03;
  case s_1_10:
    return 0x30;
  case r_1_10:
    return 0x0C;
  case s_011:
    return 0x88;
  case r_011:
    return 0x11;
  case s_01_1:
    return 0x44;
  case r_01_1:
    return 0x22;
  case s_101:
    return 0xA0;
  case r_101:
    return 0x05;
  case s_10_1:
    return 0x50;
  case r_10_1:
    return 0x0A;
  case unknown:
    return 0;
  }
  return 0;
}

LatticeTypes orientation::tranAtom(const LatticeTypes type, const _type_dir_id _1nn_tag) const {
  // assuming the dumbbell type is XY, findJumpAtom will returns which atom should jump.
  switch (findJumpAtom(_1nn_tag)) {
  case X: // return X in XY, higher bits in type
    return LatticeTypes{type.getHighEnd()};
  case Y: // return Y in XY, lower bits in type
    return LatticeTypes{type.getLowEnd()};
  case _:
  default:
#ifdef KMC_DEBUG_MODE
    throw std::runtime_error("unexpected 1nn tag");
#endif
    return type;
  }
}

// todo test
orientation orientation::trans(const _type_dir_id _1nn_tag, const bool new_higher, const bool rotate) const {
  if (_ori == 0) {
#ifdef KMC_DEBUG_MODE
    throw std::runtime_error("unexpected 1nn tag");
#endif
    return orientation{unknown};
  }
  return orientation{TransHashTable[tranHash(_ori)][_1nn_tag][!new_higher][!rotate]};
}
