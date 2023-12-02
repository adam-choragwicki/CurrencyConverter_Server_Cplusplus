#pragma once

#include <fstream>
#include <filesystem>
#include "spdlog/spdlog.h"

class FilesHelper
{
public:
    FilesHelper() = delete;

    static std::string loadFileContent(const std::string& path)
    {
        std::ifstream inputFile(path);

        if(inputFile.is_open())
        {
            std::string fileContent;
            std::string line;

            while(std::getline(inputFile, line))
            {
                fileContent += line;
            }

            inputFile.close();

            return fileContent;
        }
        else
        {
            throw std::runtime_error("Could not open " + path + " file");
        }
    }

    static bool fileExists(const std::string& path)
    {
        if(std::filesystem::is_regular_file(path))
        {
            return true;
        }
        else
        {
            spdlog::error("Path '{}' is not a file or it does not exist", path);
            return false;
        }
    }
};
