//
// Created by genshen on 2019/9/18.
//

#ifndef MISA_KMC_CONFIG_VALUES_H
#define MISA_KMC_CONFIG_VALUES_H


#include <vector>
#include <utils/bundle.h>
#include <comm/types_define.h>
#include "lattice/lattice_types.h"

namespace conf {
    enum CreateOption {
        None, Random, Pipe, Restart
    };
    struct Create {
        CreateOption create_option = CreateOption::None;;
        // random
        unsigned long va_count = 0;
        std::vector<LatticeTypes::lat_type> types;
        std::vector<unsigned int> types_ratio;
        // pipe
        std::string pipe_input_box = "";
        // restart
        std::string restart_file = "";
    };

    struct Output {
        unsigned long log_interval = 0;
        unsigned long dump_interval = 0;
        std::string dump_file_path = "";
    };

    struct ConfigValues {
        friend std::ostream &operator<<(std::ostream &os, const ConfigValues &cv);

    public:
        Create create;
        Output output;
        // box
        unsigned long box_size[comm::DIMENSION_SIZE];

        double lattice_const = 0.0;

        double cutoff_radius = 0.0;

        // simulation
        double temperature = 0.0;
        double physics_time = 0.0;
        unsigned long steps_limit = 0;
        double attempt_freq = 0.0;
        // isgenr in config file.
        bool is_def_gen = false;
        // dpasm1 in config file
        double dpa_ps = 0.0;

        void packData(kiwi::Bundle &bundle);

        void unpackData(kiwi::Bundle &bundle);
    };
}

#endif //MISA_KMC_CONFIG_VALUES_H
