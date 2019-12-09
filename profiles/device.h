//
// Created by genshen on 2019/12/6.
//

#ifndef MISA_KMC_DEVICE_H
#define MISA_KMC_DEVICE_H


#include <unistd.h>   // for isatty()
#include <stdio.h>    // for fileno()

#ifdef __cplusplus
extern "C"
{
#endif

int istty() {
    return isatty(fileno(stdout));
}

#ifdef __cplusplus
} // extern "C"
#endif


#endif //MISA_KMC_DEVICE_H
