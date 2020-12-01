//
// Created by genshen on 2018-12-12.
//

#ifndef MISA_KMC_KMC_H
#define MISA_KMC_KMC_H

#include "box.h"
#include "event.h"
#include "plugin/event_listener.h"
#include "type_define.h"
#include <models/model_adapter.h>

/*!
 * \brief the model routine of KMC simulation, including rate calculation, event selecting
 * and execution implementation.
 */
class ABVIModel : public ModelAdapter<event::SelectedEvent> {

public:
  /**
   * \brief initialize kmc with simulation box.
   */
  explicit ABVIModel(Box *box, double v, double T);

  /**
   * \brief calculate the transition rates of each defect
   *  in a region at current process
   *
   * The calculating methods depends on the lattice type(single atom, vacancy and dumbbell).
   * Different types of lattice have different methods or formulas to calculate the rate.
   * see the implementation for more details.
   *
   * \param region a region, this function will calculate the transition rates in this region
   * \note the x lattice size is not doubled in \param region parameter
   * After this step, the rate of every transition direction of each lattice will be set.
   * \return return the sum of rates of all KMC events in this region,
   *  including dumbbell transition and vacancy transition and defect generation.
   */
  _type_rate calcRates(const lat_region region) override; // todo

  /**
   * \brief select an event randomly from rates list in a given region.
   *
   * \param excepted_rate which equals to total rate* random number between 0-1.
   * \param total_rates the sum rates
   * \note the x lattice size is not doubled in \param region parameter
   * \return the selected event.
   */
  event::SelectedEvent select(const lat_region region, const _type_rate excepted_rate,
                              const _type_rate sum_rates) override;

  /**
   * \brief perform the selected KMC event.
   *
   */
  void perform(const event::SelectedEvent event) override;

  void reindex(const lat_region region) override;

  /**
   * \brief set kmc event listener.
   * \param p_listener pointer to the event listener.
   */
  void setEventListener(EventListener *p_listener);

  unsigned long defectSize() override;

protected:
  double time = 0;

private:
  Box *box = nullptr; // todo init box pointer

  /**
   * \brief attempt frequency.
   */
  const double v;

  /**
   * \brief temperature
   */
  const double T;

  /**
   * \brief pointer to event listener.
   * event callback function will be called when executing a kmc event.
   */
  EventListener *p_event_listener = nullptr;

  /**
   * \brief it returns the rate of defect generation.
   * \return
   */
  _type_rate defectGenRate();
};

#endif // MISA_KMC_KMC_H
