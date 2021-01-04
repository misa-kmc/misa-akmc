//
// Created by genshen on 2019-01-29.
//

#include "energy_dumbbell.h"

double bond::Edumb(LatticesList &lattice_list, ItlList &itl_list) {
  // used in each inter loop.
  Lattice *_1nn_list[LatticesList::MAX_1NN]; // todo reset to null in each for statement
  Lattice *_2nn_list[LatticesList::MAX_2NN];

  double edumb = 0;
  auto _1nn_2nn_contribute = [](Lattice **_1nns, Lattice **_2nns, _type_neighbour_status _1nn_status,
                                _type_neighbour_status _2nn_status) -> double {
    double contr = 0;
    for (int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
      if (((_1nn_status >> b) & 0x01) && _1nns[b]->type.isDumbbell()) {
        // the 1nn neighbour exists and it is also dumbbell
        contr += 0.70;
      }
      if (((_2nn_status >> b) & 0x01) && _2nns[b]->type.isDumbbell()) {
        // the 2nn neighbour exists and it is also dumbbell
        contr += 0.48;
      }
    }
    return contr;
  };

  for (auto const &itl_pair : itl_list.mp) {
    // get lattice id, inter instance, and lattice type
    const _type_lattice_id &id = itl_pair.first;
    const Itl &inter_instance = itl_pair.second;
    const LatticeTypes lat_type = lattice_list.getLat(id).type;

    // get 1nn/2nn lists as well as status of this lattice
    const _type_neighbour_status _1nn_status = lattice_list.get1nnStatus(id);
    const _type_neighbour_status _2nn_status = lattice_list.get2nnStatus(id);
    lattice_list.get1nn(id, _1nn_list);
    lattice_list.get2nn(id, _2nn_list);

    double emix = 0;
    double etens = 0;
    double ecomp = 0;
    double eb = 0;

    switch (lat_type._type) {
    case LatticeTypes::FeCu:
      emix = -0.46;
      ecomp = FeX_comp(id, lat_type, inter_instance, _1nn_list, _2nn_list, _1nn_status, _2nn_status);
      break;
    case LatticeTypes::CuCu:
      emix = -0.36;
      eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
      break;
    case LatticeTypes::FeNi:
      emix = -0.19;
      ecomp = FeX_comp(id, lat_type, inter_instance, _1nn_list, _2nn_list, _1nn_status, _2nn_status);
      break;
    case LatticeTypes::CuNi:
      emix = -0.5;
      eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
      break;
    case LatticeTypes::NiNi:
      emix = -0.3;
      eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
      break;
    case LatticeTypes::FeMn:
      emix = 0.39;
      ecomp = FeX_comp(id, lat_type, inter_instance, _1nn_list, _2nn_list, _1nn_status, _2nn_status);
      break;
    case LatticeTypes::CuMn:
      emix = -0.11;
      eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
      break;
    case LatticeTypes::NiMn:
      emix = 0.08;
      eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
      break;
    case LatticeTypes::MnMn:
      emix = 0.41;
      eb += _1nn_2nn_contribute(_1nn_list, _2nn_list, _1nn_status, _2nn_status);
      break;
    case LatticeTypes::FeFe:
      etens = 0;
      // for 1nn
      for (int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
        if ((_1nn_status >> b) & 0x01) { //  the 1nn neighbour exists
          if (!((inter_instance.orient.availTransDirs() >> b) & 0x01)) {
            // it is not transition direction.
            // the second condition can also be:
            // (inter_instance.avail_trans_dir >> b) & 0x01
            if (_1nn_list[b]->type.isDumbbell()) {
              eb += 0.70;
            } else {
              if (_1nn_list[b]->type._type == LatticeTypes::Cu) {
                etens += 0.13;
              } else if (_1nn_list[b]->type._type == LatticeTypes::Ni) {
                etens += 0.02;
              } else if (_1nn_list[b]->type._type == LatticeTypes::Mn) {
                etens += -0.36;
              }
            }
          } else { // it is transition direction.
            if (_1nn_list[b]->type.isDumbbell()) {
              eb += 0.70;
            } else {
              if (_1nn_list[b]->type._type == LatticeTypes::Cu) {
                etens += -0.13;
              } else if (_1nn_list[b]->type._type == LatticeTypes::Ni) {
                etens += 0.15;
              } else if (_1nn_list[b]->type._type == LatticeTypes::Mn) {
                etens += 0.02;
              }
            }
          }
        }
      }
      // for 2nn
      for (int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
        if (((_2nn_status >> b) & 0x01) && _1nn_list[b]->type.isDumbbell()) {
          eb += 0.48;
        }
      }
      break;
    default:
      emix = 0;
      etens = 0;
      ecomp = 0;
      eb = 0;
      break;
    }
    // eb is added twice, so we divided 2.
    edumb += emix + etens + ecomp + eb / 2;
  }
  return edumb;
}

