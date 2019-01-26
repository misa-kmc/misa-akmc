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

// todo refactor
// todo test
tran_orient orientation::trans(_type_dire src_orient, bool is_first_atom, bool is_lower, bool rotate) {
    switch (src_orient) {
        case dir__110:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) { // rotate
                        return tran_orient{true, dir__01_1};
                    } // rotate to another angle
                    return tran_orient{true, dir__10_1};
                } else { // dir1
                    if (!rotate) {
                        return tran_orient{true, dir__011};
                    }
                    return tran_orient{true, dir__101};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_orient{true, dir__011};
                    }
                    return tran_orient{true, dir__101};
                } else { // dir3
                    if (!rotate) {
                        return tran_orient{true, dir__01_1};
                    }
                    return tran_orient{true, dir__10_1};
                }
            }
            break;
        case dir__1_10:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) { // rotate
                        return tran_orient{false, dir__011};
                    } // rotate to another angle
                    return tran_orient{true, dir__10_1};
                } else { // dir1
                    if (!rotate) {
                        return tran_orient{false, dir__01_1};
                    }
                    return tran_orient{true, dir__101};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_orient{false, dir__01_1};
                    }
                    return tran_orient{true, dir__101};
                } else { // dir3
                    if (!rotate) {
                        return tran_orient{false, dir__011};
                    }
                    return tran_orient{true, dir__10_1};
                }
            }
            break;
        case dir__011:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) {
                        return tran_orient{false, dir__10_1};
                    }
                    return tran_orient{false, dir__1_10};
                } else { // dir1
                    if (!rotate) {
                        return tran_orient{true, dir__110};
                    }
                    return tran_orient{true, dir__101};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_orient{true, dir__110};
                    }
                    return tran_orient{true, dir__101};
                } else { // dir3
                    if (!rotate) {
                        return tran_orient{false, dir__10_1};
                    }
                    return tran_orient{false, dir__1_10};
                }
            }
            break;
        case dir__01_1:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) {
                        return tran_orient{false, dir__1_10};
                    }
                    return tran_orient{false, dir__101};
                } else { // dir1
                    if (!rotate) {
                        return tran_orient{true, dir__110};
                    }
                    return tran_orient{true, dir__10_1};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_orient{true, dir__110};
                    }
                    return tran_orient{true, dir__10_1};
                } else { // dir3
                    if (!rotate) {
                        return tran_orient{false, dir__1_10};
                    }
                    return tran_orient{false, dir__101};
                }
            }
            break;
        case dir__101:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) {
                        return tran_orient{false, dir__01_1};
                    }
                    return tran_orient{true, dir__1_10};
                } else { // dir1
                    if (!rotate) {
                        return tran_orient{true, dir__011};
                    }
                    return tran_orient{true, dir__110};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_orient{true, dir__011};
                    }
                    return tran_orient{true, dir__110};
                } else { // dir3
                    if (!rotate) {
                        return tran_orient{false, dir__01_1};
                    }
                    return tran_orient{true, dir__1_10};
                }
            }
            break;
        case dir__10_1:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) {
                        return tran_orient{false, dir__011};
                    }
                    return tran_orient{true, dir__1_10};
                } else { // dir1
                    if (!rotate) {
                        return tran_orient{true, dir__01_1};
                    }
                    return tran_orient{true, dir__110};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_orient{true, dir__01_1};
                    }
                    return tran_orient{true, dir__110};
                } else { // dir3
                    if (!rotate) {
                        return tran_orient{false, dir__011};
                    }
                    return tran_orient{true, dir__1_10};
                }
            }
            break;
    }
}

// todo test
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
#ifdef DEBUG_MODE
            throw std::runtime_error("unexpected 1nn tag");
#endif
            return type;
    }
}
