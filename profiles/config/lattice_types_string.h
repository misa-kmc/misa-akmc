//
// Created by genshen on 2019/9/19.
//

#ifndef MISA_KMC_LATTICE_TYPES_STRING_H
#define MISA_KMC_LATTICE_TYPES_STRING_H


#include <string>
#include <lattice/lattice_types.h>

namespace lat {
    /**
     * \brief convert lattice type to string.
     * \param lat_type lattice type
     * \return a string of lattice type.
     */
    std::string LatTypesString(LatticeTypes::lat_type lat_type);

    /**
     * \brief convert string to a LatticeTypes type.
     * \param lat_type string of lattice type.
     * \return lattice type.
     */
    LatticeTypes::lat_type LatTypes(const std::string &lat_type);
};


#endif //MISA_KMC_LATTICE_TYPES_STRING_H
