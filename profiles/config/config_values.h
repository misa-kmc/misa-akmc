//
// Created by genshen on 2019/9/18.
//

#ifndef MISA_KMC_CONFIG_VALUES_H
#define MISA_KMC_CONFIG_VALUES_H

#include "lattice/lattice_types.h"
#include <comm/types_define.h>
#include <utils/bundle.h>
#include <vector>

namespace conf {
  enum CreateOption { None, Random, Pipe, Restart };
  struct Create {
    CreateOption create_option = CreateOption::None;
    ;
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
    unsigned long dump_interval = 0;
    std::string dump_file_path = "";
    unsigned long logs_interval = 0;
    std::string logs_file = "";
    bool logs_to_file = false;
  };

  struct RandomSeeds {
    uint32_t create_types;
    uint32_t create_vacancy;
    uint32_t event_selection;
    uint32_t time_inc;
  };

  struct ConfigValues {
    friend std::ostream &operator<<(std::ostream &os, const ConfigValues &cv);

  public:
    Create create;
    Output output;
    RandomSeeds seeds;
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
} // namespace conf

#endif // MISA_KMC_CONFIG_VALUES_H
