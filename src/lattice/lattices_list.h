//
// Created by zhaorunchu on 2018-12-06.
// updated by genshen on 2018-12-11.
//

#ifndef MISA_KMC_LATTICES_LIST_H
#define MISA_KMC_LATTICES_LIST_H

#include <map>
#include <vector>
#include <functional>
#include "type_define.h"
#include "lattice.h"

// typedef of iteration of all lattices.
typedef std::function<bool(const _type_lattice_coord x,
                           const _type_lattice_coord y,
                           const _type_lattice_coord z,
                           Lattice &lattice)> func_lattices_callback;

// convert lattice id to x,y,z coordinate, and call callback function using x,y,z.
#define ID_TO_XYZ(id, callback) {       \
id -= local_base_id;                    \
_type_lattice_coord x = id % size_x;    \
id = id / size_x;                       \
_type_lattice_coord y = id % size_y;    \
_type_lattice_coord z = id / size_y;    \
callback;                               \
}

/**
 * \brief type of neighbour status.
 *
 * For 1nn neighbour lattice:
 * The lowest bit indicates the status of neighbour lattice at position of (-c/2,-c/2,-c/2),
 * Then second bit is position of (-c/2,-c/2,c/2), third bit is position of (-c/2,c/2,-c/2),
 * and left bit are position of (-c/2,c/2,c/2), (c/2,-c/2,-c/2),
 * (c/2,-c/2,c/2), (c/2,c/2,-c/2), (c/2,c/2,c/2) from lower bit to higher bit.
 *
 * For 1nn neighbour lattice:
 * The lowest bit indicates the status of neighbour lattice at position of (-c,0,0),
 * Then second bit is position of (0,-c,0), and third bit is position of (0,0,-c),
 * 4th bit is position of (0,0,c), 5th bit is position of (0,c,0),
 * 6th bit is position of (c,0,0).
 *
 * In which c is lattice constant, and phrase like "position of (-c/2,-c/2,-c/2)" means
 * the relative coordinates from source lattice to neighbour lattice.
 *
 */
typedef unsigned char _type_neighbour_status;

class LatticesList {
public:
    static const int MAX_1NN = 8;
    static const int MAX_NEI_BITS = 8;
    static const int MAX_2NN = 6;

    /*!
     * \brief initialize the lattice_lists array(allocate memory) with box size in x,y,z direction.
     * besides, the lattice id will be set in this constructor.
     * \param box_x box size/lattice count in x direction.
     * \param box_y box size/lattice count in y direction.
     * \param box_z box size/lattice count in z direction.
     */
    LatticesList(_type_box_size box_x, _type_box_size box_y, _type_box_size box_z);

    ~LatticesList();

    /**
     * \brief iterate all lattice in this list, each lattice will be passed to callback function.
     * if the callback return false, iteration will break.
     */
    void forAllLattices(const func_lattices_callback callback);

    /**
     * \brief get status of 1nn neighbour lattices.
     *
     * we look 8 1nn neighbour lattices, some neighbour lattices may be out of boundary of simulation box.
     * We call those lattices "1nn out-of-boundary lattices".
     * This method returns the status of "out-of-boundary lattices" in 1nn range,
     *
     * If one neighbour lattice is "1nn out-of-boundary lattice",
     * the corresponding bit in return value will be set to 0, otherwise set to 1.
     * And the bit length of return value is 8.
     *
     * The lowest bit indicates the status(out-of-boundary or not) of neighbour lattice at position of (-c/2,-c/2,-c/2).
     * Then second bit is position of (-c/2,-c/2,c/2), third bit is position of (-c/2,c/2,-c/2),
     * and left bit are position of (-c/2,c/2,c/2), (c/2,-c/2,-c/2),
     * (c/2,-c/2,c/2), (c/2,c/2,-c/2), (c/2,c/2,c/2) from lower bit to higher bit.
     * In which c is lattice constant, and phrase like "position of (-c/2,-c/2,-c/2)" means
     * the relative coordinates from center lattice(specified by \param x,y,z) to neighbour lattice.
     *
     * \param x,y,z the coordinate of the source lattice point.
     * \return bits for status of "1nn out-of-boundary lattices"
     */
    virtual _type_neighbour_status
    get1nnBoundaryStatus(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z);

    /**
     * \brief similar as above one (use x,y,z to specific a lattice), but it receives a lattice id.
     * \param id global lattice id to specific lattice position.
     * \return bits for status of "1nn out-of-boundary lattices"
     */
    _type_neighbour_status get1nnBoundaryStatus(_type_lattice_id id) {
        ID_TO_XYZ(id, return get1nnBoundaryStatus(x, y, z));
    }

    /**
     * \brief similar as get1nnBoundaryStatus, it returns the "out-of-boundary lattices" of 2nn neighbour lattices.
     *
     * In this method, we look 6 2nn neighbour lattices, some neighbour lattices may be out of boundary of simulation box.
     * We call those lattices "2nn out-of-boundary lattices".
     * This method will return bits status for "2nn out-of-boundary lattices".
     *
     * \param x the coordinate of the source lattice point.
     * \return bits for status of "2nn out-of-boundary lattices"
     */
    virtual _type_neighbour_status
    get2nnBoundaryStatus(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z);

    /**
     * \brief similar as above one (use x,y,z to specific a lattice), but it receives a lattice id.
     * \param id global lattice id to specific lattice position.
     * \return bits for status of "2nn out-of-boundary lattices"
     */
    _type_neighbour_status get2nnBoundaryStatus(_type_lattice_id id) {
        ID_TO_XYZ(id, return get2nnBoundaryStatus(x, y, z));
    }

