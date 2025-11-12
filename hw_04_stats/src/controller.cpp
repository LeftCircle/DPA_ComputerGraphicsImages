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
	ImageEditor _image_editor = Model::instance()->image_editor;
	switch (key)
	{
		case 27: // esc
			exit(0);
			break;
		case 'c':
			model->convert_to_contrast_units();
			break;
		case 'f':
			std::cout << "f key pressed! Should flip" << std::endl;
			_image_editor.flip();
			break;
		case 'F':
			std::cout << "Fractal Flame!" << std::endl;
			model->fractal_flames();
			std::cout << "Done" << std::endl; 
			break;
		case 'g':
			std::cout << "gamma of 0.9" << std::endl;
			_image_editor.gamma_filter(0.9f);
			break;
		case 'G':
			std::cout << "gamma of 1.1111" << std::endl;
			_image_editor.gamma_filter(1.0f + 1.0f / 9.0f);
			break;
		case 'J': {
			//std::cout << "Applying Julia fractal!" << std::endl;
			//std::pair<int, double> iters_and_range = _get_julia_set_paramters();
			//model->apply_julia_set(iters_and_range.first, iters_and_range.second);
			//std::cout << "Julia fractal finished!" << std::endl;
			model->on_J_pressed();
			break;
		}
		case 'j':
			std::cout << "Key j pressed" << std::endl;
			_image_editor.save_edited_image();
			break;
		case 'm': {
			std::cout << "max pixel value is " << _image_editor.get_edited_image()->get_max() << std::endl;
			break;
		}
		case 's':
			_apply_stencil();
			break;
	}
}

void Controller::special_keys(int key, int x, int y){
	Model* model = Model::instance();
	switch (key){
		case GLUT_KEY_UP:
			std::cout << "Up arrow pressed" << std::endl;
			model->on_up_arrow_pressed();
			break;
		case GLUT_KEY_DOWN:
			std::cout << "Down arrow pressed" << std::endl;
			model->on_down_arrow_pressed();
			break;
		case GLUT_KEY_RIGHT:
			std::cout << "Right arrow pressed" << std::endl;
			model->on_right_arrow_pressed();
			break;
		case GLUT_KEY_LEFT:
			std::cout << "Left arrow pressed" << std::endl;	
			model->on_left_arrow_pressed();
			break;
	}
}

std::pair<int, double> Controller::_get_julia_set_paramters(){
	int iters;
	double range;

	std::cout << "Enter the number of iterations: (int) " << std::flush;
	if (!(std::cin >> iters)){
		std::cin.clear();
		std::string discard;
		std::getline(std::cin, discard);
		std::cout << "Invalid parameters. Just using defaults" << "std::endl";
		return {100, 1.0};
	}
	std:: cout << "Enter a range can be of format ie 1.0 or 1.0e-6: (double)" << std::flush;
	if (!(std::cin >> range)) {
		std::cin.clear();
		std::string discard;
		std::getline(std::cin, discard);
		std::cout << "Invalid parameters. Just using defaults" << "std::endl";
		return {100, 1.0};
	}
	return {iters, range};
}

void Controller::_apply_stencil() {
	ImageEditor _image_editor = Model::instance()->image_editor;
	std::cout << "Applying stencil" << std::endl;
			
	Model::instance()->stencil.randomize_values();
	
	_image_editor.bounded_linear_convolution(Model::instance()->stencil);
	std::cout << "Stencil applied" << std::endl;
}