//
// Created by genshen on 2019/12/20.
//

#ifndef MISA_KMC_LATTICE_DUMP_H
#define MISA_KMC_LATTICE_DUMP_H


#include <utils/mpi_utils.h>
#include "lattice/lattices_list.h"

class LatticeDump {
public:
    LatticeDump();

    /**
     * \brief dump lattice information at current step
     * \param step  current step
     */
    void dump(const std::string dump_file_path, LatticesList *lattice_list, unsigned long step);
};


#endif //MISA_KMC_LATTICE_DUMP_H
