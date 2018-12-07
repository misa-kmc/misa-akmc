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
public:

    Atoms(){}

    ~Atoms();
    Atoms(_type_atom_cord i, _type_atom_cord j, _type_atom_cord k, int type = 0) {
        x = i;
        y = j;
        z = k;
        atom_type = type;
        rate = 0;
    }

    _type_atom_cord x, y, z;
    int id, atom_type;

    //单原子概率
    double rate;

    //跃迁事件列表
    static vector<int> ilist;
    //跃迁近邻号
    static vector<int> inbr;


    static double sum_rate;

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
