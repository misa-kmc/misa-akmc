//
// Created by genshen on 2018-12-12.
//

#ifndef MISA_KMC_LATTICE_TYPES_H
#define MISA_KMC_LATTICE_TYPES_H


#include <cstdint>

struct LatticeTypes {
    /**
     * \note rules:
     * 0. atom type must be bigger then zero
     * 1. inter lattice must be two atom lattices, two types are combined by:
     *    (type1 << big_endian_shift) | type2
     * 2. In inter lattice, the smaller atom type will be put at higher bit,
     * the larger atom type will be put at lower bit.
     * // todo make CuFu different to FeCu.
     */
    typedef enum {
        V = 0, Fe = 0x0001, Cu = 0x0002, Ni = 0x0004, Mn = 0x0008,
        FeFe = 0x0101, FeCu = 0x0102, FeNi = 0x0104, FeMn = 0x0108,
        CuCu = 0x0202, CuNi = 0x0204, CuMn = 0x0208,
        NiNi = 0x0404, NiMn = 0x0408,
        MnMn = 0x0808,
    } lat_type;

    lat_type _type;

    // initialize \member _type
    explicit LatticeTypes(lat_type tp) : _type(tp) {}

    // default value of _type is V
    explicit LatticeTypes() : _type(V) {}

    const static uint16_t c = 4;
    const static uint16_t dumbbell_critical_point = 1 << (c - 1); // 0x0008 (max single atom enum)
    const static uint16_t high_endian_shift = 8; // 8 bits of left shift for one atom type in inter lattices.

    inline bool isDumbbell() const {
        return _type > dumbbell_critical_point;
    }

    inline bool isVacancy() const {
        return _type == V;
    }

    inline bool isAtom() const {
        return (_type <= dumbbell_critical_point) && (_type != V);
    }

    /**
     * \brief it returns the difference set C of atoms (with lat_type format)
     * of two lattices type A and B, in which, C = A-B.
     *
     * \warning in currently implementations, we assumes A is dumbbell and B is single atom.
     *
     * For instance, A= FeCu, B=Cu, this function will return lattice type Fe.
     * \return lattice type of C.
     */
    lat_type diff(const LatticeTypes A, const LatticeTypes B) const;

    lat_type diff(const LatticeTypes B) const;

    /**
     * \brief get the high 8 bits of types.
     * this is useful for get first atom and second atom of dumbbell.
     * for example:
     * _type is FeCu, the method will return Fe;
     * _type is Cu, the method will return V;
     * \return the high 8 bits of types
     */
    inline lat_type getHighEnd() const {
        return static_cast<lat_type>(_type >> high_endian_shift);
    }

    /**
      * \brief get the low 8 bits of types.
      * this is useful for get first atom or second atom of dumbbell.
      * for example:
      * _type is FeCu, the method will return Cu;
      * _type is Cu, the method will return Cu;
      * \return the low 8 bits of types
      */
    inline lat_type getLowEnd() const {
//        return static_cast<lat_type>(_type & ((unsigned short) -1 >> high_endian_shift));
        return static_cast<lat_type>(_type & ((1 << high_endian_shift) - 1));
    }

    /**
     * \brief check whether an atom is the higher part of a dumbbell lattice type.
     * \return true for is higher part, false for otherwise.
     */
    inline bool isHighEnd(const lat_type atom) {
        return getHighEnd() == atom;
    }

    /**
     * \brief check whether an atom is the lower part of a dumbbell lattice type.
     * \return true for is lower part, false for otherwise.
     */
    inline bool isLowEnd(const lat_type atom) {
        return getLowEnd() == atom;
    }

    /**
     * \deprecated
     * \brief get the first atom based on lattice type and orientation(\param is_reversed).
     * \param is_reversed whether the type of inter/dumbbell is reversed.
     * \return first atom type
     */
    lat_type getFirst(const bool is_reversed) const {
        if (is_reversed) {
            // first atom will be at higher bits, second atom will be at lower bits.
            return getHighEnd();
        } else {
            // first atom will be at lower bits, second atom will be at higher bits.
            return getLowEnd();
        }
    }

    /**
     * \deprecated
     * \brief get the second atom based on lattice type and orientation(\param is_reversed).
     * \param is_reversed whether the type of inter/dumbbell is reversed.
     * \return second atom type
     */
    LatticeTypes::lat_type getSecond(const bool is_reversed) const {
        if (is_reversed) {
            // first atom will be at lower bits, second atom will be at higher bits.
            return getLowEnd();
        } else {
            // first atom will be at higher bits, second atom will be at lower bits.
            return getHighEnd();
        }
    }

    /**
     * \brief just random atom type,like Fe,Cu,Ni,Mn.
     * \param source_type the atom types to be select, randomly
     * \param ratio the ratio of mixed alloy to except.
     * \param len the length of array \param ratio and array \param source_type
     * \param hit a random number between [1, sum{\param ratio}].
     * \return the created lattice type.
     */
    static lat_type randomAtomsType(const lat_type source_type[], const unsigned int ratio[],
                                    const unsigned int len, const unsigned int hit);

    /**
     * \brief combine current type with another atom type.
     * \param another_atom another atom.
     * \return the combined lattice type.
     */
    inline lat_type combineToInter(lat_type another_atom) const {
        return combineToInter(_type, another_atom);
    }

    /**
     * \brief the combined two atom types into inter type.
     * create an inter form \param atom_a and \param atom_b
     * for example  \param atom_a is Fe and \param atom_b is Cu, this function will return FeCu.
     * \param atom_a type of atom A
     * \param atom_b type of atom B
     * \return the combined inter lattice type.
     */
    static lat_type combineToInter(lat_type atom_a, lat_type atom_b);

};


#endif //MISA_KMC_LATTICE_TYPES_H
