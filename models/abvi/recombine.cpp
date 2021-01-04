//
// Created by genshen on 2019-01-30.
//

#include "recombine.h"
#include "env.h"
#include "rate/bonds/bonds_counter.h"
#include "rate/bonds/energy_dumbbell.h"
#include <algorithm>
#include <cassert>
#include <cmath>

void rec::Rec::recombine(LatticesList *lat_list, VacancyList *vac_list, ItlList *itl_list) {
  lat_list->getLat(lat_vac).type = tp_vac;
  lat_list->getLat(lat_inter).type = tp_inter;
  // remove the item from itl list and vacancy list.
  vac_list->mp.erase(lat_vac);
  itl_list->mp.erase(lat_inter);
}

rec::Rec rec::RecList::pickMinimum() {
  assert(!rec_list.empty());
  std::sort(rec_list.begin(), rec_list.end(),
            [](const Rec &a, const Rec &b) -> bool { return a.delta_energy < b.delta_energy; });
  std::vector<Rec> candidate_rec;
  for (Rec &rec : rec_list) {
    if (abs(rec.delta_energy - rec_list[0].delta_energy) < 1e-6) {
      candidate_rec.emplace_back(rec);
    }
  }
  // todo use strong rand
  return candidate_rec[std::rand() % candidate_rec.size()];
}

void rec::RecList::create(LatticesList *lat_list, ItlList *itl_list, const _type_lattice_id id) {
  // initializing of neighbour lattices
  Lattice &center_lat = lat_list->getLat(id);
  const bool is_vac = center_lat.type.isVacancy();
#ifdef KMC_DEBUG_MODE
  // it must be vacancy or interstitial
  if (!center_lat.type.isVacancy()) {
    assert(center_lat.type.isDumbbell());
  }
#endif
  Lattice *_1nn_list[LatticesList::MAX_1NN] = {nullptr};
  Lattice *_2nn_list[LatticesList::MAX_2NN] = {nullptr};
  lat_list->get1nn(id, _1nn_list);
  lat_list->get2nn(id, _2nn_list);
  _type_neighbour_status _1nn_status = lat_list->get1nnStatus(id);
  _type_neighbour_status _2nn_status = lat_list->get2nnStatus(id);

  // for all 1nn and 2nn lattices
  if (is_vac) {
    // center_lat is vacancy.
    for (unsigned int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
      if (((_1nn_status >> b) & 0x01) && _1nn_list[b]->type.isDumbbell()) {
        // center_lat is vacancy and _1nn_list[b] is interstitial.
        stageCombination(lat_list, itl_list, &center_lat, _1nn_list[b]);
      }
    }
    for (unsigned int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
      if (((_2nn_status >> b) & 0x01) && _2nn_list[b]->type.isDumbbell()) {
        // center_lat is vacancy and _2nn_list[b] is interstitial.
        stageCombination(lat_list, itl_list, &center_lat, _2nn_list[b]);
      }
    }
  } else {
    // center_lat is interstitial.
    for (unsigned int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
      if (((_1nn_status >> b) & 0x01) && _1nn_list[b]->type.isVacancy()) {
        // center_lat is interstitial and _1nn_list[b] is vacancy.
        stageCombination(lat_list, itl_list, _1nn_list[b], &center_lat);
      }
    }
    for (unsigned int b = 0; b < LatticesList::MAX_NEI_BITS; b++) {
      if (((_2nn_status >> b) & 0x01) && _2nn_list[b]->type.isVacancy()) {
        // center_lat is interstitial and _2nn_list[b] is vacancy.
        stageCombination(lat_list, itl_list, _2nn_list[b], &center_lat);
      }
    }
  }
}

void rec::RecList::stageCombination(LatticesList *lat_list, ItlList *itl_list, Lattice *lat_vac, Lattice *lat_inter) {
  const LatticeTypes type_vac = lat_vac->type;
  const LatticeTypes type_inter = lat_inter->type;

#ifdef EAM_POT_ENABLED
  return; // todo eam
#else
  // if higher atom is the same as lower atom, this flag will be true.
  const bool same_high_low = (type_inter.getHighEnd() == type_inter.getHighEnd());

  // calculate system energy before recombination.
  double e_before_h = 0, e_before_l = 0;
  {
    // bonds energy of vacancy lattice contributed by its 1nn/2nn neighbour lattice.
    bonds::_type_pair_ia e_vac = bonds::BondsCounter::count(lat_list, lat_vac->getId(), lat_vac->type);
    // bonds energy of interstitial lattice (higher atom) contributed by its 1nn/2nn neighbour lattice.
    bonds::_type_pair_ia e_inter_h =
        bonds::BondsCounter::count(lat_list, lat_inter->getId(), LatticeTypes{type_inter.getHighEnd()});
    bonds::_type_pair_ia e_inter_l = e_inter_h;
    if (!same_high_low) {
      e_inter_l = bonds::BondsCounter::count(lat_list, lat_inter->getId(), LatticeTypes{type_inter.getLowEnd()});
    }

    // the count of dumbbells does not change, so we does not count this term.
    double e_dumbbell = bond::Edumb(*lat_list, *itl_list);
    e_before_h = e_vac + e_inter_h + e_dumbbell;
    e_before_l = e_vac + e_inter_l + e_dumbbell;
  }

  {
    // exchange atoms (V <-> XY => X <-> Y; higher atom gone away, lower atom left)
    lat_vac->type = LatticeTypes{type_inter.getHighEnd()};
    lat_inter->type = LatticeTypes{type_inter.getLowEnd()};
    bonds::_type_pair_ia e_vac = bonds::BondsCounter::count(lat_list, lat_vac->getId(), lat_vac->type);
    bonds::_type_pair_ia e_inter = bonds::BondsCounter::count(lat_list, lat_inter->getId(), lat_inter->type);
    double e_dumbbell = bond::Edumb(*lat_list, *itl_list);
    // -1*Ef110 means: delta(dumbbell count) = -1.
    double delta_e_h = (-1 * env::global_env.ef110) + (e_vac + e_inter + e_dumbbell) - e_before_h;
    rec_list.emplace_back(lat_vac->getId(), lat_inter->getId(), lat_vac->type, lat_inter->type, delta_e_h);
  }

  // exchange atoms (V <-> XY => Y <-> X; lower atom gone away, higher atom left)
  if (!same_high_low) {
    lat_vac->type = LatticeTypes{type_inter.getLowEnd()};
    lat_inter->type = LatticeTypes{type_inter.getHighEnd()};
    bonds::_type_pair_ia e_vac = bonds::BondsCounter::count(lat_list, lat_vac->getId(), lat_vac->type);
    bonds::_type_pair_ia e_inter = bonds::BondsCounter::count(lat_list, lat_inter->getId(), lat_inter->type);
    double e_dumbbell = bond::Edumb(*lat_list, *itl_list);
    // -1* Ef110 means: delta(dumbbell count) = -1.
    double delta_e_l = (-1 * env::global_env.ef110) + (e_vac + e_inter + e_dumbbell) - e_before_l;
    rec_list.emplace_back(lat_vac->getId(), lat_inter->getId(), lat_vac->type, lat_inter->type, delta_e_l);
  }

  // reset lattice type
  lat_vac->type = type_vac;
  lat_inter->type = type_inter;
#endif
}
