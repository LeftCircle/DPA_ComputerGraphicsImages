#include <OpenImageIO/imageio.h>
#include <cstddef>
#include <memory>

#include "command_line_parser.h"
#include "image_data.h"
#include "tests.h"
#include "controller.h"
#include "view.h"


int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv);
	if (parser.has_flag("-t")){
		Tests tests;
		tests.run_tests();
	}

	const std::string file_name = parser.find_val_for_flag("-image");
	if (file_name.empty()){
		std::cerr << "No image file provided. Use -image <file_name>" << std::endl;
		return 1;
	}
	
	ImageData img;
	img.oiio_read(file_name.c_str());
	View* view = View::instance();
	Controller* controller = Controller::instance();
	//controller->set_image_data(&img);
	//view->set_image_data(controller->get_modified_image_ptr());
	view->init(argc, argv, img.get_width(), img.get_height());

	view->main_loop();
	return 0;
}