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

void Model::on_J_pressed() {
	_current_state = JULIA_SET;
	_julia_iterations = DEFAULT_JULIA_ITERATIONS;
	_julia_range = DEFAULT_JULIA_RANGE;
	std::cout << "Applying Julia Set with " << _julia_iterations << " iterations and range " << _julia_range << std::endl;
	apply_julia_set(_julia_iterations, _julia_range);
}

void Model::on_right_arrow_pressed() {
	// Increase iterations
	_julia_iterations += JULIA_ITERATION_STEP;
	std::cout << "Increasing Julia Set iterations to " << _julia_iterations << std::endl;
	apply_julia_set(_julia_iterations, _julia_range);
}

void Model::on_left_arrow_pressed() {
	// Decrease iterations
	_julia_iterations = std::max(JULIA_ITERATION_STEP, _julia_iterations - JULIA_ITERATION_STEP);
	std::cout << "Decreasing Julia Set iterations to " << _julia_iterations << std::endl;
	apply_julia_set(_julia_iterations, _julia_range);
}

void Model::on_up_arrow_pressed() {
	// Decrease range
	_julia_range /= RANGE_SCALE_FACTOR;
	std::cout << "Decreasing Julia Set range to " << _julia_range << std::endl;
	apply_julia_set(_julia_iterations, _julia_range);
}

void Model::on_down_arrow_pressed() {
	// Increase range
	_julia_range *= RANGE_SCALE_FACTOR;
	std::cout << "Increasing Julia Set range to " << _julia_range << std::endl;
	apply_julia_set(_julia_iterations, _julia_range);
}


void Model::fractal_flames() {

	Spherical ifs_spherical(0.0, 0.0, 1.0, 1.0);
	Spherical ifs_spherical2(-0.5, 0.0, 0.1, 0.1);
	Spherical ifs_spherical3(0.0, 0.5, 0.5, 0.5);
	Spherical ifs_spherical4(0.0, -0.5, 0.3, 0.3);
	Sinusoidal ifs_sin;
	Sinusoidal ifs_sin_1;
	ifs_sin.set_trans_matrix(10.0, 5.0, 5, 0.0, 10.0, -5);
	ifs_sin_1.set_trans_matrix(10.0, 0.0, -5.0, 1.0, 10.0, 5.0);
	

	Linear ifs_linear(1.0, 1.0);
	Linear ifs_linear_grow_x(1.1, 1.0);
	Linear ifs_linear_grow_y(1.0, 1.1);
	Linear ifs_linear_shrink_x(1.0 / 1.1, 1.0);
	Linear ifs_linear_shrink_y(1.0, 1.0 / 1.1);
	Truncate ifs_truncate;

	negate_x neg_x;
	Rotation flip(PI);
	Rotation small_rot(PI / 75.0);
	Rotation three_way_symmetry_0(2.0 * PI / 3.0);
	Rotation three_way_symmetry_1(4.0 * PI / 3.0);
	Rotation three_way_symmetry_2(0.0);



	Randomize rand_final;

	std::vector<FlameIFSFunction*> ifs_functions = {
		// &ifs_linear,
		&ifs_sin,
		&ifs_sin_1,
		//&ifs_spherical
	};

	std::vector<float> ifs_weights = {
		1.0,
		1.0,
		// 1.0,
		//1.0
	};

	// Color palette 258
	std::vector<Color> colors = {
		Color(1.0f, 175.0f, 186.0f) / 255.0f,
		Color(204.0f, 171.0f, 214.0f) / 255.0f,
		//Color(242.0f, 251.0f, 122.0f) / 255.0f,
		//Color(0.0f, 251.0f, 122.0f) / 255.0f,
		//Color(0.0f, 129.0f, 227.0f) / 255.0f,
		//Color(1.0f, 98.0f, 115.0f) / 255.0f
	};
	
	std::vector<SymmetryIFS*> rotation_functions = {
		// &neg_x,
		// &three_way_symmetry_0,
		// &three_way_symmetry_1,
		// &three_way_symmetry_2
	};
	
	std::vector<float> rotation_weights = {
		// 1.0,
		// 1.0,
		// 1.0,
		// 1.0
	};


	IFSFunctionSystem ff_system(
		ifs_functions,
		ifs_weights,
		colors,
		rotation_functions,
		rotation_weights,
		&rand_final,
		image_editor->get_edited_image()->get_width(),
		image_editor->get_edited_image()->get_height()
	);

	int iters = 10000000;

	ff_system.fractal_frame(iters);

	image_editor->set_edited_image_to(ff_system.get_image());
}

void Model::apply_julia_set(const int iterations, const double range){

	// Initial parameters are fixed at the moment
	Point center(DEFAULT_CENTER);
	Point complex_center(DEFAULT_COMPLEX_CENTER);

	JuliaIterations jul(complex_center, iterations, 2);
	LUT<Color> color_lut{
		Color(1.0f, 175.0f, 186.0f) / 255.0f,
		Color(204.0f, 171.0f, 214.0f) / 255.0f,
		Color(242.0f, 251.0f, 122.0f) / 255.0f,
		Color(0.0f, 251.0f, 122.0f) / 255.0f,
		Color(0.0f, 1298.0f, 227.0f) / 255.0f
		//Color(1.0f, 98.0f, 115.0f) / 255.0f
	};
	image_editor->julia_set(center, range, jul, color_lut);
}