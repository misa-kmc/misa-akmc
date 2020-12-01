//
// Created by genshen on 2019-01-29.
//

#ifndef MISA_KMC_EVENT_H
#define MISA_KMC_EVENT_H

#include "type_define.h"

namespace event {
  enum EventType {
    VacancyTrans,
    DumbbellTrans,
    DefectGen,
  };

  /**
   * \brief in KMC, if a event is selected, this structure can tag an event.
   */
  struct SelectedEvent {
    event::EventType event_type;
    _type_lattice_id from_id;
    _type_lattice_id to_id;
    _type_dir_id target_tag; // the tag of target lattice of transition(available value from 0 to 7.).
    bool rotate_direction;   // the rotate tag of dumbbell transition (used only for dumbbell transition).
  };
} // namespace event
#endif // MISA_KMC_EVENT_H
