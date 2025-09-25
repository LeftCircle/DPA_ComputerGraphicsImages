#include "model.h"

Model* Model::pModel = nullptr;

Model::Model()
    : _image_data(nullptr), image_editor(nullptr), stencil(DEFAULT_STENCIL_HALF_WIDTH) {}

Model::~Model() {
    delete image_editor;
    image_editor = nullptr;
    _image_data = nullptr;
}

Model* create_model(ImageData& image_data) {
    Model* model = Model::instance();
    model->image_editor = new ImageEditor(image_data);
    return model;
	_image_data = &image_data;
	_image_editor = new ImageEditor(*_image_data);
}
