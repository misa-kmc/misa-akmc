//
// Created by genshen on 2019/10/16.
//

#ifndef MISA_KMC_PACKER_INSTANCE_H
#define MISA_KMC_PACKER_INSTANCE_H


#include "ghost_sync_packer.h"

class PackerInstance {
public:
    GhostSyncPacker newSimCommPacker() {
        return GhostSyncPacker{};
    }

    GhostSyncPacker newGhostCommPacker() {
        return GhostSyncPacker{};
    }
};


#endif //MISA_KMC_PACKER_INSTANCE_H
