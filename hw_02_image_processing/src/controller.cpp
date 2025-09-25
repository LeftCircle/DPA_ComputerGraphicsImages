#include "controller.h"


Controller* Controller::pController = nullptr;

Controller::Controller() {}

Controller::~Controller() {}

void Controller::keyboard( unsigned char key, int x, int y )
{
	ImageEditor* _image_editor = Model::instance()->image_editor;
	switch (key)
	{
		case 27: // esc
			exit(0);
			break;
		case 'j':
			std::cout << "Key j pressed" << std::endl;
			_image_editor->save_edited_image();
			break;
		case 'f':
			std::cout << "f key pressed! Should flip" << std::endl;
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