//
// Created by genshen on 2025/8/20.
//

#include "../../profiles/real_time.hpp"
// #include "real_time.hpp"

#include <gtest/gtest.h>

TEST(real_time_test, calc_real_time) {
  // parameters: mc_time, temperature, c_x
  const double sim_C_V = 1.0 / (2 * 40 * 40 * 40);
  const double real_dt = kmc::convert_to_real_time(sim_C_V, 3e-5, 663, 0.0120234375);
  EXPECT_FLOAT_EQ(real_dt, 9817.473303);

  const double real_dt2 = kmc::convert_to_real_time(sim_C_V, 3e-5, 663, 0.005703125);
  EXPECT_FLOAT_EQ(real_dt2, 16764.21327);

  const double real_dt3 = kmc::convert_to_real_time(sim_C_V, 3e-5, 663, 0.0026796875);
  EXPECT_FLOAT_EQ(real_dt3, 25342.28066);
}
