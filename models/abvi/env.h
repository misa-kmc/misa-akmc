//
// Created by genshen on 2019-02-05.
//

#ifndef MISA_KMC_CONFIG_H
#define MISA_KMC_CONFIG_H

/**
 * \brief simulation environment, for example temperature.
 */
namespace env {
  struct energy {
    int h;
  };

  /**
   * \brief configuration of kmc simulation
   */
  struct environment {
    double attempt_freq;    // unit: 1/s
    double temperature;     // temperature, unit: K.
    double ef110;           // <110> dumbbell formation energy in pure Fe, unit ev
    double defect_gen_rate; // rate of defect generation
    struct energy energy;
  };

  extern struct environment global_env;
} // namespace env

#endif // MISA_KMC_CONFIG_H
