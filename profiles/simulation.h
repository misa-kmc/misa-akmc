//
// Created by z52527 on 2019-09-24.
//

#ifndef MISA_KMC_SIMULATION_H
#define MISA_KMC_SIMULATION_H

#include <mpi.h>
#include <cstring>
#include <io/io_writer.h>

#include <domain/colored_domain.h>

class simulation {
public:

    simulation()= default;

    //virtual ~simulation();

    /**
     * Denote N as the count of all processors.
     * {@memberof domainDecomposition} will divide the simulation box into N parts,
     * we call each part as a sub-box.
     * And each sub-box will bind to a processor.
     * @param phase_space the lattice count in each dimension.
     * @param lattice_const lattice constance
     * @param cutoff_radius cutoff radius factor = cutoff/lattice_const
     */
    void createDomain(const unsigned long phase_space[comm::DIMENSION_SIZE],
                      const double lattice_const, const double cutoff_radius);

    comm::ColoredDomain *_p_domain;


};


#endif //MISA_KMC_SIMULATION_H
