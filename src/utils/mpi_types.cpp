//
// Created by genshen on 2019/10/26.
//

#include <cassert>
#include "lattice/lattice.h"
#include "mpi_types.h"

MPI_Datatype mpi_types::_mpi_type_lattice_data;

void mpi_types::setInterMPIType() {
    setMPI_DataTypeLattice(&_mpi_type_lattice_data);
}

void mpi_types::unsetInterMPIType() {
    MPI_Type_free(&_mpi_type_lattice_data);
}

void mpi_types::setMPI_DataTypeLattice(MPI_Datatype *mpi_type_lat) {
    const int entries = 2;
    // Lattice::type is int type and Lattice::id is unsigned long type.
    MPI_Datatype array_of_types[] = {MPI_INT, MPI_UNSIGNED_LONG};
    // 1 int type and 1 unsigned long type.
    int array_of_block_len[] = {1, 1};

    Lattice lat_dummy;

#if MPI_VERSION >= 2 && MPI_SUBVERSION >= 0
    MPI_Aint addr_base, addr_type, addr_id;
    MPI_Get_address(&lat_dummy, &addr_base);
    MPI_Get_address(&lat_dummy.type, &addr_type);
    MPI_Get_address(&lat_dummy.id, &addr_id);

    assert(addr_type < addr_id);
    assert(addr_base <= addr_type);

    MPI_Aint array_of_dis[] = {addr_type - addr_base, addr_id - addr_base};

    MPI_Type_create_struct(entries, array_of_block_len, array_of_dis, array_of_types, mpi_type_lat);
    MPI_Type_commit(mpi_type_lat);
#else
    static_assert(false, "MPI version not matching (MPI 2 or above is needed)");
#endif
}
