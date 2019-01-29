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
        _type_lattice_id id;
        int rate_index; // the selected event index in rate array.
    };
}
#endif //MISA_KMC_EVENT_H
