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
