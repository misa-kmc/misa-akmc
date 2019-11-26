//
// Created by genshen on 2019/10/10.
//

#ifndef MISA_KMC_MODEL_ADAPTER_H
#define MISA_KMC_MODEL_ADAPTER_H

#include <comm/domain/colored_domain.h>

/**
 * \brief this is kmc model adapter
 */
class ModelAdapter {
public:
    /**
     * \brief calculate rates in a region
     * \return the sum of all rates.
     */
    virtual double calcRates(const comm::Region<comm::_type_lattice_size> region) = 0;

    /**
     * \brief select a event from rates list
     */
    virtual void selectRate() = 0;

    virtual unsigned long defectSize() = 0;

    /**
     * \brief perform the kmv event.
     */
    virtual void perform() = 0;
};


#endif //MISA_KMC_MODEL_ADAPTER_H
