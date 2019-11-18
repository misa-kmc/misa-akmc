//
// Created by genshen on 2019-03-07.
//

#ifndef MISA_KMC_COUNTER_H
#define MISA_KMC_COUNTER_H

#include <unordered_map>
#include <cassert>

#include "lattice/lattice_types.h"

struct EnumClassHash {
    template<typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

/**
 * \brief counter for KMC defect.
 * including the count of different types of atoms,dumbbells,vacancy for each steps.
 */
class counter {
public:
    explicit counter() {};

    /**
     * \brief set value for type specified by \param tp.
     * \param tp lattice type
     */
    void set(const LatticeTypes::lat_type tp, const unsigned int count) {
        data[tp] = count;
    }

    /**
     * \brief get value for type specified by \param tp.
     * \param tp lattice type
     */
    const int get(const LatticeTypes::lat_type tp) {
        return data[tp];
    }

    /**
     * \brief add 1 from the value specified by \param tp.
     * \param tp lattice type
     */
    inline void add(const LatticeTypes::lat_type tp) {
        data[tp]++;
    }

    /**
     * \brief subtract 1 from the value specified by \param tp.
     * \param tp
     */
    inline void subtract(const LatticeTypes::lat_type tp) {
#ifdef DEBUG_MODE
        assert(data[tp] > 0);
#endif
        data[tp]--;
    }

    /**
     * \brief get the count of atoms in map with type \param tp.
     * \return atom count whose type is \param tp.
     */
    int getAtomCount(const LatticeTypes::lat_type tp);

protected:
    std::unordered_map<LatticeTypes::lat_type, int, std::hash<int>> data;
};


#endif //MISA_KMC_COUNTER_H
