//
// Created by genshen on 2019-03-09.
//

#include "m_event_listener.h"

MEventListener::MEventListener(counter &m_counter, LatListMeta meta) : m_counter(m_counter), meta(meta) {}

void MEventListener::onVacancyTrans(const unsigned long time_step, const comm::_type_lattice_coord source_id,
                                    const comm::_type_lattice_coord target_id, const LatticeTypes trans_atom) {
  // lattice count not change.
}

void MEventListener::onDumbbellTrans(const unsigned long time_step, const comm::_type_lattice_coord source_id,
                                     const comm::_type_lattice_coord target_id, const LatticeTypes old_type_src,
                                     const LatticeTypes old_type_target, const LatticeTypes new_type_src,
                                     const LatticeTypes new_type_target) {
  m_counter.subtract(old_type_src._type);
  m_counter.subtract(old_type_target._type);
  m_counter.add(new_type_src._type);
  m_counter.add(new_type_target._type);
}

void MEventListener::onRecombine(const unsigned long time_step, const rec::Rec rec) {
  m_counter.subtract(LatticeTypes::V);
  m_counter.subtract(LatticeTypes::combineToInter(rec.tp_vac._type, rec.tp_inter._type));
  m_counter.add(rec.tp_vac._type);
  m_counter.add(rec.tp_inter._type);
}

void MEventListener::onDefectGenerate(const unsigned long time_step, LatticeTypes lat_1_type, LatticeTypes lat_2_type,
                                      const LatticeTypes::lat_type combined_type) {
  m_counter.subtract(lat_1_type._type);
  m_counter.subtract(lat_2_type._type);
  m_counter.add(LatticeTypes::V);
  m_counter.add(combined_type);
}
