#include "tests.h"


const char* test_image_path = "/home/leftcircle/programming/clemson/DPA_ComputerGraphicsImages/hw_01_image_reader/images/test_image.jpeg";
const char* test_rgb_1_2_image_path = "/home/leftcircle/programming/clemson/DPA_ComputerGraphicsImages/hw_02_image_processing/test_images/rgb_1_2.png";

void test_command_line_parser(){
    const char* argv[] = {"program", "-flag", "value", "-t"};
    const int argc = 4;
    CommandLineParser parser(argc, const_cast<char**>(argv));
    assert(parser.find_val_for_flag("-flag") == "value");
    assert(parser.has_flag("-t"));
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

void test_read_image(){
    // Reads an image file and verifies its dimensions and channels.
    ImageProc img_proc;
    const char* filename = "/home/leftcircle/programming/clemson/DPA_ComputerGraphicsImages/hw_01_image_reader/images/test_image.jpeg";
    img_proc.oiio_read(filename);

    // Assuming we know the expected dimensions and channels of the test image
    int expected_width = 3296;  // Replace with actual expected width
    int expected_height = 2472; // Replace with actual expected height
    int expected_channels = 3; // Replace with actual expected channels

    std::cout << "Read image: " << filename << std::endl;
    std::cout << "Image dimensions: " << img_proc.get_width() << " x "
              << img_proc.get_height() << " x " << img_proc.get_channels() << std::endl;

    assert(img_proc.get_width() == expected_width);
    assert(img_proc.get_height() == expected_height);
    assert(img_proc.get_channels() == expected_channels);

    std::cout << "test_read_image passed." << std::endl;
}

void test_write_image(){
    // Writes an image to a file and incremenets the filename. 
    ImageProc img_proc;
    img_proc.oiio_read(test_image_path);
    std::string out_file = img_proc.get_output_file_name();
    img_proc.oiio_write();

    // confirm that the file exists:
    std::ifstream infile(out_file);
    assert(infile.good());
    infile.close();
}

void test_get_file_type(){
    const char* filename = "image.jpeg";
    const char* filename2 = "image.png";
    const char* filename3 = "image.tiff";

    std::string expected_type1 = "jpeg";
    std::string expected_type2 = "png";
    std::string expected_type3 = "tiff";

    std::string actual_type1 = StringFuncs::get_file_type(filename);
    std::string actual_type2 = StringFuncs::get_file_type(filename2);
    std::string actual_type3 = StringFuncs::get_file_type(filename3);

    assert(actual_type1 == expected_type1);
    assert(actual_type2 == expected_type2);
    assert(actual_type3 == expected_type3);
    std::cout << "test_get_file_type passed." << std::endl;
}

void test_get_file_name(){
    const char* filepath = "/path/to/image.jpeg";
    const char* filepath2 = "image.png";
    const char* filepath3 = "/another/path/image.tiff";

    std::string expected_name1 = "image";
    std::string expected_name2 = "image";
    std::string expected_name3 = "image";

    std::string actual_name1 = StringFuncs::get_file_name(filepath);
    std::string actual_name2 = StringFuncs::get_file_name(filepath2);
    std::string actual_name3 = StringFuncs::get_file_name(filepath3);

    assert(actual_name1 == expected_name1);
    assert(actual_name2 == expected_name2);
    assert(actual_name3 == expected_name3);
    std::cout << "test_get_file_name passed." << std::endl;
}

void test_image_editor_initialization(){
    ImageProc img_proc;
    img_proc.oiio_read(test_rgb_1_2_image_path);
    ImageEditor editor(img_proc);

    // Confirm that the pointers to the image and edited image are not null
    assert(editor.get_starting_image() != nullptr);
    assert(editor.get_edited_image() != nullptr);

    std::cout << "test_image_editor_initialization passed." << std::endl;

}

void test_test_image(){
    // The test image is a 10x10 image where the first pixels are
    // (255, 0, 0), (0, 255, 0), (0, 0, 255), (1, 1, 1), (2, 2, 2),
    ImageProc img_proc;
    
}

void test_gamma_filter(){


}


void Tests::run_tests() {
    test_get_image_length();
    test_command_line_parser();
    test_get_image_index();
    test_read_image();
    test_get_file_name();
    test_get_file_type();
    test_write_image();
    test_image_editor_initialization();
    test_gamma_filter();
    std::cout << "All tests passed!" << std::endl;
}