//
// Created by genshen on 2019/12/21.
//

#include <abvi/kmc.h>
#include <gtest/gtest.h>

TEST(abvi_model_kmc_perform_test, abvi_model_test) {
  BoxBuilder builder{};
  // todo type conversion.
  builder.setBoxSize(10, 10, 10);
  builder.setGhostSize(4, 4, 4);
  builder.setGlobalLatSize(10, 10, 10);
  builder.setGlobalBaseLat(0, 0, 0);
  Box *box = builder.build();

  LatListMeta lm = box->lattice_list->meta;
  for (comm::_type_lattice_coord z = lm.ghost_z; z < (lm.ghost_z + lm.box_z); z++) {
    for (comm::_type_lattice_coord y = lm.ghost_y; y < (lm.ghost_y + lm.box_y); y++) {
      for (comm::_type_lattice_coord x = lm.ghost_x; x < (lm.ghost_x + lm.box_x); x++) {
        box->lattice_list->getLat(x, y, z).type = LatticeTypes{LatticeTypes::Fe};
      }
    }
  }

  box->lattice_list->getLat(8, 8, 8).type = LatticeTypes{LatticeTypes::V};

  ABVIModel model(box, 1e-12, 300);
  // vacancy trans from (8,8,8) to (9,9,9)
  event::SelectedEvent event{
      .event_type = event::EventType::VacancyTrans,
      .from_id = box->lattice_list->getId(8, 8, 8),
      .to_id = box->lattice_list->getId(9, 8, 8),
      .target_tag = 7,
  };
  model.perform(event);

  EXPECT_EQ(box->lattice_list->getLat(8, 8, 8).type._type, LatticeTypes::Fe);
  EXPECT_EQ(box->lattice_list->getLat(9, 8, 8).type._type, LatticeTypes::V);
  delete box; // todo
}
