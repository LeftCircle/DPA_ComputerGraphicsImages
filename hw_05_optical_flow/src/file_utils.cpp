#include "file_utils.h"

//using namespace FileUtils;

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

std::string get_file_name(const std::string& filepath){
    size_t slash_pos = filepath.find_last_of("/\\");
    size_t dot_pos = filepath.find_last_of('.');
    if (dot_pos == std::string::npos || dot_pos == 0) {
        return ""; // No extension found or empty filename
    }
    size_t start = (slash_pos == std::string::npos) ? 0 : slash_pos + 1;
    return filepath.substr(start, dot_pos - start);
}

void sort_based_on_number_suffix(std::vector<std::string>& file_names, bool remove_extension){
    
    std::sort(file_names.begin(), file_names.end(),
        [](const std::string& a, const std::string& b) {
            auto extract_number = [](const std::string& filename) -> int {
                size_t i = filename.length() - 1;
                while (i > 0 && isdigit(filename[i])) {
                    --i;
                } 
                if (i == filename.length() - 1) {
                    return -1; // No trailing number
                }
                return std::stoi(filename.substr(i + 1));
            };
            int num_a = extract_number(a);
            int num_b = extract_number(b);
            return num_a < num_b;
        });
}