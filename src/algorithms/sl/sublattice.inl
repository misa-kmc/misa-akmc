//
// Created by genshen on 2019/10/12.
//

#ifndef MISA_KMC_SUB_LATTICE_INC
#define MISA_KMC_SUB_LATTICE_INC

#include <cmath>
#include <comm/comm.hpp>
#include <comm/preset/sector_forwarding_region.h>
#include <logs/logs.h>
#include "utils/simulation_domain.h"
#include "utils/random/random.h"
#include "comm_dirs.h"


template<class PKg, class PKs, class Ins, typename E>
void SubLattice::startTimeLoop(Ins pk_inst, ModelAdapter<E> *p_model, EventHooks *p_event_hooks) {
    const unsigned long time_steps = ceil(time_limit / T);
    for (unsigned long step = 0; step < time_steps; step++) { // time steps loop
        for (int sect = 0; sect < SECTORS_NUM; sect++) { // sector loop
            const double step_threshold_time = static_cast<double>(step + 1) * T - sec_meta.sector_itl->evolution_time;
            double sector_time = 0.0;
            while (sector_time < step_threshold_time) { // note: step_threshold_time may be less then 0.0
                const double total_rates = calcRatesWrapper(p_model, (*sec_meta.sector_itl).id);
                if (total_rates == 0.0 || std::abs(total_rates) < std::numeric_limits<_type_rate>::epsilon()) {
                    // If there is no defect, use synchronous parallel kMC algorithm.
                    // Because there is no kMC event, just increase time.
                    sector_time = step_threshold_time;
                } else {
                    selectPerformWrapper(p_model, total_rates, (*sec_meta.sector_itl).id);
                    const double rand = time_inc_dis(time_inc_rng);
                    const double delta_t = -std::log(rand) / total_rates;
                    sector_time += delta_t;
                }
                // todo: time comparing, nearest principle based on predicting next delta t.
            }
            // kmc simulation on this sector is finished
            // we store evolution time of current sector when the sector loop finishes.
            sec_meta.sector_itl->evolution_time += sector_time;
            // communicate ghost area of current process to sync simulation regions of neighbor process.
            syncSimRegions<PKs>(pk_inst);
            syncNextSectorGhostRegions<PKg>(pk_inst); // communicate ghost area data of next sector in current process.
            p_model->reindex(p_domain->local_sector_region[(*sec_meta.sector_itl).id]); // reindex defects lists
            ++sec_meta.sector_itl; // update sector id.
            nextSector(); // some post operations after moved to next sector.
        }
        p_event_hooks->onStepFinished(step);
    }
    p_event_hooks->onAllDone();
}

template<typename E>
double SubLattice::calcRatesWrapper(ModelAdapter<E> *p_model, const type_sector_id sector_id) {
    return p_model->calcRates(p_domain->local_sector_region[sector_id]);
}

template<typename E>
void SubLattice::selectPerformWrapper(ModelAdapter<E> *p_model, const _type_rate total_rates,
                                      const type_sector_id sector_id) {
    p_model->selectAndPerform(p_domain->local_sector_region[sector_id], total_rates);
}

template<class PKs, class Ins>
void SubLattice::syncSimRegions(Ins &pk_inst) {
    // Note that, in this communication, the sending regions are equals to
    // the receiving regions used in synchronizing ghost at each dimension.
    // Besides, the communication order of dimensions is Z,Y,X;
    // but in synchronizing ghost communication, the order is X,Y,Z.
    // todo the regions can be static.
    const type_sector cur_sector = (*sec_meta.sector_itl);
    const int dims[comm::DIMENSION_SIZE] = {comm::DIM_X, comm::DIM_Y, comm::DIM_Z};
    type_comm_lat_regions send_regions; // send regions in each dimension
    type_comm_lat_regions recv_regions; // receive regions in each dimension.
    for (int d = 0; d < comm::DIMENSION_SIZE; d++) {
        send_regions[d] = comm::fwCommSectorRecvRegion(
                cur_sector.id, dims[d], p_domain->lattice_size_ghost,
                p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
        recv_regions[d] = comm::fwCommSectorSendRegion(
                cur_sector.id, dims[d], p_domain->lattice_size_ghost,
                p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    }

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

    PKs packer = pk_inst.newSimCommPacker();
    // todo communication order from Z to Y, and X.
    comm::singleSideForwardComm(&packer, SimulationDomain::comm_sim_pro,
                                packer.getMPI_DataType(),
                                send_regions, recv_regions,
                                ranks_send, ranks_recv);
}

template<class PKg, class Ins>
void SubLattice::syncNextSectorGhostRegions(Ins &pk_inst) {
    const type_sector next_sector = sec_meta.sector_itl.next();
    const int dims[comm::DIMENSION_SIZE] = {comm::DIM_X, comm::DIM_Y, comm::DIM_Z};
    // todo the regions can be static.
    type_comm_lat_regions send_regions; // send regions in each dimension
    type_comm_lat_regions recv_regions; // receive regions in each dimension.
    for (int d = 0; d < comm::DIMENSION_SIZE; d++) {
        send_regions[d] = comm::fwCommSectorSendRegion(
                next_sector.id, dims[d], p_domain->lattice_size_ghost,
                p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);;
        recv_regions[d] = comm::fwCommSectorRecvRegion(
                next_sector.id, dims[d], p_domain->lattice_size_ghost,
                p_domain->local_split_coord, p_domain->local_sub_box_lattice_region);
    }

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

    // do data pack
    PKg packer = pk_inst.newGhostCommPacker();
    comm::singleSideForwardComm(&packer, SimulationDomain::comm_sim_pro,
                                packer.getMPI_DataType(),
                                send_regions, recv_regions,
                                ranks_send, ranks_recv);
}


#endif //MISA_KMC_SUB_LATTICE_INC
