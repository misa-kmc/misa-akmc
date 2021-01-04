//
// Created by genshen on 2019/11/25.
//

#ifndef MISA_KMC_LATTICE_LIST_META_H
#define MISA_KMC_LATTICE_LIST_META_H

#include <type_define.h>

/**
 * \brief meta data of lattice list in current process,
 * which is a part of domain
 */
struct LatListMeta {
  /**
   * \brief initialize the metadata using box size and ghost size
   * \param box_x box size/lattice count in x dimension without ghost area
   * (which is NOT doubled at x dimension). \param box_y box size/lattice
   * count in y dimension without ghost area. \param box_z box size/lattice
   * count in z dimension without ghost area. \param ghost_x ghost
   * size/lattice in x dimension (which is NOT doubled at x dimension). \param
   * ghost_y ghost size/lattice in y dimension \param ghost_z ghost
   * size/lattice in z dimension \param gbx,gby,gbz lattice size of global
   * simulation box at x,y,z dimension (is NOT doubled at x dimension) \param
   * g_base_x,g_base_y,g_base_z
   */
  LatListMeta(const _type_box_size box_x, const _type_box_size box_y, const _type_box_size box_z,
              const _type_box_size ghost_x, const _type_box_size ghost_y, const _type_box_size ghost_z,
              const _type_box_size gbx, const _type_box_size gby, const _type_box_size gbz,
              const _type_box_size g_base_x, const _type_box_size g_base_y, const _type_box_size g_base_z);

  /**
   * \brief initialize the metadata using box size and ghost size, but without
   * global lattice size and global base coordinate. The global lattice size
   * will be set to box size, and global base coordinate will be set to 0.
   * (treating the global box as a single sub-box, sharing only by current
   * process.) \param box_x,box_y,box_z box size/lattice count in x,y,z
   * dimension without ghost area \param ghost_x,ghost_y,ghost_z ghost
   * size/lattice in x,y,z dimension \note \param box_x and \param ghost_x is
   * NOT bcc doubled.
   */
  LatListMeta(const _type_box_size box_x, const _type_box_size box_y, const _type_box_size box_z,
              const _type_box_size ghost_x, const _type_box_size ghost_y, const _type_box_size ghost_z);

  /**
   * \brief the lattice size (with ghost area) of lattice lists array in each
   * dimension \note the size_x is two times then real lattice size.
   */
  const _type_lattice_coord size_x, size_y, size_z;

  /**
   * \brief the lattice size of simulation box in each dimension on current
   * process. \note the box_x is two times then real box size (without ghost
   * area) due to BCC structure. box_y and box_z is the same as the real
   * lattice size (without ghost area).
   */
  const _type_lattice_coord box_x, box_y, box_z;

  /**
   * \brief the lattice size of global simulation box in each dimension.
   */
  const _type_lattice_coord g_box_x, g_box_y, g_box_z;

  /**
   * \brief the baseline coordinate of current process at each dimension for
   * setting global lattice id.
   */
  const _type_lattice_coord g_base_x, g_base_y, g_base_z;

  /**
   * \brief the ghost lattice size of lattice lists array in each dimension
   * \note the ghost_x is two times then real lattice size.
   */
  const _type_lattice_coord ghost_x, ghost_y, ghost_z;

  // the max local lattice id in box.
  const _type_lattice_id _max_id;

  // global id = local id + local_base_id
  const _type_lattice_id local_base_id = 0;

public:
  /*!
   * \brief calculate global lattice id by lattice coordinate
   * \param x,y,z the index of lattice in lattice list 3d array starting from
   * global box boundary \note the lattice specified by \param x,y,z must be
   * located at simulation area of current process, which means only the
   * lattices in simulation area have global id, ghost lattices do not have
   * global id. Global id usually be used between processes communication.
   * \return The global id of the corresponding lattice in simulation area.
   */
  inline _type_lattice_id getGId(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) {
    // todo return from Lattice object.
    return (x - ghost_x + g_base_x) + g_box_x * ((y - ghost_y + g_base_y) + (z - ghost_z + g_base_z) * g_box_y);
  }

  /**
   * \brief calculate local lattice id by lattice coordinate
   * \param x,y,z the index of lattice in lattice list 3d array starting from
   * local ghost boundary \return local id of the corresponding lattice in
   * simulation area and ghost area.
   */
  inline _type_lattice_id getLId(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) const {
    return x + size_x * (y + z * size_y);
  }

  /**
   * \brief convert coordinate by local id.
   */
  inline void getCoordByLId(_type_lattice_id lid, _type_lattice_coord *x, _type_lattice_coord *y,
                            _type_lattice_coord *z) const {
    *x = lid % size_x;
    lid = lid / size_x;
    *y = lid % size_y;
    *z = lid / size_y;
  }

  /**
   * \brief return true if it is a ghost lattice
   * \return return true if it is a ghost lattice, false for otherwise.
   */
  inline bool isGhostLat(_type_lattice_id lid) const {
    _type_lattice_coord x = 0, y = 0, z = 0;
    getCoordByLId(lid, &x, &y, &z);
    x -= ghost_x;
    y -= ghost_y;
    z -= ghost_z;
    return x < 0 || y < 0 || z < 0 || x >= box_x || y >= box_y || z >= box_z;
  }

  /**
   * \brief calculate a lattice id by a base lattice id and a 1nn offset.
   * \param lat_id base local lattice id
   * \param _1nn_offset 1nn offset
   * \return the lattice id based on lattice id(specified by \param lat_id)
   * with a offset specified by \param _1nn_offset
   */
  _type_lattice_id getIdBy1nnOffset(const _type_lattice_id lat_id, const _type_dir_id _1nn_offset) const;
};

#endif // MISA_KMC_LATTICE_LIST_META_H
