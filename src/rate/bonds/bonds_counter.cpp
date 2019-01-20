//
// Created by genshen on 2019-01-20.
//

#include "bonds_counter.h"

/**
 * table 7 in paper {Vincent, E., C. S. Becquart, and C. Domain. \
 * "Solute interaction with point defects in α Fe during thermal ageing: A combined ab initio and atomic kinetic Monte Carlo approach."  \
 * Journal of nuclear materials 351.1-3 (2006): 88-99}.
 */
const std::map<PairBond::bond_type, _type_pair_ia> BondsCounter::_1nn_bonds = {
        {PairBond::FeFe, -0.778},
        {PairBond::VV,   0.315},
        {PairBond::CuCu, -0.581},
        {PairBond::NiNi, -0.793},
        {PairBond::MnMn, -0.438},

        {PairBond::VFe,  -0.161},
        {PairBond::FeCu, -0.609},
        {PairBond::FeNi, -0.821},
        {PairBond::FeMn, -0.648},

        {PairBond::VCu,  -0.103},
        {PairBond::VNi,  -0.234},
        {PairBond::VMn,  -0.151},

        {PairBond::CuNi, -0.692},
        {PairBond::CuMn, -0.519},
        {PairBond::NiMn, -0.831},
};

const std::map<PairBond::bond_type, _type_pair_ia> BondsCounter::_2nn_bonds = {
        {PairBond::FeFe, -0.389},
        {PairBond::VV,   -0.214},
        {PairBond::CuCu, -0.389},
        {PairBond::NiNi, -0.389},
        {PairBond::MnMn, -0.389},

        {PairBond::VFe,  -0.161},
        {PairBond::FeCu, -0.344},
        {PairBond::FeNi, -0.399},
        {PairBond::FeMn, -0.364},

        {PairBond::VCu,  -0.206},
        {PairBond::VNi,  -0.351},
        {PairBond::VMn,  -0.206},

        {PairBond::CuNi, -0.344},
        {PairBond::CuMn, -0.249},
        {PairBond::NiMn, -0.464},
};
