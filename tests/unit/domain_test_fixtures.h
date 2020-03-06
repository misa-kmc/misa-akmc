//
// Created by genshen on 2019/12/30.
//

#ifndef MISA_KMC_DOMAIN_TEST_FIXTURES_H
#define MISA_KMC_DOMAIN_TEST_FIXTURES_H

#include <comm/domain/colored_domain.h>
#include <comm/types_define.h>
#include <gtest/gtest.h>
#include <lattice/period_lattice_list.h>
#include <utils/mpi_utils.h>
#include <utils/simulation_domain.h>

/**
 * \brief test fixture for common domain and lattice list;
 *  lattice list can be different in each test
 * \tparam L type of lattice list.
 */
template <class L> class DomainSteUp : public ::testing::Test {
protected:
  void SetUp() override {
    // set domain
    MPI_Comm new_comm;
    comm::mpi_process pro = comm::mpi_process{
        kiwi::mpiUtils::local_process.own_rank,
        kiwi::mpiUtils::local_process.all_ranks,
        kiwi::mpiUtils::local_process.comm,
    };
    p_domain = comm::ColoredDomain::Builder()
                   .setComm(pro, &new_comm)
                   .setPhaseSpace(phase_space_int64)
                   .setCutoffRadius(cutoff_radius)
                   .setLatticeConst(lattice_const)
                   .build();
    kiwi::mpiUtils::onGlobalCommChanged(new_comm);
    SimulationDomain::setSimDomain(kiwi::mpiUtils::global_process);

    // set lattice list
    lattice_list = new L{LatListMeta{
        static_cast<_type_box_size>(p_domain->sub_box_lattice_size[0]),
        static_cast<_type_box_size>(p_domain->sub_box_lattice_size[1]),
        static_cast<_type_box_size>(p_domain->sub_box_lattice_size[2]),
        static_cast<_type_box_size>(p_domain->lattice_size_ghost[0]),
        static_cast<_type_box_size>(p_domain->lattice_size_ghost[1]),
        static_cast<_type_box_size>(p_domain->lattice_size_ghost[2]),
        p_domain->phase_space[0],
        p_domain->phase_space[1],
        p_domain->phase_space[2],
        static_cast<_type_box_size>(p_domain->sub_box_lattice_region.x_low),
        static_cast<_type_box_size>(p_domain->sub_box_lattice_region.y_low),
        static_cast<_type_box_size>(p_domain->sub_box_lattice_region.z_low),
    }};
  }

  void TearDown() override {
    delete lattice_list;
    delete p_domain;
  }

  // set domain
  const int64_t phase_space_int64[comm::DIMENSION_SIZE] = {40, 50, 60};
  const double lattice_const = 2.85532;
  const double cutoff_radius = 5.6 / lattice_const;

  comm::ColoredDomain *p_domain = nullptr;
  L *lattice_list = nullptr;
};

#endif // MISA_KMC_DOMAIN_TEST_FIXTURES_H
