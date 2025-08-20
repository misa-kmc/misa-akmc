//
// Created by genshen on 2025/8/20.
//

#ifndef MISA_AKMC_REAL_TIME_HPP
#define MISA_AKMC_REAL_TIME_HPP

#include <cmath>
#include <cstdio>

/**
 * This file can convert MC simulation time to real physics time.
 * see also: https://doi.org/10.1016/j.jnucmat.2006.02.018
 */
namespace kmc {
  namespace def {
    // formation energy of Vacancy and Fe.
    constexpr double E_for_V_Fe = 1.995; // unit: eV

    // binding energies of Vacancy and first neighbor X.
    constexpr double E_b_V_X_1 = 0.17; // unit: eV

    // binding energies of Vacancy and second neighbor X.
    constexpr double E_b_V_X_2 = 0.19; // unit: eV

    constexpr double Delta_S_div_k = 2.0;
  } // namespace def

  /**
   * @param mc_time: MC time
   * @param T: Temperature. unit: K.
   * @param c_x: concentration of solute X.
   */
  inline double convert_to_real_time(const double C_V_sim, const double mc_time, const double T, const double c_x) {
    constexpr double kT_300 = 0.025852;   // kT under temperature 300K, unit eV.
    const double kT = T / 300.0 * kT_300; // kT under temperature T

    const double C_V_real =
        exp(-(def::E_for_V_Fe) / kT) * exp(def::Delta_S_div_k) *
        (1 - 8 * c_x - 6 * c_x + 8 * c_x * exp(def::E_b_V_X_1 / kT) + 6 * c_x * exp(def::E_b_V_X_2 / kT));

    const double t_real = C_V_sim / C_V_real * mc_time;
    return t_real;
  }
} // namespace kmc

#endif // MISA_AKMC_REAL_TIME_HPP
