//
// Created by genshen on 2019/10/13.
//

#include "unit_test_config.h"
#include <gtest/gtest.h>

#ifdef KMC_TEST_MPI_ENABLE_FLAG

#include <test/gtest_env.h>

#endif

// see https://github.com/google/googletest/issues/822 for more information.
// main function for adapting mpi environment
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
#ifdef KMC_TEST_MPI_ENABLE_FLAG
  ::testing::AddGlobalTestEnvironment(new kiwi::MPIEnvironment);
#endif // end TEST_MPI_ENABLE_FLAG
  return RUN_ALL_TESTS();
}
