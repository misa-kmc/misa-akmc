//
// Created by genshen on 2020/1/26.
//

#include "lattice/lattices_list.h"
#include "profile_config.h"
#include <args.hpp>
#include <iostream>

#include "ans.h"
#include "xyz_reader.h"

int main(int argc, char **argv) {
  args::ArgumentParser parser("Analysis tools for misa-akmc output.", "");
  args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
  args::ValueFlag<std::string> conf(parser, "input", "The input file path", {'i', "input"});
  args::Flag version(parser, "version", "show version number", {'v', "version"});
  try {
    parser.ParseCLI(argc, (const char *const *)argv);
  } catch (args::Help) {
    std::cout << parser;
    return 0;
  } catch (args::ParseError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  } catch (args::ValidationError e) {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  }

  if (conf) {
    std::string xyz_file_path = args::get(conf);
    XYZReader reader(xyz_file_path);
    LatticesList *lat_list = reader.getLatticeList();
    if (lat_list == nullptr) {
      std::cerr << "error reading file " << xyz_file_path << std::endl;
    } else {
      // todo ans.
      ans::analysis(lat_list);
    }
    return 0;
  }

  if (version) {
    std::cout << "MISA KMC version " << KMC_VERSION_STRING << std::endl;
    std::cout << "Build time: " << __TIME__ << " " << __DATE__ << "." << std::endl;
    return 0;
  }
  // if no args, print usage.
  std::cerr << parser;

  return 0;
}
