//
// Created by genshen on 2020/1/26.
//

#include "xyz_reader.h"
#include "lattice/period_lattice_list.h"
#include "particle.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

XYZReader::XYZReader(std::string file_path) {
  std::ifstream infile(file_path);
  if (!infile.good()) {
    throw std::invalid_argument("bad argument of file path");
  }

  unsigned long lat_count = 0;
  infile >> lat_count;
  std::string comment_line;
  std::getline(infile, comment_line);

  std::vector<particle> lats_coords;
  _type_lattice_coord x_min = 0x0fffffff, y_min = 0x0fffffff, z_min = 0x0fffffff;
  _type_lattice_coord x_max = 0, y_max = 0, z_max = 0;

  // read file line by line, and save each coordinate.
  // and record max and min coordinate
  std::string line;
  while (std::getline(infile, line)) {
    if (line.empty()) {
      continue;
    }
    // convert particle coordinate to index in lattice list array
    particle p(line);
    if (p.x < x_min) {
      x_min = p.x;
    }
    if (p.y < y_min) {
      y_min = p.y;
    }
    if (p.z < z_min) {
      z_min = p.z;
    }
    if (p.x > x_max) {
      x_max = p.x;
    }
    if (p.y > y_max) {
      y_max = p.y;
    }
    if (p.z > z_max) {
      z_max = p.z;
    }
    lats_coords.emplace_back(p);
  }
  if (lats_coords.size() != lat_count) {
    throw std::invalid_argument("bad lattice count");
  }

  const _type_lattice_size box_size_x = (x_max - x_min + 1) / 2;
  const _type_lattice_size box_size_y = y_max - y_min + 1;
  const _type_lattice_size box_size_z = z_max - z_min + 1;

  std::cout << "box size: " << box_size_x << "," << box_size_y << "," << box_size_z << std::endl;

  auto lattice_meta = LatListMeta{box_size_x, box_size_y, box_size_z, 2, 2, 2};
  // todo delete
  lat_list = new PeriodLatticeList(lattice_meta);
  for (auto l : lats_coords) {
    lat_list->getLat(l.x, l.y, l.z).setType(l.type);
  }
}
