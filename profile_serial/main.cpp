//
// Created by genshen on 2018/11/7.
//

#include <iostream>
#include <cmath>

#include <abvi/env.h>
#include <abvi/box.h>
#include <utils/random/random.h>
#include <abvi/kmc.h>
#include <counter.h>
#include <utils/random/rng_type.h>

#include "m_event_listener.h"

void placeDumbbells(LatticesList *lattice_list, ItlList *itl_list, counter &m_counter,
                    const LatticeTypes::lat_type types[],
                    unsigned long count[], const int len);

void logCounter(counter &m_counter) {
    std::cout << "| type\t| count" << std::endl
              << "| V \t|" << m_counter.get(LatticeTypes::V) << std::endl
              << "| Fe\t|" << m_counter.get(LatticeTypes::Fe) << std::endl
              << "| Cu\t|" << m_counter.get(LatticeTypes::Cu) << std::endl
              << "| Ni\t|" << m_counter.get(LatticeTypes::Ni) << std::endl
              << "| Mn\t|" << m_counter.get(LatticeTypes::Mn) << std::endl
              << "| FeFe\t|" << m_counter.get(LatticeTypes::FeFe) << std::endl
              << "| FeCu\t|" << m_counter.get(LatticeTypes::FeCu) << std::endl
              << "| FeNi\t|" << m_counter.get(LatticeTypes::FeNi) << std::endl
              << "| FeMn\t|" << m_counter.get(LatticeTypes::FeMn) << std::endl
              << "| CuCu\t|" << m_counter.get(LatticeTypes::CuCu) << std::endl
              << "| CuNi\t|" << m_counter.get(LatticeTypes::CuNi) << std::endl
              << "| CuMn\t|" << m_counter.get(LatticeTypes::CuMn) << std::endl
              << "| NiNi\t|" << m_counter.get(LatticeTypes::NiNi) << std::endl
              << "| NiMn\t|" << m_counter.get(LatticeTypes::NiMn) << std::endl
              << "| MnMn\t|" << m_counter.get(LatticeTypes::MnMn) << std::endl << std::endl;
}

int main() {
    r::initSeed(0x146221); // initialize random number seed // todo generate seed here

    // set configuration
    env::global_env = env::environment{
            6E12, // attempt_freq 6*10^12 // todo too big
            773,  // temperature, K
            3.53, // ef110
            500,  // defect_gen_rate: 10^2 to 10^3
            env::energy{
                    // h
            }
    };

    //  build simulation box
    const _type_box_size box_x = 50;
    const _type_box_size box_y = 50;
    const _type_box_size box_z = 50;
    BoxBuilder builder{};
    builder.setBoxSize(box_x, box_y, box_z);
    Box *sim_box = builder.build();

    // generate lattice type
    counter m_counter;
    sim_box->lattice_list->forAllLattices([&](const _type_lattice_coord x,
                                              const _type_lattice_coord y,
                                              const _type_lattice_coord z,
                                              Lattice &lattice) {
        // set lattice types randomly.
        const LatticeTypes::lat_type src_types[] = {LatticeTypes::Fe, LatticeTypes::Cu,
                                                    LatticeTypes::Ni, LatticeTypes::Mn};
        const unsigned int ratio[] = {100, 0, 0, 0}; // Fe based lattice
        const unsigned int ratio_total = 100;
        std::uniform_int_distribution<> types_dis(1, ratio_total);
        r::type_rng types_rng(0x100);
        lattice.type._type = LatticeTypes::randomAtomsType(src_types, ratio, 4, types_dis(types_rng));
        m_counter.add(lattice.type._type); // add 1 for this type lattice.
        return true;
    });

    // place atoms into lattice.
    const LatticeTypes::lat_type types[] = {LatticeTypes::Cu, LatticeTypes::Ni, LatticeTypes::Mn};
    unsigned long counts[] = {sim_box->lattice_list->getLatCount() * 150 / 10000, 0, 0}; // Cu 1.5%
    placeDumbbells(sim_box->lattice_list, sim_box->itl_list, m_counter, types, counts, 3);

    // set defects type and defect list.
    struct DefectPosition {
        _type_lattice_coord x, y, z;
        LatticeTypes type;
    };
    std::vector<DefectPosition> defects = {
            DefectPosition{50, 27, 23, LatticeTypes{LatticeTypes::V}},
    };
    for (DefectPosition def :defects) {
        Lattice &lat = sim_box->lattice_list->getLat(def.x, def.y, def.z);
        m_counter.subtract(lat.type._type);
        m_counter.add(def.type._type);
        lat.type = def.type;
        if (def.type.isVacancy()) {
            sim_box->va_list->mp.insert(std::make_pair(lat.getId(), Vacancy{}));
        } else if (def.type.isDumbbell()) {
            Itl itl; // todo set orientation in constructor.
            itl.orient = {orientation::random()};
            sim_box->itl_list->mp.insert(std::make_pair(lat.getId(), itl));
        }
        // todo update avail_trans_dir, not in beforeRatesUpdate.
    }

    // log update statistics values
    std::cout << "==== KMC initialized with:" << std::endl;
    logCounter(m_counter);

    MEventListener m_listener(m_counter);

    // start simulation
    ABVIModel kmc{sim_box, env::global_env.attempt_freq, env::global_env.temperature}; // fixme init box pointer
    kmc.setEventListener(&m_listener);

    double current_time = 0;
    const double total_time = 0.1;
    unsigned long step = 0;
    const clock_t app_start_time = clock();
    const comm::Region<comm::_type_lattice_size> box_region{0, 0, 0, box_x, box_y, box_z};
    while (current_time < total_time) {
        const _type_rate total_rates = kmc.calcRates(box_region);
        const event::SelectedEvent event = kmc.select(box_region, r::random() * total_rates, total_rates);
        if (event.event_type == event::DefectGen) {
            std::cout << "Defect generation." << std::endl;
        }
        kmc.perform(event);
        current_time += -log(r::random()) / total_rates;

        step++;
        std::cout << "total rate: " << total_rates << "; time: " << current_time << std::endl;
        if (step % 10 == 0) {
            const double app_time = static_cast<double>(clock() - app_start_time) / CLOCKS_PER_SEC;
            std::cout << "program execution time (s):" << app_time << std::endl;
            std::cout << "==== KMC lattice count at time step " << step << ":" << std::endl;
            logCounter(m_counter);
        }
    }
    return 0;
}

void placeDumbbells(LatticesList *lattice_list, ItlList *itl_list, counter &m_counter,
                    const LatticeTypes::lat_type types[],
                    unsigned long count[], const int len) {
    const _type_lattice_count lattices_count = lattice_list->getLatCount();
    // for each type
    for (int tp = 0; tp < len; tp++) {
        _type_lattice_count unplaced_count = 0;
        // for each in count of corresponding type.
        for (int con = 0; con < count[tp]; con++) {
            const uint32_t id = r::rand32(0, static_cast<const uint32_t>(lattices_count));
            Lattice &lat = lattice_list->getLat(id);
            if (lat.type.isAtom()) {
                m_counter.subtract(lat.type._type);
                lat.setType(lat.type.combineToInter(types[tp]));
                m_counter.add(lat.type._type);
                Itl itl;
                itl.orient = orientation{orientation::random()};
                // todo update avail tran dirs, not in beforeRatesUpdate.
                itl_list->mp.insert(std::make_pair(lat.getId(), itl));
            } else {
                unplaced_count++;
            }
        }
        // set unplaced count
        count[tp] = unplaced_count;
    }
}
