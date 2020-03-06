//
// Created by genshen on 2019/9/18.
//

#ifndef MISA_KMC_CONFIG_PARSING_H
#define MISA_KMC_CONFIG_PARSING_H

#include "config_values.h"
#include <config/config.h>
#include <yaml-cpp/yaml.h>

/**
 * \brief the ConfigParsing class in single instance mode.
 */
class ConfigParsing : public kiwi::config {
public:
  /**
   * \brief config values will be stored here.
   */
  conf::ConfigValues configValues;

  /**
   * Create a new config instance first,
   * then it will try to parse the yaml config file.
   * and config data from resolving will be stored in {@var configValues}
   * @param configureFilePath path of yaml config file.
   * @return an pointer to {@class ConfigParsing}.
   */
  static ConfigParsing *newInstance(const std::string config_file);

  /**
   * Just create an empty {@class ConfigParsing} class (not parsing yaml config
   * file).
   * @return
   */
  static ConfigParsing *getInstance();

  bool configureCheck();

private:
  // pointer of this {@class ConfigParsing} for single mode.
  static ConfigParsing *m_pInstance; // stored in static area.

  ConfigParsing();

  /**
   * parse yaml format config using lib: https://github.com/jbeder/yaml-cpp
   * visit the url above to get more information.
   * @param table
   */
  //    void resolveConfig(std::shared_ptr<cpptoml::table> table) override;

  /**
   * [master] put data into bundle, in which is used to buffer config data.
   * later, the bundle will be sent to the other processors (sync config).
   * @param bundle used for buffering config data.
   */
  void putConfigData(kiwi::Bundle &bundle) override;

  /**
   * [non-master] get config data from bundle after finishing sync.
   * @param bundle
   */
  void getConfigData(kiwi::Bundle &bundle) override;

  /**
   * \brief
   * \param config_file
   */
  void parse(const std::string config_file);

  bool parseBox(const YAML::Node &yaml_box);

  bool parseSim(const YAML::Node &yaml_sim);

  bool parseCreate(const YAML::Node &yaml_create);

  bool parseSeeds(const YAML::Node &yaml_seeds);

  bool parseOutput(const YAML::Node &output);
};

#endif // MISA_KMC_CONFIG_PARSING_H
