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
	Model* model = Model::instance();
	ImageEditor* _image_editor = Model::instance()->image_editor;
	switch (key)
	{
		case 27: // esc
			exit(0);
			break;
		case 'c':
			std::cout << "Clearing Image" << std::endl;
			_image_editor->clear();
		case 'f':
			std::cout << "f key pressed! Should flip" << std::endl;
			_image_editor->flip();
			break;
		case 'F':
			std::cout << "Fractal Flame!" << std::endl;
			model->fractal_flames();
			std::cout << "Done" << std::endl; 
			break;
		case 'g':
			std::cout << "gamma of 0.9" << std::endl;
			_image_editor->gamma_filter(0.9f);
			break;
		case 'G':
			std::cout << "gamma of 1.1111" << std::endl;
			_image_editor->gamma_filter(1.0f + 1.0f / 9.0f);
			break;
		case 'J':
			std::cout << "Applying Julia fractal!" << std::endl;
			model->apply_julia_set();
			std::cout << "Julia fractal finished!" << std::endl;
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
	// Stencil could be resized here with
	// Model::instance()->stencil.resize(int new_half_width);
	// or by changing the default stencil size in Model.h
	
	ImageEditor* _image_editor = Model::instance()->image_editor;
	std::cout << "Applying stencil" << std::endl;
			
	Model::instance()->stencil.randomize_values();
	
	_image_editor->bounded_linear_convolution(Model::instance()->stencil);
	std::cout << "Stencil applied" << std::endl;
}