double bond::FeX_comp(const _type_lattice_id id, const LatticeTypes type, const Itl &itl,
                      Lattice *_1nns[LatticesList::MAX_1NN], Lattice *_2nns[LatticesList::MAX_2NN],
                      const _type_neighbour_status _1nn_status, const _type_neighbour_status _2nn_status) {
  double ecomp_FeX = 0;
  double eb_FeX = 0;
  _type_dirs_status comp_status = itl.avail_trans_dir; // todo make sure avail_trans_dir have been set.
  // in dumbbell type, Fe is always higher bits.
  comp_status &= itl.orient.availTransDirsLow(); // for type FeX, get trans dirs of atom X.
  LatticeTypes compsol = LatticeTypes{type.getLowEnd()};

  // for 1nn neighbour lattice
  for (int b = 0; b < Itl::TRANS_DIRS_BITS_SIZE; b++) {
    if ((comp_status >> b) & 0x01) {
      // the neighbour exists, and it is the transition direction atom X can jump to.
      // and X is not atom Fe(which must be true).
      switch (compsol._type) {
      case LatticeTypes::Cu:
        if (_1nns[b]->type.isDumbbell()) {
          eb_FeX += 0.70;
        } else {
          if (_1nns[b]->type._type == LatticeTypes::Cu) {
            ecomp_FeX += -0.18;
          } else if (_1nns[b]->type._type == LatticeTypes::Ni) {
            ecomp_FeX += -0.35;
          } else if (_1nns[b]->type._type == LatticeTypes::Mn) {
            ecomp_FeX += -0.46;
          }
        }
        break;
      case LatticeTypes::Ni:
        if (_1nns[b]->type.isDumbbell()) {
          eb_FeX += 0.70;
        } else {
          if (_1nns[b]->type._type == LatticeTypes::Cu) {
            ecomp_FeX += -0.21;
          } else if (_1nns[b]->type._type == LatticeTypes::Ni) {
            ecomp_FeX += -0.32;
          } else if (_1nns[b]->type._type == LatticeTypes::Mn) {
            ecomp_FeX += -0.30;
          }
        }
        break;
      case LatticeTypes::Mn:
        if (_1nns[b]->type.isDumbbell()) {
          eb_FeX += 0.70;
        } else {
          if (_1nns[b]->type._type == LatticeTypes::Cu) {
            ecomp_FeX += 0.28;
          } else if (_1nns[b]->type._type == LatticeTypes::Ni) {
            ecomp_FeX += 0.29;
          } else if (_1nns[b]->type._type == LatticeTypes::Mn) {
            ecomp_FeX += 0.28;
          }
        }
        break;
      default:
        break;
      }
    }
  }
  // for 2nn neighbour lattices.
  for (int b = 0; b < Itl::TRANS_DIRS_BITS_SIZE; b++) {
    if (((_2nn_status >> b) & 0x01) && _2nns[b]->type.isDumbbell()) {
      eb_FeX += 0.48;
    }
  }
  return ecomp_FeX + eb_FeX / 2;
  ;
}
