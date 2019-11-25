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
     * \param bx box size/lattice count in x direction without ghost area (which is NOT doubled at x dimension).
     * \param by box size/lattice count in y direction without ghost area.
     * \param bz box size/lattice count in z direction without ghost area.
     * \param gx ghost size/lattice in x direction (which is NOT doubled at x dimension).
     * \param gy ghost size/lattice in y direction
     * \param gz ghost size/lattice in z direction
     */
    LatListMeta(const _type_box_size bx, const _type_box_size by, const _type_box_size bz,
                const _type_box_size gx, const _type_box_size gy, const _type_box_size gz);

    /**
     * \brief the lattice size (with ghost area) of lattice lists array in each dimension
     * \note the size_x is two times then real lattice size.
     */
    const _type_lattice_coord size_x, size_y, size_z;

    /**
     * \brief the size of lattice lists array in each dimension.
     * \note the size_x is two times then real box size (without ghost area) due to BCC structure.
     * size_y and size_y is the same as the real lattice size (with ghost area).
     */
    const _type_lattice_coord box_x, box_y, box_z;

    // the max lattice id in box.
    const _type_lattice_id _max_id;

    // global id = local id + local_base_id
    const _type_lattice_id local_base_id = 0;
};

#endif //MISA_KMC_LATTICE_LIST_META_H
