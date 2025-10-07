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


	std::vector<IFSFunction*> ifs_functions = {
		&ifs_spherical
	};

	// Color palette 258
	std::vector<Color> colors = {
		Color(1.0f, 175.0f, 186.0f) / 255.0f,
		Color(204.0f, 171.0f, 214.0f) / 255.0f,
		Color(242.0f, 251.0f, 122.0f) / 255.0f,
		Color(0.0f, 251.0f, 122.0f) / 255.0f,
		Color(0.0f, 1298.0f, 227.0f) / 255.0f,
		Color(1.0f, 98.0f, 115.0f) / 255.0f
	};

	image_editor->clear();
	image_editor->fractal_flame(10000, ifs_functions, colors);

}