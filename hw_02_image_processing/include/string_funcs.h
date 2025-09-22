#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H


#include <string>

class StringFuncs {
public:
    static std::string get_file_type(const std::string& filename) {
        size_t dot_pos = filename.find_last_of('.');
        if (dot_pos == std::string::npos || dot_pos == filename.length() -
            1) {
            return ""; // No extension found or empty extension
        }
        return filename.substr(dot_pos + 1);
    }

    /* given path/to/file_name.extension, returns file_name*/
    static std::string get_file_name(const std::string& filepath) {
        size_t slash_pos = filepath.find_last_of("/\\");
        size_t dot_pos = filepath.find_last_of('.');
        if (dot_pos == std::string::npos || dot_pos == 0) {
            return ""; // No extension found or empty filename
        }
        size_t start = (slash_pos == std::string::npos) ? 0 : slash_pos + 1;
        return filepath.substr(start, dot_pos - start);
    }
};



#endif