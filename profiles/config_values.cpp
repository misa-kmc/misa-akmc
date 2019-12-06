//
// Created by genshen on 2019/9/18.
//

#include <iostream>
#include <cassert>
#include "config_values.h"
#include "lattice_types_string.h"

void conf::ConfigValues::packData(kiwi::Bundle &bundle) {
    // box
    bundle.put(comm::DIMENSION_SIZE, box_size);
    bundle.put(lattice_const);
    bundle.put(cutoff_radius);
    // simulation
    bundle.put(temperature);
    bundle.put(physics_time);
    bundle.put(steps_limit);
    bundle.put(is_def_gen);
    bundle.put(dpa_ps);
    bundle.put(attempt_freq);
    // create
    bundle.put(create.create_option);
    bundle.put(create.va_count);
    unsigned long size_types = create.types.size();
    unsigned long size_types_r = create.types_ratio.size();
    bundle.put(size_types);
    bundle.put(size_types, create.types.data());
    bundle.put(size_types_r);
    bundle.put(size_types_r, create.types_ratio.data());
    bundle.put(create.pipe_input_box);
    bundle.put(create.restart_file);
    // log
    bundle.put(output.log_interval);
    bundle.put(output.dump_interval);
    bundle.put(output.dump_file_path);
}

void conf::ConfigValues::unpackData(kiwi::Bundle &bundle) {
    int cursor = 0;
    bundle.get(cursor, comm::DIMENSION_SIZE, box_size);
    bundle.get(cursor, lattice_const);
    bundle.get(cursor, cutoff_radius);
    // simulation
    bundle.get(cursor, temperature);
    bundle.get(cursor, physics_time);
    bundle.get(cursor, steps_limit);
    bundle.get(cursor, is_def_gen);
    bundle.get(cursor, dpa_ps);
    bundle.get(cursor, attempt_freq);
    // create
    bundle.get(cursor, create.create_option);
    bundle.get(cursor, create.va_count);
    unsigned long size_types, size_types_r;
    bundle.get(cursor, size_types);
    create.types.resize(size_types);
    bundle.get(cursor, size_types, create.types.data());
    bundle.get(cursor, size_types_r);
    create.types_ratio.resize(size_types_r);
    bundle.get(cursor, size_types_r, create.types_ratio.data());
    bundle.get(cursor, create.pipe_input_box);
    bundle.get(cursor, create.restart_file);
    // log
    bundle.get(cursor, output.log_interval);
    bundle.get(cursor, output.dump_interval);
    bundle.get(cursor, output.dump_file_path);
}

std::ostream &conf::operator<<(std::ostream &os, const conf::ConfigValues &cv) {
    os << "===========config of simulation=============" << std::endl;
    os << "size: " << cv.box_size[0] << " " << cv.box_size[1] << " " << cv.box_size[2] << std::endl;
    os << "lattice const: " << cv.lattice_const << std::endl;
    os << "cutoff radius: " << cv.cutoff_radius << std::endl;
    os << "simulation: T,\t time,\t steps,\t is gen,\t dpa,\t attempt_freq\n" "\t"
       << cv.temperature << "\t" << cv.physics_time << "\t" << cv.steps_limit << "\t"
       << (cv.is_def_gen ? "true" : "false") << "\t" << cv.dpa_ps << "\t" << cv.attempt_freq << std::endl;

    if (cv.create.create_option == conf::CreateOption::Random) {
        os << "create by: random" << std::endl;
        os << "va:" << cv.create.va_count << std::endl;
        assert(cv.create.types.size() == cv.create.types_ratio.size());
        for (unsigned long i = 0; i < cv.create.types.size(); i++) {
            os << lat::LatTypesString(cv.create.types[i]) << ": " << cv.create.types_ratio[i] << "%  ";
        }
        os << std::endl;
    }

    if (cv.create.create_option == conf::CreateOption::Pipe) {
        os << "create by: pipe" << std::endl;
        os << "pipe input box:" << cv.create.pipe_input_box << std::endl;
    }

    if (cv.create.create_option == conf::CreateOption::Restart) {
        os << "create by: restart" << std::endl;
        os << "restart file:" << cv.create.restart_file << std::endl;
    }
    os << "output: log.interval: " << cv.output.log_interval
       << ", log.dump_interval: " << cv.output.dump_interval
       << ", log.dump_file_path: " << cv.output.dump_file_path << std::endl;
    os << "============================================" << std::endl << std::endl;
    return os;
}
