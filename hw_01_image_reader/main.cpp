#include <OpenImageIO/imageio.h>
#include <cstddef>
#include <memory>

#include "command_line_parser.h"
#include "image_proc.h"
#include "tests.h"
#include "StarterViewer.h"
#include "controller.h"

using namespace starter;


int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv);
    if (parser.has_flag("-t")){
        Tests tests;
        tests.run_tests();
    }

   StarterViewer* viewer = CreateViewer();

   std::vector<std::string> args;

   for(int i=0;i<argc;i++)
   {
      std::string s(argv[i]);
      args.push_back(s);
   }
   

    const std::string file_name = parser.find_val_for_flag("-image");
    if (file_name.empty()){
        std::cerr << "No image file provided. Use -image <file_name>" << std::endl;
        return 1;
    }

    ImageProc img;
    img.oiio_read(file_name.c_str());
    viewer->SetWidth(img.get_width());
    viewer->SetHeight(img.get_height());

    viewer->Init(args);

    viewer->pixels = img.get_pixel_ptr();

    viewer->MainLoop();
    return 0;
}