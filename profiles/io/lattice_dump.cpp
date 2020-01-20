//
// Created by genshen on 2019/12/20.
//

#include <fstream>
#include <comm/types_define.h>
#include <comm/domain/region.hpp>
#include "lattice_dump.h"
#include "utils/macros.h"
#include "../config/lattice_types_string.h"

LatticeDump::LatticeDump() {

}

void LatticeDump::dump(LatticesList *lattice_list, kiwi::mpi_process process, unsigned long step) {
    char outfileName[20];
    sprintf(outfileName, "dump_%d_%ld.xyz", process.own_rank, step);

    std::ofstream outfile;
    outfile.open(outfileName);

    outfile << (lattice_list->meta.box_x * lattice_list->meta.box_y * lattice_list->meta.box_z) << std::endl;
    outfile << std::endl;

    comm::Region<_type_lattice_coord> lbr{lattice_list->meta.ghost_x,
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
                    outfile << lat::LatTypesString(lattice.type._type)
                            << "\t" << x / 2 << "\t" << y << "\t" << z
                            << std::endl;
                } else {
                    outfile << lat::LatTypesString(lattice.type._type)
                            << "\t" << (x / 2 + 0.5) << "\t" << (y + 0.5) << "\t" << (z + 0.5)
                            << std::endl;
                }

            }
        }
    }
    outfile.close();
}
