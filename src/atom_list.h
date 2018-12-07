//
// Created by zhaorunchu on 2018-12-06.
//

#ifndef MISA_KMC_ATOM_LIST_H
#define MISA_KMC_ATOM_LIST_H

#include <map>
#include <vector>
#include "type_define.h"
#include "atom.h"

using namespace std;

class AtomList{
public:
    AtomList(){
       init();
    }

    //the whole array of all Atoms.
    Atoms a[MAX_X][MAX_Y][MAX_Z];

    vector<int> Onenn_list, Twonn_list;

    int Onenn_x[8] = {1, 1, 1, 1, -1, -1, -1};

    int Onenn_y[8] = {0, 0, 1, 1, 0, 0, 1, 1};

    int Onenn_z[8] = {0, 1, 0, 1, 0, 1, 0, 1};

    int Twonn_x[6] = {2, -2, 0, 0, 0, 0};

    int Twonn_y[6] = {0, 0, 1, -1, 0, 0};

    int Twonn_z[6] = {0, 0, 0, 0, 1, -1};

    //find Itl id in Atom.
    static map<_type_atom_cord , int> mp;

    /*!
     * @brief get some random Atoms.
     */
    void init();

    /*!
     * @brief get all lattice near 1nn
     * @param Onenn_list a vector to store all atoms in the distance of 1nn
     * @return true.
     */
    bool get1nn(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z, vector<_type_atom_pair> &Onenn_list);

    /*!
     * @brief get all lattice near 1nn
     * @param Onenn_list a vector to store all atoms in the distance of 1nn
     * @return true.
     */
    bool get2nn(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z, vector<_type_atom_pair> &Twonn_list);

    /*!
     * @brief calculate the corresponding number of coordinate
     * @return Id
     */
    _type_atom_cord getId(_type_atom_cord x, _type_atom_cord y, _type_atom_cord z);

};

class ItlList{
public:
    //a map to correspond the id and the Itl sequence number.
    map <int, int > mp;
    /*!
     * @brief to find the sequence number in class Ilt
     *        which include the extra information about Itl type atoms
     * @param id use the AtomList getId
     * @return the sequence number in in Itl
     */
    int getItlnum(_type_atom_cord id);
};
#endif //MISA_KMC_ATOM_LIST_H

