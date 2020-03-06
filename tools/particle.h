//
// Created by genshen on 2020/1/27.
//

#ifndef MISA_KMC_PARTICLE_H
#define MISA_KMC_PARTICLE_H

#include "config/lattice_types_string.h"
#include "type_define.h"
#include <sstream>
#include <string>

const double LC = 1.0;

struct particle {
  /**
   * \brief construct particle from string
   */
  particle(std::string line) {
    double xf = 0.0, yf = 0.0, zf = 0.0;
    std::istringstream line_stream(line);
    std::string str_lat_type;
    getline(line_stream, str_lat_type, '\t');
    line_stream >> xf >> yf >> zf;

    type = lat::LatTypes(str_lat_type);
    x = static_cast<_type_lattice_coord>(2 * xf / LC);
    y = static_cast<_type_lattice_coord>(yf / LC);
    z = static_cast<_type_lattice_coord>(zf / LC);
  }

  _type_lattice_coord x, y, z;
  LatticeTypes::lat_type type;
};

#endif // MISA_KMC_PARTICLE_H
