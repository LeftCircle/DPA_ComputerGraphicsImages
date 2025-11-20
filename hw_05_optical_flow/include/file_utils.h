#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include <filesystem>

namespace FileUtils{
    std::vector<std::string> get_all_files_starting_with(const std::string& directory, const std::string& prefix);
}

#endif 