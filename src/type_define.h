//
// Created by genshen on 2018/11/9.
//

#ifndef MISA_KMC_TYPE_DEFINE_H
#define MISA_KMC_TYPE_DEFINE_H

#include "building_config.h"

/**
 * the type of lattice position in cartesian coordinate system
 */
typedef unsigned long _type_box_size;
typedef unsigned long _type_lattice_size;
typedef long _type_lattice_offset;
typedef long _type_lattice_coord;
typedef unsigned long _type_lattice_id;
typedef unsigned long _type_lattice_count;

/*!
 * \brief the type of transition rate
 */
typedef double _type_rate;

/*!
 * the number of all the lattices
 */
#define MAX_X 100
#define MAX_Y 100
#define MAX_Z 100
#define MAX_ITL 100

#endif //MISA_KMC_TYPE_DEFINE_H
