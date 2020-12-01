//
// Created by genshen on 2019/10/10.
//

#ifndef MISA_KMC_MODEL_ADAPTER_H
#define MISA_KMC_MODEL_ADAPTER_H

#include <comm/domain/colored_domain.h>
#include <type_define.h>
#include <utils/random/random.h>

/**
 * \brief this is kmc model adapter
 * \tparam E the event type
 */
template <typename E> class ModelAdapter {
public:
  typedef comm::Region<comm::_type_lattice_size> lat_region;

  ModelAdapter();

  /**
   * \brief calculate rates in a region in the given region
   * \return the sum of all rates.
   */
  virtual _type_rate calcRates(const lat_region region) = 0;

  /**
   * \brief select an event from rates list in the given region
   * \param excepted_rate excepted rate
   * \param sum_rates the total rates
   */
  virtual E select(const lat_region region, const _type_rate excepted_rate, const _type_rate sum_rates) = 0;

  virtual unsigned long defectSize() = 0;

  /**
   * \brief perform the kmv event.
   */
  virtual void perform(const E e) = 0;

  /**
   * \brief select an event and perform the event.
   * this is the wrapper function for \fn select() and \fn perform()
   * \param sum_rates the total rates
   */
  void selectAndPerform(const lat_region region, const _type_rate sum_rates);

  /**
   * \brief reindex defect list in a specific region.
   * \param region the region
   */
  virtual void reindex(const lat_region region) = 0;

  /**
   * \brief generate a float random number between [0,1)
   * \return
   */
  inline double rand() {
    return r::random(); // todo random number
  }
};

template <typename E> ModelAdapter<E>::ModelAdapter() {}

template <class E> void ModelAdapter<E>::selectAndPerform(const lat_region region, const _type_rate sum_rates) {
  E e = select(region, rand() * sum_rates, sum_rates);
  perform(e);
}

#endif // MISA_KMC_MODEL_ADAPTER_H
