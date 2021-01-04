//
// Created by genshen on 2019/10/13.
//

#ifndef MISA_KMC_COMM_DIRS_H
#define MISA_KMC_COMM_DIRS_H

#include <array>
#include <comm/types_define.h>

// todo use libcomm

/**
 *
 * \brief the send directions (can only be comm::DIR_LOW and comm::DIR_HIGH) of
 * single forward communication in x,y,z dimensions. \param sector_id sector id
 * \return send directions of each dimension.
 */
inline std::array<unsigned int, comm::DIMENSION_SIZE> ssfdCommSendDirs(const type_sector_id sector_id) {
  return {
      (0x7u - sector_id) & 0x1u,
      ((0x7u - sector_id) >> 1u) & 0x1u,
      ((0x7u - sector_id) >> 2u) & 0x1u,
  };
}

/**
 * \brief the receive directions of single forward communication in x,y,z
 * dimensions. \param sector_id sector id \return receive directions of each
 * dimension.
 */
inline std::array<unsigned int, comm::DIMENSION_SIZE> ssfdCommRecvDirs(const type_sector_id sector_id) {
  return {
      sector_id & 0x1u,
      (sector_id >> 1u) & 0x1u,
      (sector_id >> 2u) & 0x1u,
  };
}

#endif // MISA_KMC_COMM_DIRS_H
