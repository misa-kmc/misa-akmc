//
// Created by genshen on 2019-01-10.
//

#include "orientation.h"

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
_type_dirs_status orientation::availTransDirections() const {
    switch (_ori) {
        case dir__110:
            return 0xC3;
        case dir__1_10:
            return 0x3C;
        case dir__011:
            return 0x99;
        case dir__01_1:
            return 0x66;
        case dir__101:
            return 0xA5;
        case dir__10_1:
            return 0x5A;
    }
}
