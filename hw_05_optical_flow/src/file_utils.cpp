#include "file_utils.h"

using namespace FileUtils;

std::vector<std::string> get_all_files_starting_with(const std::string& directory, const std::string& prefix){
    std::vector<std::string> matching_files;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (filename.rfind(prefix, 0) == 0) {
                matching_files.push_back(entry.path().string());
            }
        }
    }
    return matching_files;
}