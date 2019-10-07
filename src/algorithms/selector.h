//
// Created by genshen on 2019/9/21.
//

#ifndef MISA_KMC_SELECTOR_H
#define MISA_KMC_SELECTOR_H

#include "ring.hpp"

#define SECTORS_NUM 8
typedef unsigned int type_sector_id;

typedef ring<type_sector_id, SECTORS_NUM> type_sector;

#endif //MISA_KMC_SELECTOR_H
