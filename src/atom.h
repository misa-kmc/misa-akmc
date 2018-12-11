//
// Created by genshen on 2018/11/7.
//

#ifndef MISA_KMC_ATOM_H
#define MISA_KMC_ATOM_H

#include <map>
#include <vector>
#include "type_define.h"


using namespace std;

class Atoms {
    friend class AtomList;  // so atom list can set private member:id directly.

public:
    /**
     * \brief initial an atom object on the lattice point specified by (i,j,k).
     */
    explicit Atoms();

    /*!
     * \brief grid point type,e.g. Fe, or Cu, or Vacancy, or Dumbbell.
     */
    int atom_type;

    /*!
     * \brief the transition rate of this lattice
     */
    _type_rate rate;

    //跃迁事件列表
    static vector<int> ilist;
    //跃迁近邻号
    static vector<int> inbr;

    static _type_rate sum_rate;

    /**
     * \brief get unique id of current lattice
     * \return lattice id
     */
    inline _type_atom_id getId() {
        return id;
    }

    //states[xi][yi][zi] = a[getId(xi, yi, zi)].type
    /*!
     * @brief 复合反应对象的选择
     * @param isitl 跃迁事件列表
     * @param minE 截断能量
     * @param list_recb 复合队列
     * @param xi 原子坐标
     * @param yi
     * @param zi
     * @param xv 复合反应原子坐标
     * @param yv
     * @param zv
     */
    void
    recb_check_ecal(bool isitl, double minE, vector<int> list_recb, int xi, int yi, int zi, int xv, int yv, int zv);

    /*!
     * @brief 复合反应函数
     * @param xi 间隙坐标
     * @param yi
     * @param zi
     * @param xv 空位坐标
     * @param yv
     * @param zv
     */
    void rules_recb(int xi, int yi, int zi, int xv, int yv, int zv);

    //first, second, dir1,dir2,dir3,dir4, int StateFeX,
    double FeXcomp(int x, int y, int z);

    double Edumb();

private:
    _type_atom_id id;

};

class Itl {
public:

    Itl() {
    }

    ~Itl();

    int first, second;

    /*direction*/
    int dir1, dir2, dir3, dir4;
};

#endif //MISA_KMC_ATOM_H
