#pragma once

#include "config.h"
#include "json_processing/json_reader.h"
#include "utilities/files_helper.h"
#include <string>

class ConfigLoader
{
public:
    static Config readConfigFile(const std::string& path);
};
