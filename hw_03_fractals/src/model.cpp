#include "model.h"

Model* Model::pModel = nullptr;

Model::Model()
	: image_editor(nullptr), stencil(DEFAULT_STENCIL_HALF_WIDTH), _image_data(nullptr) {}

Model::~Model() {
	delete image_editor;
	image_editor = nullptr;
	_image_data = nullptr;
}

Model* create_model(ImageData& image_data) {
	Model* model = Model::instance();
	model->image_editor = new ImageEditor(image_data);
	return model;
}


void Model::fractal_flames() {

	Spherical ifs_spherical;
	Sinusoidal ifs_sin;
	
	Rotation flip(PI);
	Rotation small_rot(PI / 10.0);

	Scale skew(1.0, 1.0);

	std::vector<IFSFunction*> ifs_functions = {
		&ifs_spherical,
		&ifs_sin
	};

	std::vector<float> ifs_weights = {
		1.0,
		1.0
	};

	std::vector<SymmetryIFS*> rotation_functions = {
		&flip,
		&small_rot
	};

	std::vector<float> rotation_weights = {
		1.0,
		1.0
	};

	// Color palette 258
	std::vector<Color> colors = {
		Color(1.0f, 175.0f, 186.0f) / 255.0f,
		Color(204.0f, 171.0f, 214.0f) / 255.0f
		//Color(242.0f, 251.0f, 122.0f) / 255.0f,
		//Color(0.0f, 251.0f, 122.0f) / 255.0f,
		//Color(0.0f, 1298.0f, 227.0f) / 255.0f,
		//Color(1.0f, 98.0f, 115.0f) / 255.0f
	};

	IFSFunctionSystem ff_system(
		ifs_functions,
		ifs_weights,
		colors,
		rotation_functions,
		rotation_weights,
		&skew,
		image_editor->get_edited_image()->get_width(),
		image_editor->get_edited_image()->get_height()
	);

	int iters = 10000;

	ff_system.fractal_frame(iters);

	image_editor->set_edited_image_to(ff_system.get_image());

}

void Model::apply_julia_set(){

	// Initial parameters are fixed at the moment
	Point center(0.03811, 0.01329);
	Point complex_center(0.8*cos(254.3 * 3.14159265/180.0), 0.8*sin(254.3 * 3.14159265/180.0));
	int iters = 100;
	const double range = 1.0;

	JuliaIterations jul(complex_center, 100, 2);
	LUT<Color> color_lut{
		Color(1.0f, 175.0f, 186.0f) / 255.0f,
		Color(204.0f, 171.0f, 214.0f) / 255.0f,
		Color(242.0f, 251.0f, 122.0f) / 255.0f,
		Color(0.0f, 251.0f, 122.0f) / 255.0f,
		Color(0.0f, 1298.0f, 227.0f) / 255.0f,
		Color(1.0f, 98.0f, 115.0f) / 255.0f
	};
	image_editor->julia_set(center, range, jul, color_lut);
}