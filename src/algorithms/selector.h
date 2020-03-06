//
// Created by genshen on 2019/9/21.
//

#ifndef MISA_KMC_SELECTOR_H
#define MISA_KMC_SELECTOR_H

#include "ring.hpp"

#define SECTORS_NUM 8
typedef unsigned int type_sector_id;

typedef struct {
  type_sector_id id;     // id of this sector
  double evolution_time; // evolution time of this sector.
} type_sector;

typedef ring<type_sector, SECTORS_NUM> type_sectors_ring;

#endif // MISA_KMC_SELECTOR_H
