//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_ORIENTATION_H
#define MISA_KMC_ORIENTATION_H


struct tran_orient;

typedef unsigned char _type_dirs_status; // todo compatible with _type_neighbour_status
typedef unsigned char _type_dir_id;
typedef unsigned char _type_dirs_size;

struct orientation {
    typedef enum {
        dir__110, // <110>
        dir__1_10, // <1-10>
        dir__011, // <011>
        dir__01_1, // <01-1>
        dir__101, // <101>
        dir__10_1, // <10-1>
    } _type_dire;

    _type_dire _ori;

    /**
     * \brief get available transition directions of this orientation.
     * \return
     */
    _type_dirs_status availTransDirections() const;

    /**
     * \brief get new orientation after transition.
     * \param src_orient the source orientation.
     * \param is_first_atom whether the transition atom is first atom.
     * \param is_lower whether the transition orientation is lower position.
     * \param rotate rotate orientation.
     * \return new orientation after transition.
     */
    static tran_orient trans(_type_dire src_orient, bool is_first_atom, bool is_lower, bool rotate);
};

struct tran_orient {
    /**
     * \brief if the atom type does not follow the orientation,
     * this value will be true, otherwise will be false.
     *
     * For example, the atom type is FeCu,
     * if the first atom is Fe, the second atom is Cu, value reversed will be true;
     * if the first atom is Cu, the second atom is Fe, value reversed will be false.
     */
    bool reversed;
    orientation orient;
};

#endif //MISA_KMC_ORIENTATION_H
