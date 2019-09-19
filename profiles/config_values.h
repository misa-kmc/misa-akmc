//
// Created by genshen on 2019/9/18.
//

#ifndef MISA_KMC_CONFIG_VALUES_H
#define MISA_KMC_CONFIG_VALUES_H


#include <utils/bundle.h>

struct ConfigValues {
    int data;

    void packData(kiwi::Bundle &bundle);

    void unpackData(kiwi::Bundle &bundle);
};


#endif //MISA_KMC_CONFIG_VALUES_H
