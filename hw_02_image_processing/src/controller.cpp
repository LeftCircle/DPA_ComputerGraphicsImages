#include "controller.h"


Controller* Controller::pController = nullptr;

Controller* create_controller()
{
	Controller* ctrl = Controller::instance();
	return ctrl;
}

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
		case 'f':
			std::cout << "f key pressed! Should flip" << std::endl;
			_image_editor->flip();
			break;
		case 'g':
			std::cout << "gamma of 0.9" << std::endl;
			_image_editor->gamma_filter(0.9f);
			break;
		case 'G':
			std::cout << "gamma of 1.1111" << std::endl;
			_image_editor->gamma_filter(1.0f + 1.0f / 9.0f);
			break;
		case 'j':
			std::cout << "Key j pressed" << std::endl;
			_image_editor->save_edited_image();
			break;
		case 's':
			_apply_stencil();
			break;
	}
}

void Controller::_apply_stencil() {
	ImageEditor* _image_editor = Model::instance()->image_editor;
	std::cout << "Applying stencil" << std::endl;
			
	// Stencil could be resized here with
	// Model::instance()->stencil.resize(int new_half_width);
	
	Model::instance()->stencil.randomize_values();
	
	_image_editor->bounded_linear_convolution(Model::instance()->stencil);
	std::cout << "Stencil applied" << std::endl;
}