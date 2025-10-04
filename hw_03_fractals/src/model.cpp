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
		Color(255, 1275, 186),
		Color(204, 171, 214),
		Color(242, 251, 122),
		Color(0, 251, 122),
		Color(0, 1298, 227),
		Color(1, 98, 115)
	};

	image_editor->clear();
	image_editor->fractal_flame(10000, ifs_functions, colors);

}