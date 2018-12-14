//
// Created by genshen on 2018-12-09.
//

#ifndef MISA_KMC_MACROS_H
#define MISA_KMC_MACROS_H

#define ASSIGN_ARRAY_8(arr, i0, i1, i2, i3, i4, i5, i6, i7)  { \
   arr[0] = i0; \
   arr[1] = i1; \
   arr[2] = i2; \
   arr[3] = i3; \
   arr[4] = i4; \
   arr[5] = i5; \
   arr[6] = i6; \
   arr[7] = i7; \
}

#define Edumb_for(){\
   for (it = _1nn_list.begin(); it != _1nn_list.end(); it++) {\
      StateDB = _atoms[it->first][it->second.first][it->second.second].atom_type;\
      if (StateDB != 3 && StateDB != 7 && StateDB != 12\
          && StateDB != 1 && StateDB != 0)\
            eb += 0.70;\
   }\
   for (it = _2nn_list.begin(); it != _2nn_list.end(); it++) {\
      StateDB = _atoms[it->first][it->second.first][it->second.second].atom_type;\
      if (StateDB != 3 && StateDB != 7 && StateDB != 12\
         && StateDB != 1 && StateDB != 0)\
            eb += 0.48;\
   }\
}

#endif //MISA_KMC_MACROS_H
