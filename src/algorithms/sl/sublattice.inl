//
// Created by genshen on 2019/10/12.
//

#ifndef MISA_KMC_SUB_LATTICE_INC
#define MISA_KMC_SUB_LATTICE_INC

#include <comm/comm.hpp>
#include <comm/preset/sector_forwarding_region.h>
#include "utils/random/random.h"
#include "comm_dirs.h"

template<class PKf, class PKg, class PKs, class Ins>
void SubLattice::startTimeLoop(Ins pk_inst) {
    const unsigned long time_steps = ceil(time_limit / T);
    for (unsigned long step = 0; step < time_steps; step++) { // time steps loop
        for (int sect = 0; sect < SECTORS_NUM; sect++) { // sector loop
            const double init_overflow_time = sec_meta.sector_itl->evolution_time - step * T;
            double sector_time = init_overflow_time;
            while (sector_time < T) {
                const double total_rates = calcRates((*sec_meta.sector_itl).id);
                p_model->selectRate();
                p_model->perform();
                const double delta_t = -log(r::random() / total_rates);
                sector_time += delta_t;
            }
            // kmc simulation on this sector is finished
            // we store evolution time of current sector when the sector loop finishes.
            sec_meta.sector_itl->evolution_time += sector_time - init_overflow_time;
            // communicate ghost area of current process to sync simulation regions of neighbor process.
            syncSimRegions<PKs>(pk_inst);
            syncNextSectorGhostRegions<PKg>(pk_inst); // communicate ghost area data of next sector in current process.
            ++sec_meta.sector_itl; // update sector id.
            nextSector(); // some post operations after moved to next sector.
        }
    }
}

template<class PKs, class Ins>
void SubLattice::syncSimRegions(Ins &pk_inst) {
    // Note that, in this communication, the sending regions are equals to
    // the receiving regions used in synchronizing ghost at each dimension.
    // Besides, the communication order of dimensions is Z,Y,X;
    // but in synchronizing ghost communication, the order is X,Y,Z.
    // todo the regions can be static.
    const type_sector cur_sector = (*sec_meta.sector_itl);
    // sending region at x dimension.
    const comm::type_region_array send_regions_x = comm::fwCommSectorRecvRegion(
            cur_sector.id, comm::DIM_X, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    // sending region at y dimension.
    const comm::type_region_array send_regions_y = comm::fwCommSectorRecvRegion(
            cur_sector.id, comm::DIM_Y, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    // sending region at z dimension.
    const comm::type_region_array send_regions_z = comm::fwCommSectorRecvRegion(
            cur_sector.id, comm::DIM_Z, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);

    // receiving region at x dimension.
    const comm::type_region_array recv_regions_x = comm::fwCommSectorSendRegion(
            cur_sector.id, comm::DIM_X, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    // receiving region at y dimension.
    const comm::type_region_array recv_regions_y = comm::fwCommSectorSendRegion(
            cur_sector.id, comm::DIM_Y, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    // receiving region at z dimension.
    const comm::type_region_array recv_regions_z = comm::fwCommSectorSendRegion(
            cur_sector.id, comm::DIM_Z, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);

    // pack data
    // call ssfdCommRecvDirs for send dirs and call ssfdCommSendDirs for receive dirs.
    const std::array<unsigned int, comm::DIMENSION_SIZE> send_dirs = ssfdCommRecvDirs(cur_sector.id);
    const std::array<unsigned int, comm::DIMENSION_SIZE> recv_dirs = ssfdCommSendDirs(cur_sector.id);
    const std::array<unsigned int, comm::DIMENSION_SIZE> ranks_send = {
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_X][send_dirs[0]]),
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Y][send_dirs[1]]),
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Z][send_dirs[2]]),
    };
    const std::array<unsigned int, comm::DIMENSION_SIZE> ranks_recv = {
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_X][recv_dirs[0]]),
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Y][recv_dirs[1]]),
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Z][recv_dirs[2]]),
    };
    // do data pack
    int ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &ranks); // todo use comm in domain instead.
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    comm::mpi_process pro = comm::mpi_process{my_rank, ranks, MPI_COMM_WORLD};
    PKs packer = pk_inst.newSimCommPacker();
    comm::singleSideForwardComm(&packer, pro, packer.getMPI_DataType(),
                                type_comm_lat_regions{send_regions_x, send_regions_y, send_regions_z},
                                type_comm_lat_regions{recv_regions_x, recv_regions_y, recv_regions_z},
                                ranks_send, ranks_recv);

}

template<class PKg, class Ins>
void SubLattice::syncNextSectorGhostRegions(Ins &pk_inst) {
    const type_sector next_sector = sec_meta.sector_itl.next();
    // todo the regions can be static.
    // sending region at x dimension.
    const comm::type_region_array send_regions_x = comm::fwCommSectorSendRegion(
            next_sector.id, comm::DIM_X, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    // sending region at y dimension.
    const comm::type_region_array send_regions_y = comm::fwCommSectorSendRegion(
            next_sector.id, comm::DIM_Y, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    // sending region at z dimension.
    const comm::type_region_array send_regions_z = comm::fwCommSectorSendRegion(
            next_sector.id, comm::DIM_Z, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);

    // receiving region at x dimension.
    const comm::type_region_array recv_regions_x = comm::fwCommSectorRecvRegion(
            next_sector.id, comm::DIM_X, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    // receiving region at y dimension.
    const comm::type_region_array recv_regions_y = comm::fwCommSectorRecvRegion(
            next_sector.id, comm::DIM_Y, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    // receiving region at z dimension.
    const comm::type_region_array recv_regions_z = comm::fwCommSectorRecvRegion(
            next_sector.id, comm::DIM_Z, p_domain->lattice_size_ghost,
            p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);

    // pack data
    const std::array<unsigned int, comm::DIMENSION_SIZE> send_dirs = ssfdCommSendDirs(next_sector.id);
    const std::array<unsigned int, comm::DIMENSION_SIZE> recv_dirs = ssfdCommRecvDirs(next_sector.id);
    const std::array<unsigned int, comm::DIMENSION_SIZE> ranks_send = {
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_X][send_dirs[0]]),
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Y][send_dirs[1]]),
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Z][send_dirs[2]]),
    };
    const std::array<unsigned int, comm::DIMENSION_SIZE> ranks_recv = {
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_X][recv_dirs[0]]),
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Y][recv_dirs[1]]),
            static_cast<unsigned int>(p_domain->rank_id_neighbours[comm::DIM_Z][recv_dirs[2]]),
    };

    int ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &ranks);
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    unsigned int my_rank_uint = my_rank;
    // do data pack
    comm::mpi_process pro = comm::mpi_process{my_rank, ranks, MPI_COMM_WORLD};
    PKg packer = pk_inst.newGhostCommPacker();
    comm::singleSideForwardComm(&packer, pro, packer.getMPI_DataType(),
                                type_comm_lat_regions{send_regions_x, send_regions_y, send_regions_z},
                                type_comm_lat_regions{recv_regions_x, recv_regions_y, recv_regions_z},
                                ranks_send, ranks_recv);
}


#endif //MISA_KMC_SUB_LATTICE_INC
