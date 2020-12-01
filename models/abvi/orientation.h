//
// Created by genshen on 2019-01-10.
//

#ifndef MISA_KMC_ORIENTATION_H
#define MISA_KMC_ORIENTATION_H

#include "lattice/lattice_types.h"
#include "lattice/lattices_list.h"
#include <utils/random/random.h>

struct orientation {
  typedef enum {
    unknown = 0,
    s_110 = 1,        // <110>
    s_1_10 = 2,       // <1-10>
    s_011 = 3,        // <011>
    s_01_1 = 4,       // <01-1>
    s_101 = 5,        // <101>
    s_10_1 = 6,       // <10-1>
    r_110 = -s_110,   // -<110>
    r_1_10 = -s_1_10, // -<1-10>
    r_011 = -s_011,   // -<011>
    r_01_1 = -s_01_1, // -<01-1>
    r_101 = -s_101,   // -<101>
    r_10_1 = -s_10_1, // -<10-1>
  } _type_dire;

  // the count of orientation, including 'unknown'.
  static const unsigned int orientation_count = 13;
  //  the count of single orientation (the count of orientation whose value > 0).
  static const unsigned int single_orient_count = 6;

  _type_dire _ori;

  /**
   * \brief generate a orientation randomly (not including 'unknown').
   * \return random orientation
   */
  static _type_dire random() {
    uint32_t r = r::rand32(0, 2 * single_orient_count) + 1; // value of r is from 1 to 12
    // if it is odd, return negative orientation, else return positive orientation
    return r % 2 == 0 ? static_cast<_type_dire>(r / 2) : static_cast<_type_dire>(-((r + 1) / 2));
  }

  /**
   * \brief get available transition directions of two atoms in dumbbell under this orientation.
   * If a 1nn neighbour lattice is an available transition directions,
   * the corresponding bit will be set to 1.
   * And if it is not available, the bit will be set to 0.
   * \note that: availTransDirs() is always equals to " availTransDirsHigh | availTransDirsLow()"
   * \return the bits of available transition directions under this orientation.
   */
  _type_dirs_status availTransDirs() const;

  /**
   * \brief get available transition directions of the atom at higher bits in lattice type.
   * For example, the lattice type is XY, this function returns the bits to indicates
   * which neighbour lattices atom X can jump(returns atom X's available transition directions).
   * \return the bits of available transition directions of the atom at higher bits in lattice type
   */
  _type_dirs_status availTransDirsHigh() const;

  /**
   * \brief get available transition directions of the atom at lower bits in lattice type.
   * For example, the lattice type is XY, this function returns the bits to indicates
   * which neighbour lattices atom Y can jump (returns atom Y's available transition directions).
   * \return the bits of available transition directions of the atom at lower bits in lattice type
   */
  _type_dirs_status availTransDirsLow() const;

  /**
   * \brief this method returns the atom type jumping to a neighbour lattice specified by \param _1nn_tag.
   * We assume the dumbbell type is XY, this function will return atom X (if atom X can jump to the neighbour)
   * or atom Y (if atom Y can jump to the neighbour),
   * or XY (if no atom can jump to the neighbour, which may cause a fault error).
   * \param type the type of dumbbell.
   * \param _1nn_tag 1nn neighbour lattice tag (value from 0 to 7).
   * \return jumping atom in 2 dumbbell atoms.
   */
  LatticeTypes tranAtom(const LatticeTypes type, const _type_dir_id _1nn_tag) const;

  /**
   * \deprecated currently
   * \brief get new orientation after transition.
   * \param _1nn_tag 1nn neighbour lattice tag (value from 0 to 7) for transition.
   * \param new_higher whether the jump atom is at higher bits in new dumbbell.
   * \param rotate rotate direction, true for positive, false for negative.
   * \return new orientation after transition.
   */
  orientation trans(const _type_dir_id _1nn_tag, const bool new_higher, const bool rotate) const;

private:
  const static char X = 'X';
  const static char Y = 'Y';
  const static char _ = '_';

  /**
   * \brief find jump atom in JumpAtomHashTable based on \param _1nn_tag and dumbbell orientation.
   * \param _1nn_tag 1nn neighbour lattice tag
   * \return atom tag: 'X'(atom X can jump) or 'Y'(atom Y can jump), or '_' (none can jump).
   */
  inline char findJumpAtom(const _type_dir_id _1nn_tag) const {
    // hashing dumbbell orient to array index.
    return JumpAtomHashTable[_ori + single_orient_count][_1nn_tag];
  }

  inline int tranHash(_type_dire orient) const { return orient < 0 ? -orient - 1 : orient - 1; }

  /**
   * \brief assuming the dumbbell type is XY,
   * we can search this table to determine which atom (X or Y)
   * can jump to the corresponding neighbour lattice.
   */
  const static char JumpAtomHashTable[orientation_count][LatticesList::MAX_1NN];

  /**
   * \brief transition table.
   * we can get new orientation after transition from this table by
   * origin orientation, 1nn lattice tag, jump atom position in new dumbbell and rotate direction
   *
   */
  const static _type_dire TransHashTable[single_orient_count][LatticesList::MAX_1NN][2][2];
};

#endif // MISA_KMC_ORIENTATION_H
