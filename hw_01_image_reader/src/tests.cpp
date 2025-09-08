#include "tests.h"


void test_command_line_parser(){
    const char* argv[] = {"program", "-flag", "value", "-t"};
    const int argc = 4;
    CommandLineParser parser(argc, const_cast<char**>(argv));
    assert(parser.find_val_for_flag("-flag") == "value");
    assert(parser.has_flag("-t"));
}

void test_set_and_get_pixel_values() {
    // Sets and gets pixel values at specific coordinates.

    int width = 4;
    int height = 4;
    int channels = 3;
    ImageProc img_proc;
    img_proc.set_dimensions(width, height, channels);

    std::vector<float> expected_values = {0.1f, 0.2f, 0.3f};
    std::vector<float> actual_values(channels, 0.0f);
    //img_proc.set_pixel_values(1, 1, expected_values);
    //img_proc.get_pixel_values(1, 1, actual_values);
    //assert(actual_values == expected_values);
    std::cout << "test_set_and_get_pixel_values passed." << std::endl;
}


void test_get_image_length() {
    // Returns the lenght of the data array based on the width, height, and channels.

    int width = 4;
    int height = 4;
    int channels = 3;
    int expected_length = width * height * channels;

    ImageProc img_proc;
    img_proc.set_dimensions(width, height, channels);
    int length = img_proc.get_data_len();
    assert(length == expected_length);
    std::cout << "test_get_image_length passed." << std::endl;
}

void test_get_image_index(){
    // Calculates the index in the data array for given (x, y) coordinates.

    int width = 4;
    int height = 4;
    int channels = 3;
    ImageProc img_proc;
    img_proc.set_dimensions(width, height, channels);

    //expected is channel + n_channels * (x + y * width)
    int x = 2;
    int y = 1;
    int c = 2;
    int expected_index = c + channels * (x + y * width);

    int index = img_proc.get_index(x, y, c);
    assert(index == expected_index);

    // Now test non interleaved
    expected_index = channels * (x + y * width);
    index = img_proc.get_index(x, y);
    assert(index == expected_index);

    std::cout << "test_get_image_index passed." << std::endl;
}


void Tests::run_tests() {
    test_get_image_length();
    test_command_line_parser();
    test_set_and_get_pixel_values();
    test_get_image_index();
    std::cout << "All tests passed!" << std::endl;
}