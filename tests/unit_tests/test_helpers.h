#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

inline std::filesystem::path GetUnitTestDataDir()
{
    return std::filesystem::path(UNIT_TEST_DATA_DIR) / "data";
}

inline std::string ReadFileToString(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + path.string());
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

inline void WriteFileToString(const std::filesystem::path& path, const std::string& content)
{
    std::filesystem::create_directories(path.parent_path());

    std::ofstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Failed to write file: " + path.string());
    }

    file << content;
}

class ScopedCurrentPath
{
public:
    explicit ScopedCurrentPath(const std::filesystem::path& path)
        : previous_(std::filesystem::current_path())
    {
        std::filesystem::current_path(path);
    }

    ScopedCurrentPath(const ScopedCurrentPath&) = delete;
    ScopedCurrentPath& operator=(const ScopedCurrentPath&) = delete;

    ~ScopedCurrentPath()
    {
        std::error_code ec;
        std::filesystem::current_path(previous_, ec);
    }

private:
    std::filesystem::path previous_;
};

class ScopedTempDir
{
public:
    ScopedTempDir()
    {
        const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() / ("cc-unit-" + std::to_string(timestamp));
        std::filesystem::create_directories(path_);
    }

    ScopedTempDir(const ScopedTempDir&) = delete;
    ScopedTempDir& operator=(const ScopedTempDir&) = delete;

    ~ScopedTempDir()
    {
        std::error_code ec;
        std::filesystem::remove_all(path_, ec);
    }

    const std::filesystem::path& path() const
    {
        return path_;
    }

private:
    std::filesystem::path path_;
};
