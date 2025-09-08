#include <OpenImageIO/imageio.h>
#include <cstddef>
#include <memory>

#include "command_line_parser.h"
#include "tests.h"

using namespace OIIO;



// From https://openimageio.readthedocs.io/en/v3.0.9.0/imageinput.html
void simple_read(const std::string& filename)
{
    //const char* filename = "tahoe.tif";

    auto inp = ImageInput::open(filename);
    if (!inp)
        return;
    const ImageSpec& spec = inp->spec();
    int xres              = spec.width;
    int yres              = spec.height;
    int nchannels         = spec.nchannels;
    auto pixels           = std::unique_ptr<unsigned char[]>(
        new unsigned char[xres * yres * nchannels]);
    inp->read_image(0, 0, 0, nchannels, TypeDesc::UINT8, &pixels[0]);
    //inp->close();
}

// From https://github.com/AcademySoftwareFoundation/OpenImageIO/blob/main/docs/QuickStart.md
// void open_and_save_image()
// {
//     auto input = oiio.ImageInput.open("input.tif");
//     ImageSpec spec = input.spec();
//     size_t nbytes = spec.image_bytes();
//     std::unique_ptr<std::byte[]> pixels = new std::byte[nbytes];
//     input->read_image(0, 0, 0, spec.nchannels, TypeUnknown, pixels.get());
//     input->close();

//     auto output = ImageOutput.create("output.exr");
//     output->open("output.exr", spec);
//     output->write_image(TypeUnknown, pixels.get());
//     output->close();
// }

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv);
    if (parser.has_flag("-t")){
        Tests tests;
        tests.run_tests();
    }

    const std::string file_name = parser.find_val_for_flag("-image");
    simple_read(file_name);
    std::cout << "Main finished" << std::endl;
    return 0;
}