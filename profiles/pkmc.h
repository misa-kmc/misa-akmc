//
// Created by genshen on 2019/9/18.
//

#ifndef MISA_KMC_PKMC_H
#define MISA_KMC_PKMC_H

#include "simulation.h"
#include <kiwi_app.h>
#include <string>

class PKMC : public kiwi::kiwiApp {

public:
  /**
   * Parse command line argv, and print necessary help information (e.g. run:
   * app --help).
   * @param argc argc from function main().
   * @param argv argv from function main().
   * @return false for interrupting the running of the program after parsing
   * argv.
   */
  bool beforeCreate(int argc, char *argv[]) override;

  /**
   * mpi has been initialed, then parse configure file on master processor and
   * synchronize it to other processor. initial architecture environments(e.g.
   * sunway) if necessary.
   */
  void onCreate() override;

  /**
   * \brief create boxes and lattices for latter simulation.
   */
  bool prepare() override;

  /**
   * run simulation.
   */
  void onStart() override;

  void onFinish() override;

  void beforeDestroy() override;

  void onDestroy() override;

private:
  std::string configFilePath = "config.yaml"; // configure file path default value.
  simulation *sim;
};

#endif // MISA_KMC_PKMC_H
