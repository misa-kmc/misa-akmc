//
// Created by genshen on 2019-03-09.
//

#ifndef MISA_KMC_M_EVENT_LISTENER_H
#define MISA_KMC_M_EVENT_LISTENER_H

#include <abvi/plugin/event_listener.h>
#include <counter.h>

/**
 * \see base class for interface/api description.
 */
class MEventListener : public EventListener {
public:
  counter &m_counter;

  MEventListener(counter &m_counter, LatListMeta meta);

  void onVacancyTrans(const unsigned long time_step, const comm::_type_lattice_coord source_id,
                      const comm::_type_lattice_coord target_id, const LatticeTypes trans_atom) override;

  void onDumbbellTrans(const unsigned long time_step, const comm::_type_lattice_coord source_id,
                       const comm::_type_lattice_coord target_id, const LatticeTypes old_type_src,
                       const LatticeTypes old_type_target, const LatticeTypes new_type_src,
                       const LatticeTypes new_type_target) override;

  void onRecombine(const unsigned long time_step, const rec::Rec rec) override;

  void onDefectGenerate(const unsigned long time_step, LatticeTypes lat_1_type, LatticeTypes lat_2_type,
                        const LatticeTypes::lat_type combined_type) override;

private:
  const LatListMeta meta;
};

#endif // MISA_KMC_M_EVENT_LISTENER_H
