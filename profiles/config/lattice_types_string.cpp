//
// Created by genshen on 2019/9/19.
//

#include "lattice_types_string.h"
#include <stdexcept>

std::string lat::LatTypesString(LatticeTypes::lat_type lat_type) {
  switch (lat_type) {
  case LatticeTypes::V:
    return "V";
  case LatticeTypes::Fe:
    return "Fe";
  case LatticeTypes::Cu:
    return "Cu";
  case LatticeTypes::Ni:
    return "Ni";
  case LatticeTypes::Mn:
    return "Mn";
  case LatticeTypes::FeFe:
    return "FeFe";
  case LatticeTypes::FeCu:
    return "FeCu";
  case LatticeTypes::FeNi:
    return "FeNi";
  case LatticeTypes::FeMn:
    return "FeMn";
  case LatticeTypes::CuCu:
    return "CuCu";
  case LatticeTypes::CuNi:
    return "CuNi";
  case LatticeTypes::CuMn:
    return "CuMn";
  case LatticeTypes::NiNi:
    return "NiNi";
  case LatticeTypes::NiMn:
    return "NiMn";
  case LatticeTypes::MnMn:
    return "MnMn";
  }
}

LatticeTypes::lat_type lat::LatTypes(const std::string &lat_type) {
  if (lat_type == "V") {
    return LatticeTypes::V;
  } else if (lat_type == "Fe") {
    return LatticeTypes::Fe;
  } else if (lat_type == "Cu") {
    return LatticeTypes::Cu;
  } else if (lat_type == "Ni") {
    return LatticeTypes::Ni;
  } else if (lat_type == "Mn") {
    return LatticeTypes::Mn;
  } else if (lat_type == "FeFe") {
    return LatticeTypes::FeFe;
  } else if (lat_type == "FeCu") {
    return LatticeTypes::FeCu;
  } else if (lat_type == "FeNi") {
    return LatticeTypes::FeNi;
  } else if (lat_type == "FeMn") {
    return LatticeTypes::FeMn;
  } else if (lat_type == "CuCu") {
    return LatticeTypes::CuCu;
  } else if (lat_type == "CuNi") {
    return LatticeTypes::CuNi;
  } else if (lat_type == "CuMn") {
    return LatticeTypes::CuMn;
  } else if (lat_type == "NiNi") {
    return LatticeTypes::NiNi;
  } else if (lat_type == "NiMn") {
    return LatticeTypes::NiMn;
  } else if (lat_type == "MnMn") {
    return LatticeTypes::MnMn;
  } else {
    throw std::invalid_argument("wrong lattice type");
  }
}
