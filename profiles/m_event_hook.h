//
// Created by genshen on 2019/12/20.
//

#ifndef MISA_KMC_M_EVENT_HOOK_H
#define MISA_KMC_M_EVENT_HOOK_H


#include <hook/event_hooks.hpp>

class MEventHook : public EventHooks {
public:
    void onStepFinished(unsigned long step) override;

    void onAllDone() override;
};


#endif //MISA_KMC_M_EVENT_HOOK_H
