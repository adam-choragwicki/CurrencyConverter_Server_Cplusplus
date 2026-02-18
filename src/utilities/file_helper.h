#pragma once

#include <fstream>
#include <filesystem>
#include "spdlog/spdlog.h"

class FileHelper
{
public:
    FileHelper() = delete;

    static std::string loadFileContent(const std::string& path)
    {
        spdlog::debug("Loading '{}'", path);

        if (!fileExists(path))
        {
            throw std::runtime_error(std::format("File {} does not exist", path));
        }

        return readFileToString(path);
    }

    static std::string readFileToString(const std::string& path)
    {
        // Open in binary mode to preserve line endings/data integrity
        std::ifstream in(path, std::ios::in | std::ios::binary);

        if (!in)
        {
            throw std::runtime_error("Failed to open " + path);
        }

        std::ostringstream ss;
        ss << in.rdbuf();

        return ss.str();
    }

    static bool fileExists(const std::string& path)
    {
        if (std::filesystem::is_regular_file(path))
        {
            return true;
        }

        return false;
    }
};
