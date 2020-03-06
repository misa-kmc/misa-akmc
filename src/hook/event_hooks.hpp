//
// Created by genshen on 2019/12/20.
//

#ifndef MISA_KMC_EVENT_HOOKS_HPP
#define MISA_KMC_EVENT_HOOKS_HPP

class EventHooks {
public:
  /**
   * \brief be called after a kmc step finished.
   * \param step current kmc step
   */
  virtual void onStepFinished(unsigned long step) = 0;

  /**
   * \brief be called if all kmc steps are finished.
   */
  virtual void onAllDone() = 0;
};

#endif // MISA_KMC_EVENT_HOOKS_HPP
