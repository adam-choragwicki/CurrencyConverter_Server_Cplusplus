#pragma once

#include <fstream>

class FileLoader
{
public:
    FileLoader() = delete;

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
        std::ifstream inputFile(path);
        return inputFile.good();
    }
};
