//
// Created by genshen on 2019/9/18.
//

#include <iostream>
#include <args.hpp>
#include <logs/logs.h>
#include <utils/mpi_utils.h>
#include <lattice/lattices_list.h>
#include <utils/mpi_types.h>
#include "creation.h"
#include "building_config.h"
#include "profile_config.h"
#include "config_parsing.h"
#include "pkmc.h"

bool PKMC::beforeCreate(int argc, char **argv) {
    // parser arguments
    // see https://github.com/Taywee/args for using args.
    args::ArgumentParser parser("This is parallel misa-kmc program.", "");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlag<std::string> conf(parser, "conf", "The configure file", {'c', "conf"});
    args::Flag version(parser, "version", "show version number", {'v', "version"});
    try {
        parser.ParseCLI(argc, (const char *const *) argv);
    }
    catch (args::Help) {
        std::cout << parser;
        return false;
    }
    catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return false;
    }
    catch (args::ValidationError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return false;
    }

    if (conf) {
        configFilePath = args::get(conf);
        return true;
    }

    if (version) {
        std::cout << "MISA KMC version " << KMC_VERSION_STRING << std::endl;
        std::cout << "Build time: " << __TIME__ << " " << __DATE__ << "." << std::endl;
        return false;
    }
    // if no args, print usage.
    std::cerr << parser;
    return false;
}

void PKMC::onCreate() {
    ConfigParsing *p_config;
    if (kiwi::mpiUtils::global_process.own_rank == MASTER_PROCESSOR) {
        kiwi::logs::s("env", "mpi env is initialized.\n");
        // initial config Obj, then read and resolve config file.
        p_config = ConfigParsing::newInstance(configFilePath); // todo config file from argv.
        if (p_config->hasError) {
            kiwi::logs::e("config", "{0}\n", p_config->errorMessage);
            this->abort(2);
        }
    } else {
        // just initial a empty config Obj.
        p_config = ConfigParsing::getInstance();
    }
    p_config->sync(); // sync config data to other processors from master processor.

#ifdef KMC_DEBUG_MODE
    // print configure.
    if (kiwi::mpiUtils::global_process.own_rank == MASTER_PROCESSOR) {
        std::cout << p_config->configValues;
    }
#endif
}

bool PKMC::prepare() {
    mpi_types::setInterMPIType();
    sim = new simulation();
    conf::ConfigValues config_v = ConfigParsing::getInstance()->configValues;
    sim->createDomain(config_v.box_size, config_v.lattice_const,
                      config_v.cutoff_radius);

    sim->createLattice();
    // initialize lattices types.
    switch (config_v.create.create_option) {
        case conf::None:
            // todo log error
            return false;
        case conf::Random:
            creation::createRandom(sim->box->lattice_list, config_v.create.types,
                                   config_v.create.types_ratio, config_v.create.va_count,
                                   sim->_p_domain);
            break;
        case conf::Pipe:
            break;
        case conf::Restart:
            break;
    }
    return true;
}

void PKMC::onStart() {
    conf::ConfigValues config_v = ConfigParsing::getInstance()->configValues;
    //  set up ghost.
    sim->prepareForStart();
    // run simulation
    sim->simulate(config_v.physics_time);
}

void PKMC::onFinish() {
    mpi_types::unsetInterMPIType();
}

void PKMC::beforeDestroy() {
    delete sim;
}

void PKMC::onDestroy() {
    kiwiApp::onDestroy();
}
