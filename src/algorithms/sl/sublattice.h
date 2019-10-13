//
// Created by genshen on 2019/9/21.
//

#ifndef MISA_KMC_SUBLATTICE_H
#define MISA_KMC_SUBLATTICE_H

#include <array>
#include <domain/colored_domain.h>
#include "models/model_adapter.h"
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

class SubLattice {
public:
    /**
     * \brief initialize sub-lattice algorithm with a specific kMC model.
     * \param p_domain simulation domain.
     * \param p_model kMC model
     * \param time_limit the max evolution time.
     * \param T threshold time for communication.
     */
    explicit SubLattice(const comm::ColoredDomain *p_domain, ModelAdapter *p_model,
                        const double time_limit, const double T);

    /**
     * \brief start kmc time loop.
     * \tparam PKf packer used to initialize ghost regions.
     * \tparam PKg packer used to sync ghost regions.
     * \tparam PKs packer used to sync simulation regions.
     * \tparam Ins packer instance creator.
     * \param pk_inst to instant the packer for communication.
     */
    template<class PKf, class PKg, class PKs, class Ins>
    void startTimeLoop(Ins pk_inst); // todo make sure RegionPacker is PKf, PKg, PKs's base class at compiling time.

    /**
     * \brief calculate rates in a region
     * \param sector_id sector id
     * \return the total rates
     */
    double calcRates(const type_sector_id sector_id);

private:

    /**
     * \brief communicate ghost area of current process to sync simulation regions of neighbor process.
     * \tparam PKs packer used to sync simulation regions.
     * \tparam Ins packer instance creator.
     * \param pk_inst to instant the packer for communication.
     */
    template<class PKs, class Ins>
    void syncSimRegions(Ins &pk_inst);

    /**
     * \brief communicate ghost area data of next sector in current process.
     * \tparam PKg packer used to sync ghost regions.
     * \tparam Ins packer instance creator.
     * \param pk_inst to instant the packer for communication.
     */
    template<class PKg, class Ins>
    void syncNextSectorGhostRegions(Ins &pk_inst);

    /**
     * \brief some post operations after moved to next sector.
     */
    void nextSector();

private:
    typedef std::array<std::vector<comm::Region<comm::_type_lattice_coord>>, comm::DIMENSION_SIZE> type_comm_lat_regions;

    const comm::ColoredDomain *p_domain = nullptr;

    // threshold time for communication (the T can be changing in simulation).
    const double T;
    const double time_limit = 0.0;

    SectorMeta sec_meta;

    /**
     * \brief kmc model to perform event selecting and execution.
     */
    ModelAdapter *p_model = nullptr;
};

#include "sublattice.inl"

#endif //MISA_KMC_SUBLATTICE_H
