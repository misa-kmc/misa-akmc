//
// Created by genshen on 2019/9/21.
//

#ifndef MISA_KMC_SUBLATTICE_H
#define MISA_KMC_SUBLATTICE_H

#include <array>
#include <domain/colored_domain.h>
#include "../parallel_interface.h"

#define SECTORS_NUM 8
typedef unsigned int type_sector_id;

struct SectorMeta {
    // current sector.
    type_sector_id cur_sector;
    // threshold time for communication (the T can be changing in simulation).
    const double T;
    // time increment in current sector.
    double sector_time;
};

class SubLattice : public ParallelInterface {
public:
    const double evolution_time[SECTORS_NUM]; // evolution time of each sector.
    const type_sector_id sector_ids[SECTORS_NUM]{ // todo use a ring.
            comm::X_LOW | comm::Y_LOW | comm::Z_LOW,
            comm::X_LOW | comm::Y_HIGH | comm::Z_LOW,
            comm::X_LOW | comm::Y_LOW | comm::Z_HIGH,
            comm::X_LOW | comm::Y_HIGH | comm::Z_HIGH,
            comm::X_HIGH | comm::Y_LOW | comm::Z_LOW,
            comm::X_HIGH | comm::Y_HIGH | comm::Z_LOW,
            comm::X_HIGH | comm::Y_LOW | comm::Z_HIGH,
            comm::X_HIGH | comm::Y_HIGH | comm::Z_HIGH,
    };

    /**
     * \brief start kmc time loop.
     */
    void startTimeLoop() override;

    /**
     * \brief calculate rates in a region
     * \return the total rates
     */
    double calcRates(const comm::Region<comm::_type_lattice_size> region) override;

    /**
     * \brief select a event from rates list
     */
    void selectRate() override;

    /**
     * \brief perform the kmv event.
     */
    void perform() override;

private:
    SectorMeta sec_meta; // todo init it.

    /**
     * \brief it returns whether the time threshold T is reached.
     * \return
     */
    bool isTReached();

    /**
     * \brief move to next sector.
     */
    void nextSector();

    /**
     * \brief before performiing computing of next sector.
     */
    void beforeNextSector();
};


#endif //MISA_KMC_SUBLATTICE_H
