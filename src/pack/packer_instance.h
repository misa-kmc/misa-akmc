//
// Created by genshen on 2019/10/16.
//

#ifndef MISA_KMC_PACKER_INSTANCE_H
#define MISA_KMC_PACKER_INSTANCE_H

#include "ghost_sync_packer.h"
#include "lattice/lattices_list.h"
#include "sim_sync_packer.h"

class PackerInstance {
public:
  explicit PackerInstance(LatticesList *lattice_list) : lattice_list(lattice_list) {}

  SimSyncPacker newSimCommPacker() { return SimSyncPacker{lattice_list}; }

  GhostSyncPacker newGhostCommPacker() { return GhostSyncPacker{lattice_list}; }

private:
  // reference or pointers to create packer
  LatticesList *lattice_list;
};

#endif // MISA_KMC_PACKER_INSTANCE_H
