#include "controller.h"


Controller* Controller::pController = nullptr;

Controller::Controller() {}

Controller::~Controller() {}

void Controller::keyboard( unsigned char key, int, int)
{
	switch (key)
	{
		case 27: // esc
			exit(0);
			break;
		case 'j':
			std::cout << "Key j pressed" << std::endl;
			_image_proc->oiio_write();
			break;
	}
}

Controller* create_controller()
{
	Controller* ctrl = Controller::instance();
	return ctrl;
}