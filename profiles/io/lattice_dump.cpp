//
// Created by genshen on 2019/12/20.
//

#include "lattice_dump.h"
#include "../config/lattice_types_string.h"
#include "utils/macros.h"
#include <comm/domain/region.hpp>
#include <comm/types_define.h>
#include <fstream>

LatticeDump::LatticeDump() {}

void LatticeDump::dump(const std::string dump_file_path,
                       LatticesList *lattice_list, unsigned long step) {
  std::ofstream outfile;
  outfile.open(dump_file_path);
  if (!outfile.good()) {
    throw std::invalid_argument("invalid file path " + dump_file_path);
  }

  outfile << (lattice_list->meta.box_x * lattice_list->meta.box_y *
              lattice_list->meta.box_z)
          << std::endl;
  outfile << std::endl;

  comm::Region<_type_lattice_coord> lbr{
      lattice_list->meta.ghost_x,
      lattice_list->meta.ghost_y,
      lattice_list->meta.ghost_z,
      (lattice_list->meta.ghost_x + lattice_list->meta.box_x),
      lattice_list->meta.ghost_y + lattice_list->meta.box_y,
      lattice_list->meta.ghost_z + lattice_list->meta.box_z,
  };
  for (comm::_type_lattice_coord z = lbr.z_low; z < lbr.z_high; z++) {
    for (comm::_type_lattice_coord y = lbr.y_low; y < lbr.y_high; y++) {
      for (comm::_type_lattice_coord x = lbr.x_low; x < lbr.x_high; x++) {
        Lattice &lattice = lattice_list->getLat(x, y, z);
        if (x % 2 == 0) {
          outfile << lat::LatTypesString(lattice.type._type) << "\t" << x / 2
                  << "\t" << y << "\t" << z << std::endl;
        } else {
          outfile << lat::LatTypesString(lattice.type._type) << "\t"
                  << (x / 2 + 0.5) << "\t" << (y + 0.5) << "\t" << (z + 0.5)
                  << std::endl;
        }
      }
    }
  }
  outfile.close();
}