    /**
     * \brief return the bits status of 1nn lattices.
     *
     * when calling get1nn, the array "_1nn_list" will be filled with pointer(s) of available 1nn lattice(s).
     * If 1nn neighbour lattice at some direction is not available,
     * the corresponding bit in return value will be set to 0, otherwise it will be set to 1.
     *
     * \param x,y,z the coordinate of the source lattice point.
     * \return bits for status of 1nn neighbour lattices.
     */
    virtual _type_neighbour_status
    get1nnStatus(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) = 0;

    /**
     * \brief similar as above one (use x,y,z to specific a lattice), but it receives a lattice id.
     * \param id global lattice id to specific lattice position.
     * \return bits for status of 1nn neighbour lattices.
     */
    _type_neighbour_status get1nnStatus(_type_lattice_id id) {
        ID_TO_XYZ(id, return get1nnStatus(x, y, z));
    }

    /**
     * \brief similar as get1nnStatus, it returns the bits status of 2nn neighbour lattices.
     * \param x the coordinate of the source lattice point.
     * \return bits for status of 2nn neighbour lattices
     */
    virtual _type_neighbour_status
    get2nnStatus(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) = 0;

    /**
     * \brief similar as above one (use x,y,z to specific a lattice), but it receives a lattice id.
     * \param id global lattice id to specific lattice position.
     * \return bits for status of 2nn neighbour lattices.
     */
    _type_neighbour_status get2nnStatus(_type_lattice_id id) {
        ID_TO_XYZ(id, return get2nnStatus(x, y, z));
    }

    /*!
     * \brief get all lattices near 1nn
     * 
     * if a 1nn neighbour lattice does not exists (also means the corresponding bit in get1nnStatus return value is 0),
     * the corresponding array element will not get changed.
     * 
     * \param x,y,z the coordinate of the lattice point.
     * \param _1nn_list a array to store all pointers of Lattices in the distance of 1nn.
     * \note note that the coordinate specified by [x,y,z] must be in the lattice box, or "index out of bounds" may happen.
     * \return the lattice pointers count in 1nn list.
     */
    virtual int get1nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z,
                       Lattice *_1nn_list[LatticesList::MAX_1NN]) = 0;

    /**
     * \brief similar as above one (use x,y,z to specific a lattice), but it receives a lattice id.
     * \param id global lattice id to specific lattice position.
     * \return the lattice pointers count in 1nn list.
     */
    int get1nn(_type_lattice_id id, Lattice *_1nn_list[MAX_1NN]) {
        ID_TO_XYZ(id, return get1nn(x, y, z, _1nn_list));
    }

    /*!
     * \brief get all lattice near 2nn
     * \param x,y,z the coordinate of the lattice point.
     * \param _2nn_list a array to store all pointers of Lattices in the distance of 2nn.
     * \note note that the coordinate specified by [x,y,z] must be in the lattice box, or "index out of bounds" may happen.
     * \return the lattice pointers count in 2nn list.
     */
    virtual int
    get2nn(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z, Lattice *_2nn_list[MAX_2NN]) = 0;

    /**
    * \brief similar as above one (use x,y,z to specific a lattice), but it receives a lattice id.
    * \param id global lattice id to specific lattice position.
    * \return the lattice pointers count in 2nn list.
    */
    int get2nn(_type_lattice_id id, Lattice *_2nn_list[MAX_2NN]) {
        ID_TO_XYZ(id, return get2nn(x, y, z, _2nn_list));
    }

    /*!
     * \brief calculate the corresponding id of coordinate
     * \return Id
     */
    inline _type_lattice_id getId(_type_lattice_coord x, _type_lattice_coord y, _type_lattice_coord z) {
        return x + y * size_x + z * size_x * size_y; // todo return from Lattice object.
    }

    /**
     * \return the max lattice id of all lattices.
     */
    inline _type_lattice_id maxId() {
        return _max_id;
    }

    /**
     * \brief get a lattice reference by lattice coordinate
     * \param x,y,z lattice coordinate
     * \return the reference of the matched lattice.
     */
    inline Lattice &getLat(const _type_lattice_coord x, const _type_lattice_coord y,
                           const _type_lattice_coord z) const {
        return _lattices[z][y][x]; // read only
    };

    /**
     * \brief get Lattice object by lattice id
     *
     * \note in the implementations, we does not Guarantee the lattices array boundary.
     * If the lattice specified the \param id is out of box, your program may crash.
     *
     * \param id the given lattice id.
     * \return the reference of the matched lattice.
     */
    Lattice &getLat(_type_lattice_id id);

protected:
    /*!
     * \brief the size of lattice lists array in each dimension.
     * \note the size_x is two times then real box size due to BCC structure.
     * size_y and size_y is the same as the simulation real box size.
     */
    const _type_lattice_coord size_x, size_y, size_z;

    // the max lattice id in box.
    const _type_lattice_id _max_id;

    // global id = local id + local_base_id
    const _type_lattice_id local_base_id = 0;

    /*!
     * \brief the 3d array of all lattices.
     * the first dimension of this array represent x index of lattice in box,
     * then second and third represent y and z index.
     *
     * \note if the box size is [b_x, b_y, b_z], then the array size will be [2*b_x, b_y, b_z].
     * the size of array in first dimension is two times then the box size in x direction due to BCC structure.
     */
    Lattice ***_lattices = nullptr;
};


#endif //MISA_KMC_LATTICES_LIST_H
