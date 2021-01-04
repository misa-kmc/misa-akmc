//
// Created by genshen on 2018-12-09.
//

#ifndef MISA_KMC_BOX_H
#define MISA_KMC_BOX_H

#include "defect/itl_list.h"
#include "defect/vacancy_list.h"
#include "lattice/lattices_list.h"

class Box;

/**
 * \brief the class to build the box.
 */
class BoxBuilder {
public:
  /**
   * \brief set the simulation box size on this process at x, y and z dimension.
   * \param b_x lattice size (not doubled) at x dimension
   * \param b_y lattice size of simulation box area at y dimension
   * \param b_z lattice size of simulation box area at z dimension
   */
  inline void setBoxSize(const _type_box_size b_x, const _type_box_size b_y, const _type_box_size b_z) {
    this->box_x = b_x;
    this->box_y = b_y;
    this->box_z = b_z;
  }

  /**
   * \brief set the ghost size on this process at x, y and z dimension.
   * \param g_x lattice size (not doubled) of ghost area at x dimension
   * \param g_y lattice size of ghost area at y dimension
   * \param g_z lattice size of ghost area at z dimension
   */
  inline void setGhostSize(const _type_box_size g_x, const _type_box_size g_y, const _type_box_size g_z) {
    this->ghost_x = g_x;
    this->ghost_y = g_y;
    this->ghost_z = g_z;
  }

  /**
   * \brief set lattice size of global simulation box for builder
   * \param gls_x lattice size(not doubled) of global simulation box at x dimension
   * \param gls_y lattice size of global simulation box at y dimension
   * \param gls_z lattice size of global simulation box at z dimension
   */
  inline void setGlobalLatSize(const _type_box_size gls_x, const _type_box_size gls_y, const _type_box_size gls_z) {
    global_box_x = gls_x;
    global_box_y = gls_y;
    global_box_z = gls_z;
  }

  /**
   * \brief set beginning lattice coordinate of global simulation box for builder
   * \param gls_x base lattice coordinate(not doubled) of global simulation box at x dimension
   * \param gls_y base lattice coordinate of global simulation box at y dimension
   * \param gls_z base lattice coordinate of global simulation box at z dimension
   */
  inline void setGlobalBaseLat(const _type_box_size gbs_x, const _type_box_size gbs_y, const _type_box_size gbs_z) {
    global_base_x = gbs_x;
    global_base_y = gbs_y;
    global_base_z = gbs_z;
  }

  /**
   * \brief build the simulation box, and initialize lattice list,
   *        including lattice list, vacancy list and itl list.
   * \return the pointer to new box.
   */
  Box *build();

private:
  // the box size on current  process (x dimension is not doubled, just lattice size)
  _type_box_size box_x = 0, box_y = 0, box_z = 0;
  _type_box_size ghost_x = 0, ghost_y = 0, ghost_z = 0;
  // the global box size
  _type_box_size global_box_x = 0, global_box_y = 0, global_box_z = 0;
  _type_box_size global_base_x = 0, global_base_y = 0, global_base_z = 0;

  double v, T;
};

/*!
 * \brief a box contains the information of all lattice points in the simulation box.
 */
class Box {
  friend Box *BoxBuilder::build();

public:
  /*!
   * \brief list of all lattice points.
   */
  LatticesList *lattice_list;

  /*!
   * \brief list of vacancy indexed by lattice id.
   */
  VacancyList *va_list;

  /*!
   * \brief list of interval lattice (dumbbell) indexed by lattice id.
   * if the type of a lattice is interval/dumbbell,
   * then the extra data such as first/second/orientation will be save in this list indexed by lattice id.
   */
  ItlList *itl_list;

  // destroy lattice list, vacancy list and itl list.
  ~Box();

protected:
  // make it private, so you can not create an Box object using new Box() outside its friend class/function.
  // we can only create a Box object by using BoxBuilder.
  Box();

  /*!
   * \brief in this method, the member \var lattice_list,itl_list,va_list will be created
   * from box size and ghost size parameter (lattice metadata).
   * \note the box size, base coordinate and ghost size in metadata at x dimension is BCC doubled.
   */
  void createBox(const LatListMeta lattice_meta);

private:
  //跃迁事件列表
  static std::vector<int> ilist;
  //跃迁近邻号
  static std::vector<int> inbr;
};

#endif // MISA_KMC_BOX_H
