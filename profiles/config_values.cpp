//
// Created by genshen on 2019/9/18.
//

#include "config_values.h"

void ConfigValues::packData(kiwi::Bundle &bundle) {
    bundle.put(data);
    // todo put data into bundle
}

void ConfigValues::unpackData(kiwi::Bundle &bundle) {
    int c = 0;
    bundle.get(c, data);
    // todo get data from bundle
}
