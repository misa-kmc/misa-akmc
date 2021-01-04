//
// Created by genshen on 2019-01-24.
//

#include <abvi/defect/itl_list.h>
#include <gtest/gtest.h>

TEST(itl_updateRates_with_beforeRatesUpdate_test, itl_rates_update_test) {
  Lattice lattice;
  lattice.type._type = LatticeTypes::NiMn;

  Itl itl;
  itl.orient = orientation{orientation::s_110};

  Lattice data[8];
  data[0].type._type = LatticeTypes::Fe;
  data[1].type._type = LatticeTypes::Fe;
  data[2].type._type = LatticeTypes::Fe;
  data[3].type._type = LatticeTypes::Fe;
  data[4].type._type = LatticeTypes::Fe;
  data[5].type._type = LatticeTypes::Fe;
  data[6].type._type = LatticeTypes::Fe;
  data[7].type._type = LatticeTypes::Fe;

  Lattice *_1nn[8] = {&data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7]};
  const _type_neighbour_status _1nn_status = 0xFF;
  // callback function to update transition rate.
  auto test_rates_updater = [](Lattice *lat_nei, const LatticeTypes::lat_type ghost_atom,
                               const _type_dir_id _1nn_offset) -> _type_rate { return _1nn_offset + 1; };

  // case 1
  itl.beforeRatesUpdate(_1nn, _1nn_status); // set avail_trans_dir to 0xC3 (orientation: <110>).
  itl.updateRates(lattice, _1nn, _1nn_status, test_rates_updater);

  // transition rate of neighbour 0,0,1,1,6,6,7,7, namely.
  _type_rate expected_rated_1[Itl::RATES_SIZE] = {1.0 / 2, 1.0 / 2, 2.0 / 2, 2.0 / 2,
                                                  7.0 / 2, 7.0 / 2, 8.0 / 2, 8.0 / 2};
  for (int i = 0; i < Itl::RATES_SIZE; i++) {
    EXPECT_EQ(itl.rates[i], expected_rated_1[i]);
  }

  // case 2.
  data[1].type._type = LatticeTypes::FeCu;
  itl.beforeRatesUpdate(_1nn, _1nn_status); // set avail_trans_dir to  (orientation: <110>).
  itl.updateRates(lattice, _1nn, _1nn_status, test_rates_updater);

  // transition rate of neighbour 0,0,1,1,6,6,7,7, namely.
  // default value of rate is zero (set in function beforeRatesUpdate).
  _type_rate expected_rated_2[Itl::RATES_SIZE] = {1.0 / 2, 1.0 / 2, 0.0 / 2, 0.0 / 2,
                                                  7.0 / 2, 7.0 / 2, 8.0 / 2, 8.0 / 2};
  for (int i = 0; i < Itl::RATES_SIZE; i++) {
    EXPECT_EQ(itl.rates[i], expected_rated_2[i]);
  }
  data[1].type._type = LatticeTypes::Fe;
}

TEST(itl_updateRates_test, itl_rates_update_test) {
  Lattice lattice;
  lattice.type._type = LatticeTypes::NiMn;

  Itl itl;
  itl.orient = orientation{orientation::s_110};

  Lattice *_1nn[8];
  // callback function to update transition rate.
  auto test_rates_updater = [](Lattice *lat_nei, // lat_nei is not used.
                               const LatticeTypes::lat_type ghost_atom,
                               const _type_dir_id _1nn_offset) -> _type_rate { return _1nn_offset; };

  // case 1
  itl.avail_trans_dir = 0xC3;
  itl.updateRates(lattice, _1nn, 0xFF, test_rates_updater);

  // transition rate of neighbour 0,0,1,1,6,6,7,7, namely.
  _type_rate expected_rates_1[Itl::RATES_SIZE] = {0.0 / 2, 0.0 / 2, 1.0 / 2, 1.0 / 2,
                                                  6.0 / 2, 6.0 / 2, 7.0 / 2, 7.0 / 2};
  for (int i = 0; i < Itl::RATES_SIZE; i++) {
    EXPECT_EQ(itl.rates[i], expected_rates_1[i]);
  }

  // case 2.
  // clear rates
  for (int i = 0; i < Itl::RATES_SIZE; i++) {
    itl.rates[i] = -1;
  }

  itl.avail_trans_dir = 0xC1;
  itl.updateRates(lattice, _1nn, 0xFF, test_rates_updater);

  _type_rate expected_rated_2[Itl::RATES_SIZE] = {0.0 / 2, 0.0 / 2, -1.0,    -1.0, // -1 not get changed.
                                                  6.0 / 2, 6.0 / 2, 7.0 / 2, 7.0 / 2};
  for (int i = 0; i < Itl::RATES_SIZE; i++) {
    EXPECT_EQ(itl.rates[i], expected_rated_2[i]);
  }

  // case 3.
  // clear rates
  for (int i = 0; i < Itl::RATES_SIZE; i++) {
    itl.rates[i] = -1;
  }

  itl.avail_trans_dir = 0x00;
  itl.updateRates(lattice, _1nn, 0xFF, test_rates_updater);

  _type_rate expected_rated_3[Itl::RATES_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1};
  for (int i = 0; i < Itl::RATES_SIZE; i++) {
    EXPECT_EQ(itl.rates[i], expected_rated_3[i]);
  }
}
