//
// Created by genshen on 2019-03-08.
//

#ifndef MISA_KMC_EVENT_LISTENER_H
#define MISA_KMC_EVENT_LISTENER_H

#include "../recombine.h"

/**
 * \brief KMC event listener for all kinds of KMC events,
 * including vacancy transition, dumbbell transition, vacancy-dumbbell recombination and defect generation.
 */
class EventListener {
public:
  /**
   * \brief this function will be call when vacancy transition occurs if it is correctly set.
   * \note this func is called after performing transition.
   *
   * \param time_step current time steps
   * \param source_id local lattice id of source lattice
   * \param target_id local lattice id of target lattice
   * \param trans_atom the atom moves from target lattice to source lattice.
   */
  virtual void onVacancyTrans(const unsigned long time_step, const comm::_type_lattice_coord source_id,
                              const comm::_type_lattice_coord target_id, const LatticeTypes trans_atom) = 0;

  /**
   * \brief this function will be call when dumbbell transition occurs if it is correctly set.
   * \note this func is called after performing transition.
   *
   * \param time_step current time steps.
   * \param g_source_id local lattice id of source lattice
   * \param g_target_id local lattice id of target lattice
   * \param old_type_src type of source lattice before transition (it should be vacancy)
   * \param old_type_target type of target lattice before transition (it should be an single atom)
   * \param new_type_src type of source lattice after transition (it should be an atom)
   * \param new_type_target type of target lattice after transition (it should be vacancy)
   */
  virtual void onDumbbellTrans(const unsigned long time_step, const comm::_type_lattice_coord source_id,
                               const comm::_type_lattice_coord target_id, const LatticeTypes old_type_src,
                               const LatticeTypes old_type_target, const LatticeTypes new_type_src,
                               const LatticeTypes new_type_target) = 0;

  /**
   * \brief this function will be call when recombination occurs if it is correctly set.
   * \note this func is called before performing recombination.
   *
   * \param time_step current time steps.
   * \param rec recombine object.
   */
  virtual void onRecombine(const unsigned long time_step, const rec::Rec rec) = 0;

  /**
   * \brief this function will be call when defect generation occurs if it is correctly set.
   * lat_1_type + lat_2_type => V + combined_type
   *
   * \param time_step current time stpes
   * \note this func is called before performing defect generating.
   * \param lat_1_type type of the lattice chosen to be a dumbbell.
   * \param lat_2_type type of another lattice chosen to be a vacancy.
   * \param combined_type the type of combined dumbbell.
   */
  virtual void onDefectGenerate(const unsigned long time_step, LatticeTypes lat_1_type, LatticeTypes lat_2_type,
                                const LatticeTypes::lat_type combined_type) = 0;
};

#endif // MISA_KMC_EVENT_LISTENER_H
