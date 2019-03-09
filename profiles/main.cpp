//
// Created by genshen on 2018/11/7.
//

#include <iostream>
#include <cmath>

#include <env.h>
#include <box.h>
#include <utils/random/random.h>
#include <kmc.h>

void placeDumbbells(LatticesList *lattice_list, ItlList *itl_list,
                    const LatticeTypes::lat_type types[],
                    unsigned long count[], const int len);

int main() {
    r::initSeed(); // initialize random number seed // todo generate seed here

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
    sim_box->lattice_list->forAllLattices([](const _type_lattice_coord x,
                                             const _type_lattice_coord y,
                                             const _type_lattice_coord z,
                                             Lattice &lattice) {
        // set lattice types randomly.
        const LatticeTypes::lat_type src_types[] = {LatticeTypes::Fe, LatticeTypes::Cu,
                                                    LatticeTypes::Ni, LatticeTypes::Mn};
        const unsigned int ratio[] = {100, 0, 0, 0}; // Fe bassed lattice
        lattice.type._type = LatticeTypes::randomAtomsType(src_types, ratio, 4);
        return true;
    });

    // place atoms into lattice.
    const LatticeTypes::lat_type types[] = {LatticeTypes::Cu, LatticeTypes::Ni, LatticeTypes::Mn};
    unsigned long counts[] = {sim_box->lattice_list->getLatCount() * 150 / 10000, 0, 0}; // Cu 1.5%
    placeDumbbells(sim_box->lattice_list, sim_box->itl_list, types, counts, 3);

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
    // todo update statistics values

    // start simulation
    double current_time = 0;
    const double total_time = 100;
    kmc kmc{sim_box}; // fixme init box pointer
    while (current_time < total_time) {
        const _type_rate total_rates = kmc.updateRates(env::global_env.attempt_freq, env::global_env.temperature);
        const event::SelectedEvent event = kmc.select(r::random() * total_rates, total_rates);
        if (event.event_type == event::DefectGen) {
            std::cout << "Defect generation." << std::endl;
        }
        kmc.execute(event);
        current_time += -log(r::random()) / total_rates;
        std::cout << "rate: " << total_rates << "; time: " << current_time << std::endl;
    }
    return 0;
}

void placeDumbbells(LatticesList *lattice_list, ItlList *itl_list,
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
                lat.setType(lat.type.combineToInter(types[tp]));
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
