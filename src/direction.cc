//
// Created by genshen on 2019-01-10.
//

#include "direction.h"

// todo refactor
tran_dir dirs::trans(_type_dire src_dir, bool is_first_atom, bool is_lower, bool rotate) {
    switch (src_dir) {
        case dir__110:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) { // rotate
                        return tran_dir{true, dir__01_1};
                    } // rotate to another angle
                    return tran_dir{true, dir__10_1};
                } else { // dir1
                    if (!rotate) {
                        return tran_dir{true, dir__011};
                    }
                    return tran_dir{true, dir__101};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_dir{true, dir__011};
                    }
                    return tran_dir{true, dir__101};
                } else { // dir3
                    if (!rotate) {
                        return tran_dir{true, dir__01_1};
                    }
                    return tran_dir{true, dir__10_1};
                }
            }
            break;
        case dir__1_10:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) { // rotate
                        return tran_dir{false, dir__011};
                    } // rotate to another angle
                    return tran_dir{true, dir__10_1};
                } else { // dir1
                    if (!rotate) {
                        return tran_dir{false, dir__01_1};
                    }
                    return tran_dir{true, dir__101};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_dir{false, dir__01_1};
                    }
                    return tran_dir{true, dir__101};
                } else { // dir3
                    if (!rotate) {
                        return tran_dir{false, dir__011};
                    }
                    return tran_dir{true, dir__10_1};
                }
            }
            break;
        case dir__011:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) {
                        return tran_dir{false, dir__10_1};
                    }
                    return tran_dir{false, dir__1_10};
                } else { // dir1
                    if (!rotate) {
                        return tran_dir{true, dir__110};
                    }
                    return tran_dir{true, dir__101};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_dir{true, dir__110};
                    }
                    return tran_dir{true, dir__101};
                } else { // dir3
                    if (!rotate) {
                        return tran_dir{false, dir__10_1};
                    }
                    return tran_dir{false, dir__1_10};
                }
            }
            break;
        case dir__01_1:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) {
                        return tran_dir{false, dir__1_10};
                    }
                    return tran_dir{false, dir__101};
                } else { // dir1
                    if (!rotate) {
                        return tran_dir{true, dir__110};
                    }
                    return tran_dir{true, dir__10_1};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_dir{true, dir__110};
                    }
                    return tran_dir{true, dir__10_1};
                } else { // dir3
                    if (!rotate) {
                        return tran_dir{false, dir__1_10};
                    }
                    return tran_dir{false, dir__101};
                }
            }
            break;
        case dir__101:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) {
                        return tran_dir{false, dir__01_1};
                    }
                    return tran_dir{true, dir__1_10};
                } else { // dir1
                    if (!rotate) {
                        return tran_dir{true, dir__011};
                    }
                    return tran_dir{true, dir__110};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_dir{true, dir__011};
                    }
                    return tran_dir{true, dir__110};
                } else { // dir3
                    if (!rotate) {
                        return tran_dir{false, dir__01_1};
                    }
                    return tran_dir{true, dir__1_10};
                }
            }
            break;
        case dir__10_1:
            if (is_first_atom) {
                if (is_lower) { // dir2
                    if (!rotate) {
                        return tran_dir{false, dir__011};
                    }
                    return tran_dir{true, dir__1_10};
                } else { // dir1
                    if (!rotate) {
                        return tran_dir{true, dir__01_1};
                    }
                    return tran_dir{true, dir__110};
                }
            } else {
                if (is_lower) { // dir4
                    if (!rotate) {
                        return tran_dir{true, dir__01_1};
                    }
                    return tran_dir{true, dir__110};
                } else { // dir3
                    if (!rotate) {
                        return tran_dir{false, dir__011};
                    }
                    return tran_dir{true, dir__1_10};
                }
            }
            break;
    }
}
