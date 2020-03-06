//
// Created by genshen on 2020/1/26.
//

#ifndef MISA_KMC_XYZ_READER_H
#define MISA_KMC_XYZ_READER_H

#include "../src/lattice/lattices_list.h"
#include <string>

class XYZReader {
public:
  /**
   * \brief read from file spcified by \param file_path and parsing it.
   * save its parsing result too lattice list
   * \param file_path
   */
  explicit XYZReader(std::string file_path);

  inline LatticesList *getLatticeList() { return lat_list; }

private:
  LatticesList *lat_list = nullptr;
};

#endif // MISA_KMC_XYZ_READER_H
