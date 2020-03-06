//
// Created by genshen on 2019/12/6.
//

#ifndef MISA_KMC_DEVICE_H
#define MISA_KMC_DEVICE_H

#include <stdio.h>  // for fileno()
#include <unistd.h> // for isatty()

#ifdef __cplusplus
extern "C" {
#endif

int istty() { return isatty(fileno(stdout)); }

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MISA_KMC_DEVICE_H
