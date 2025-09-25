#include "controller.h"


Controller* Controller::pController = nullptr;

Controller::Controller() {}

Controller::~Controller() {}

void Controller::set_image_data(ImageData* image_data) {
	_image_data = image_data;
	_image_editor = new ImageEditor(*_image_data);
	
}

void Controller::keyboard( unsigned char key, int x, int y )
{
	switch (key)
	{
		case 27: // esc
			exit(0);
			break;
		case 'j':
			std::cout << "Key j pressed" << std::endl;
			_image_editor->save_edited_image();
			break;
		case 'g':
			std::cout << "Key g pressed" << std::endl;
			_image_editor->gamma_filter(0.9f);
			break;
		case 'G':
			std::cout << "Key G pressed" << std::endl;
			_image_editor->gamma_filter(1.0f + 1.0f / 9.0f);
			break;
	}
}

Controller* create_controller()
{
	Controller* ctrl = Controller::instance();
	return ctrl;
}