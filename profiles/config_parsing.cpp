//
// Created by genshen on 2019/9/18.
//

#include "config_parsing.h"

ConfigParsing *ConfigParsing::m_pInstance = nullptr;

ConfigParsing::ConfigParsing() : configValues() {}

ConfigParsing *ConfigParsing::newInstance(const std::string config_file) {
    if (m_pInstance == nullptr) {
        m_pInstance = new ConfigParsing();  // todo delete
        // todo read file and start parsing data to member configValue
    }
    return m_pInstance;
}

ConfigParsing *ConfigParsing::getInstance() {
    if (m_pInstance == nullptr) {
        m_pInstance = new ConfigParsing();
    }
    return m_pInstance; // make sure there is a configure instance.
}

bool ConfigParsing::configureCheck() {
    return false;
}

void ConfigParsing::putConfigData(kiwi::Bundle &bundle) {
    configValues.packData(bundle);
}

void ConfigParsing::getConfigData(kiwi::Bundle &bundle) {
    configValues.unpackData(bundle);
}
