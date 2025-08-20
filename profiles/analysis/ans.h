//
// Created by genshen on 2020/1/27.
//

#ifndef MISA_KMC_ANS_H
#define MISA_KMC_ANS_H

#include <lattice/lattices_list.h>
#include <mpi.h>

namespace ans {
  // do analysis: return the number of isolated solute atoms.
  struct analysis_result {
    using _type_count = unsigned long;
    const MPI_Datatype mpi_datatype = MPI_UNSIGNED_LONG;

    _type_count isolated_solutes = 0;
    _type_count vacancies = 0;   // number of vacancies in ths simulation box.
    _type_count lattice_num = 0; // number of simulation lattice

    void reduce_to_rank(int rank);
  };

  analysis_result analysis(LatticesList *lats_list);

}; // namespace ans

#endif // MISA_KMC_ANS_H
