//
// Created by z52527 on 2018-12-06.
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

    vector <int > Onenn_list, Twonn_list;
    /*!
     * @brief get some random Atoms.
     */
    void init();

    /*!
     * @brief get all lattice near 1nn
     * @param Onenn_list a vector to store all atoms in the distance of 1nn
     * @return true.
     */
    bool get1nn(vector<int> &Onenn_list);

    /*!
     * @brief get all lattice near 1nn
     * @param Onenn_list a vector to store all atoms in the distance of 1nn
     * @return true.
     */
    bool get2nn(vector<int> &Twonn_list);

    /*!
     * @brief get type of current atoms
     * @return type
     */
    int getType();


    /*!
     * @brief calculate the corresponding number of coordinate
     * @return Id
     */
    int getId();

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
    int getItlnum(int id);
};
#endif //MISA_KMC_ATOM_LIST_H

