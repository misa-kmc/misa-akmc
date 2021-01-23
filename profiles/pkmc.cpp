//
// Created by genshen on 2019/9/18.
//

#include <abvi/kmc.h>
#include <args.hpp>
#include <iostream>
#include <lattice/lattices_list.h>
#include <logs/logs.h>
#include <utils/mpi_types.h>
#include <utils/mpi_utils.h>

#include "building_config.h"
#include "config/config_parsing.h"
#include "config/lattice_types_string.h"
#include "creation.h"
#include "device.h"
#include "m_event_hook.h"
#include "m_event_listener.h"
#include "pkmc.h"
#include "profile_config.h"
#include "seed_relocate.h"

bool PKMC::beforeCreate(int argc, char **argv) {
  // parser arguments
  // see https://github.com/Taywee/args for using args.
  args::ArgumentParser parser("This is parallel misa-kmc program.", "");
  args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
  args::ValueFlag<std::string> conf(parser, "conf", "The configure file", {'c', "conf"});
  args::Flag version(parser, "version", "show version number", {'v', "version"});
  try {
    parser.ParseCLI(argc, (const char *const *)argv);
  } catch (args::Help) {
    std::cout << parser;
    return false;
  } catch (args::ParseError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return false;
  } catch (args::ValidationError e) {
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
  // sync config data to other processors from master processor.
  p_config->sync();

#ifdef KMC_DEBUG_MODE
  // print configure.
  if (kiwi::mpiUtils::global_process.own_rank == MASTER_PROCESSOR) {
    std::cout << p_config->configValues;
  }
#endif

  // prepare logs.
  if (istty() && !(p_config->configValues.output.logs_to_file)) {
    // set colorful log if we output to console and it is a real tty(no io
    // redirection).
    kiwi::logs::setColorFul(true);
  } else if (p_config->configValues.output.logs_to_file) {
    kiwi::logs::setLogFile(p_config->configValues.output.logs_file);
  }
}

bool PKMC::prepare() {
  mpi_types::setInterMPIType();
  sim = new simulation();
  conf::ConfigValues config_v = ConfigParsing::getInstance()->configValues;
  // if the seed is 0, reset the seed using random device.
  r::seedRelocate(&config_v.seeds.create_types, &config_v.seeds.create_vacancy, &config_v.seeds.event_selection,
                  &config_v.seeds.time_inc);
  sim->createDomain(config_v.box_size, config_v.lattice_const, config_v.cutoff_radius);

  sim->createLattice();
  // initialize lattices types.
  switch (config_v.create.create_option) {
  case conf::None:
    // todo log error
    return false;
  case conf::Random:
    creation::createRandom(config_v.seeds.create_types, config_v.seeds.create_vacancy, sim->box->lattice_list,
                           sim->box->va_list, config_v.create.types, config_v.create.types_ratio,
                           config_v.create.va_count, sim->_p_domain);
    break;
  case conf::Pipe:
    creation::createFromPile(config_v.create.pipe_input_box, config_v.seeds.create_types, sim->box->lattice_list,
                             sim->box->va_list, config_v.create.types, config_v.create.types_ratio, sim->_p_domain);
    kiwi::logs::i("create", "placed {} defects from pipe file.\n", sim->box->va_list->mp.size());
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
  // setup model and run simulation
  counter m_counter = counter::newCounter(sim->box->lattice_list); // atoms counter
  m_counter.setLatTypeToStrFunc(&lat::LatTypesString);
  std::cout << m_counter;

  ABVIModel model(sim->box, config_v.attempt_freq, config_v.temperature);
  MEventListener m_listener(m_counter, sim->box->lattice_list->meta);
  model.setEventListener(&m_listener);
  // run simulation
  MEventHook m_event_hook(config_v.output, sim->box->lattice_list, &m_counter);
  sim->simulate(&model, &m_event_hook, config_v.seeds.time_inc, config_v.physics_time);
}

void PKMC::onFinish() { mpi_types::unsetInterMPIType(); }

void PKMC::beforeDestroy() { delete sim; }

void PKMC::onDestroy() { kiwiApp::onDestroy(); }
