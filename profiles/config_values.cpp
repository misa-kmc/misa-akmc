//
// Created by genshen on 2019/9/18.
//

#include <iostream>
#include <cassert>
#include "config_values.h"
#include "lattice_types_string.h"

void ConfigValues::packData(kiwi::Bundle &bundle) {
    bundle.put(data);
    // todo put data into bundle
}

void ConfigValues::unpackData(kiwi::Bundle &bundle) {
    int c = 0;
    bundle.get(c, data);
    // todo get data from bundle
}

std::ostream &operator<<(std::ostream &os, const ConfigValues &cv) {
    os << "===========config of simulation=============" << std::endl;
    os << "size: " << cv.box_size[0] << " " << cv.box_size[1] << " " << cv.box_size[2] << std::endl;
    os << "lattice const: " << cv.lattice_const << std::endl;
    os << "cutoff radius: " << cv.cutoff_radius << std::endl;
    os << "simulation: T,\t time,\t steps,\t is gen,\t dpa\n" "\t"
       << cv.temperature << "\t" << cv.physics_time << "\t" << cv.steps_limit << "\t"
       << (cv.is_def_gen ? "true" : "false") << "\t" << cv.dpa_ps << std::endl;

    if (cv.create.create_option == CreateOption::Random) {
        os << "create by: random" << std::endl;
        os << "va:" << cv.create.va_count << std::endl;
        assert(cv.create.types.size() == cv.create.types_ratio.size());
        for (unsigned long i = 0; i < cv.create.types.size(); i++) {
            os << lat::LatTypesString(cv.create.types[i]) << ": " << cv.create.types_ratio[i] << "%  ";
        }
        os << std::endl;
    }

    if (cv.create.create_option == CreateOption::Pipe) {
        os << "create by: pipe" << std::endl;
        os << "pipe input box:" << cv.create.pipe_input_box << std::endl;
    }

    if (cv.create.create_option == CreateOption::Restart) {
        os << "create by: restart" << std::endl;
        os << "restart file:" << cv.create.restart_file << std::endl;
    }
    os << "output: log.interval: " << cv.output.log_interval
       << ", log.dump_interval: " << cv.output.dump_interval
       << ", log.dump_file_path: " << cv.output.dump_file_path << std::endl;
    os << "============================================" << std::endl << std::endl;
    return os;
}
