//
// Created by genshen on 2019/9/21.
//

#ifndef MISA_KMC_SUBLATTICE_H
#define MISA_KMC_SUBLATTICE_H

#include <array>
#include <domain/colored_domain.h>
#include "../parallel_interface.h"
#include "../selector.h"

struct SectorMeta {
    explicit SectorMeta();

    // collection of sectors.
    type_sectors_ring sectors{
            {
                    type_sector{comm::X_LOW | comm::Y_LOW | comm::Z_LOW, 0.0},
                    type_sector{comm::X_LOW | comm::Y_HIGH | comm::Z_LOW, 0.0},
                    type_sector{comm::X_LOW | comm::Y_LOW | comm::Z_HIGH, 0.0},
                    type_sector{comm::X_LOW | comm::Y_HIGH | comm::Z_HIGH, 0.0},
                    type_sector{comm::X_HIGH | comm::Y_LOW | comm::Z_LOW, 0.0},
                    type_sector{comm::X_HIGH | comm::Y_HIGH | comm::Z_LOW, 0.0},
                    type_sector{comm::X_HIGH | comm::Y_LOW | comm::Z_HIGH, 0.0},
                    type_sector{comm::X_HIGH | comm::Y_HIGH | comm::Z_HIGH, 0.0},
            }};

    type_sectors_ring::iterator sector_itl;
};

class SubLattice : public ParallelInterface {
public:
    explicit SubLattice(const comm::ColoredDomain *p_domain, const double T);

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

private:
    // threshold time for communication (the T can be changing in simulation).
    const double T;
    const comm::ColoredDomain *p_domain = nullptr;
};


#endif //MISA_KMC_SUBLATTICE_H
