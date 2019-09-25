//
// Created by genshen on 2019/9/18.
//

#include <iostream>
#include <args.hpp>
#include <logs/logs.h>
#include <utils/mpi_utils.h>
#include <lattice/lattices_list.h>
#include <lattice/normal_lattice_list.h>
#include "building_config.h"
#include "profile_config.h"
#include "config_parsing.h"
#include "pkmc.h"
#include "simulation.h"

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

    simulation simulation1;

    ConfigParsing *p_config;
    p_config = ConfigParsing::getInstance();
    simulation1.createDomain(p_config->configValues.box_size, p_config->configValues.lattice_const,
                             p_config->configValues.cutoff_radius);

    auto a = NormalLatticeList(simulation1._p_domain->sub_box_lattice_size[0], simulation1._p_domain->sub_box_lattice_size[1],
                 simulation1._p_domain->sub_box_lattice_size[0], simulation1._p_domain->lattice_size_ghost[0]);

    return true;
}

void PKMC::onStart() {
    kiwiApp::onStart();
}

void PKMC::onFinish() {
    kiwiApp::onFinish();
}

void PKMC::beforeDestroy() {
    kiwiApp::beforeDestroy();
}

void PKMC::onDestroy() {
    kiwiApp::onDestroy();
}
