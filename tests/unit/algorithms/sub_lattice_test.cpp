//
// Created by genshen on 2019/10/13.
//


#include <array>
#include <gtest/gtest.h>
#include <utils/mpi_utils.h>
#include <algorithms/sl/sublattice.h>
#include <algorithms/lattice_region_packer.h>

class TestPks : public LatticeRegionPacker<int> {
public:
    const unsigned long sendLength(const std::vector<comm::Region<int>> send_regions,
                                   const int dimension, const int direction) override {
        return 1;
    }

    void onSend(int buffer[], const std::vector<comm::Region<int>> send_regions,
                const unsigned long send_len, const int dimension, const int direction) override {
        buffer[0] = 0x100 + dimension;
    }

    void onReceive(int buffer[], const std::vector<comm::Region<int>> recv_regions,
                   const unsigned long receive_len, const int dimension, const int direction) override {
        data[dimension] = buffer[0];
    }

    static MPI_Datatype getMPI_DataType() {
        return MPI_INT;
    }

private:
    std::array<int, comm::DIMENSION_SIZE> data = {0, 0, 0};
};


class TestPackerInstance {
public:
    // make its return type matches template type.
    TestPks newSimCommPacker() {
        return TestPks{};
    }

    // make its return type matches template type.
    TestPks newGhostCommPacker() {
        return TestPks{};
    }
};

class TestSLModel : public ModelAdapter {
    double calcRates(const comm::Region<comm::_type_lattice_size> region) override {
        return 1.0;
    }

    void selectRate() override {}

    void perform() override {}
};

TEST(sublattice_template_compile_test, sublattice_test) {
    const int grid_size[3] = {2, 2, 2};
    const int64_t space[3] = {50 * grid_size[0], 60 * grid_size[1], 71 * grid_size[2]};
    const double lattice_const = 0.86;
    const double cutoff_radius_factor = 1.1421;

    // init domain
    MPI_Comm new_comm;
    comm::mpi_process pro = comm::mpi_process{
            kiwi::mpiUtils::local_process.own_rank,
            kiwi::mpiUtils::local_process.all_ranks,
            kiwi::mpiUtils::local_process.comm,
    };
    comm::ColoredDomain *p_domain = comm::ColoredDomain::Builder()
            .setComm(pro, &new_comm)
            .setPhaseSpace(space)
            .setCutoffRadius(cutoff_radius_factor)
            .setLatticeConst(lattice_const)
            .build();

    TestSLModel model;
    SubLattice sl(p_domain, &model, 1.0, 1.0);

    TestPackerInstance pk_ins;
    sl.startTimeLoop<TestPks, TestPks, TestPks, TestPackerInstance>(pk_ins);
}
