#include "config_loader.h"

Config ConfigLoader::readConfigFile(const std::string& path)
{
    std::string fileContent = FilesHelper::loadFileContent(path);

    JsonReader jsonReader(fileContent);

    int port = std::stoi(jsonReader.getNumericValueAsString("port"));
    bool debug = std::stoi(jsonReader.getBooleanValueAsString("debug"));

    Config config(port, debug);

    return config;
}
