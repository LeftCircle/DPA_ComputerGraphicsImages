#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>


//namespace FileUtils{
std::vector<std::string> get_all_files_starting_with(const std::string& directory, const std::string& prefix);
void sort_based_on_number_suffix(std::vector<std::string>& file_names, bool remove_extension = false);
/* given path/to/file_name.extension, returns file_name*/
std::string get_file_name(const std::string& filepath);
//}

#endif 