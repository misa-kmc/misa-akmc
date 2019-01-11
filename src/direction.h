//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_DIRECTION_H
#define MISA_KMC_DIRECTION_H


struct tran_dir;

struct dirs {
    typedef enum {
        dir__110, // <110>
        dir__1_10, // <1-10>
        dir__011, // <011>
        dir__01_1, // <01-1>
        dir__101, // <101>
        dir__10_1, // <10-1>
    } _type_dire;

    _type_dire _d;

    /**
     * \brief get new direction after transition.
     * \param src_dir the source direction.
     * \param is_first_atom whether the transition atom is first atom.
     * \param is_lower whether the transition direction is lower position.
     * \param rotate rotate direction.
     * \return new direction after transition.
     */
    static tran_dir trans(_type_dire src_dir, bool is_first_atom, bool is_lower, bool rotate);
};

struct tran_dir {
    /**
     * \brief if the atom type does not follow the direction,
     * this value will be true, otherwise will be false.
     *
     * For example, the atom type is CuFe,
     * if the first atom is Fe, the second atom is Cu, value reversed will be true;
     * if the first atom is Cu, the second atom is Fe, value reversed will be false.
     */
    bool reversed;
    dirs::_type_dire dir;
};

#endif //MISA_KMC_DIRECTION_H
