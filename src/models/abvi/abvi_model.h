//
// Created by genshen on 2019/10/14.
//

#ifndef MISA_KMC_ABVI_MODEL_H
#define MISA_KMC_ABVI_MODEL_H


#include "models/model_adapter.h"

class ABVIModel : public ModelAdapter {
public:
    double calcRates(const comm::Region<comm::_type_lattice_size> region) override;

    /**
     * \brief select a event from rates list
     */
    void selectRate() override;

    /**
     * \brief perform the kmv event.
     */
    void perform() override;
};


#endif //MISA_KMC_ABVI_MODEL_H
