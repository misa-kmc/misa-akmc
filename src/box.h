//
// Created by genshen on 2018-12-09.
//

#ifndef MISA_KMC_BOX_H
#define MISA_KMC_BOX_H

#include "lattices_list.h"
#include "itl_list.h"
#include "vacancy_list.h"

class Box;

/**
 * \brief the class to build the box.
 */
class BoxBuilder {
public:
    /*!
     * \brief set the box size in x,y and z dimension.
     * \param size_x
     * \param size_y
     * \param size_z
     */
    inline void setBoxSize(_type_box_size size_x, _type_box_size size_y, _type_box_size size_z) {
        this->size_x = size_x;
        this->size_y = size_y;
        this->size_z = size_z;
    }

    /*!
     * \brief build the simulation box, initialize lattice information.
     * \return the pointer to new box.
     */
    Box *build();

private:
    // the box size
    _type_box_size size_x, size_y, size_z;
};


/*!
 * \brief a box contains the information of all lattice points in the simulation box.
 */
class Box {
    friend Box *BoxBuilder::build();

public:
    // the box size in dimension x,y,z.
    const _type_box_size size_x, size_y, size_z;

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
     * then the extra data such as first/second/direction will be save in this list indexed by lattice id.
     */
    ItlList *itl_list;

protected:

    // make it private, so you can not create an Box object using new Box() outside its friend class/function.
    // we can only create a Box object by using BoxBuilder.
    Box(_type_box_size size_x, _type_box_size size_y, _type_box_size size_z);

    /*!
     * \brief in this method, the member @var lattice_list,itl_list,va_list will be created
     * from box size parameter.
     */
    void createBox();

private:
    //跃迁事件列表
    static std::vector<int> ilist;
    //跃迁近邻号
    static std::vector<int> inbr;

    static _type_rate sum_rate;
};


#endif //MISA_KMC_BOX_H
