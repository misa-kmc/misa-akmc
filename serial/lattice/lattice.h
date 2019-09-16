//
// Created by genshen on 2018/11/7.
//

#ifndef MISA_KMC_LATTICE_H
#define MISA_KMC_LATTICE_H

#include <map>
#include <vector>
#include "type_define.h"
#include "lattice_types.h"

class Lattice {
    friend class LatticesList;  // so lattice list can set private member:id directly.

public:
    /**
     * \brief initial an lattice object on the lattice point specified by (i,j,k).
     */
    explicit Lattice();

    /*!
     * \brief type of lattice point,e.g. Fe, or Cu, or Vacancy, or Dumbbell.
     */
    LatticeTypes type;

    /**
     * \brief get unique id of current lattice
     * \return lattice id
     */
    inline _type_lattice_id getId() const{
        return id;
    }

    /**
     * \brief set the lattice type
     */
    inline void setType(LatticeTypes::lat_type tp) {
        type._type = tp;
    }

    //states[xi][yi][zi] = a[getId(xi, yi, zi)].type
    /*!
     * \brief 复合反应对象的选择
     * \param isitl 跃迁事件列表
     * \param minE 截断能量
     * \param list_recb 复合队列
     * \param xi 原子坐标
     * \param yi
     * \param zi
     * \param xv 复合反应原子坐标
     * \param yv
     * \param zv
     */
    void
    recb_check_ecal(bool isitl, double minE, std::vector<int> list_recb, int xi, int yi, int zi, int xv, int yv,
                    int zv);

    /*!
     * \brief 复合反应函数
     * \param xi 间隙坐标
     * \param yi
     * \param zi
     * \param xv 空位坐标
     * \param yv
     * \param zv
     */
    void rules_recb(int xi, int yi, int zi, int xv, int yv, int zv);



private:
    _type_lattice_id id;

};

#endif //MISA_KMC_LATTICE_H
