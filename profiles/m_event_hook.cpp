//
// Created by genshen on 2019/12/20.
//

#include <logs/logs.h>
#include "utils/simulation_domain.h"
#include "m_event_hook.h"
#include "io/lattice_dump.h"


MEventHook::MEventHook(LatticesList *p_lattice_list, counter *p_counter) :
        p_lattice_list(p_lattice_list), p_counter(p_counter) {}

void MEventHook::onStepFinished(unsigned long step) {
    if (step % 200 == 0) {
        kiwi::logs::d("hook", "finish step: {}\n", step);
    }
    if (step % 5000 == 0) {
        LatticeDump dump;
        dump.dump(p_lattice_list, SimulationDomain::kiwi_sim_pro, step);
    }
}

void MEventHook::onAllDone() {

}
