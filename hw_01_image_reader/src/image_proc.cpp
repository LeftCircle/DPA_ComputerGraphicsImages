#include "image_proc.h"

ImageProc::ImageProc()
	: _width(0), _height(0), _channels(0), _image_data_ptr(nullptr) {}

ImageProc::~ImageProc() {
}

void ImageProc::set_dimensions(int width, int height, int channels) {
	_width = width;
	_height = height;
	_channels = channels;
	int data_len = get_data_len();
	_image_data_ptr = std::make_unique<float[]>(data_len);
}

int ImageProc::get_data_len() const {
	return _width * _height * _channels;
}

long ImageProc::get_index(int x, int y, int channel) const {
	if (x < 0 || x >= _width || y < 0 || y >= _height || channel < 0 || channel >= _channels) {
		throw std::out_of_range("Invalid coordinates or channel");
	}
	return channel + _channels * (x + y * _width);
}

long ImageProc::get_index(int x, int y) const {
	if (x < 0 || x >= _width || y < 0 || y >= _height) {
		throw std::out_of_range("Invalid coordinates");
	}
	return _channels * (x + y * _width);
}

void ImageProc::set_pixel_values(int x, int y, const std::vector<float>& values) {
	if (x < 0 || x >= _width || y < 0 || y >= _height || values.size() != static_cast<size_t>(_channels)) {
		throw std::out_of_range("Invalid coordinates or values size");
	}
	int index = (y * _width + x) * _channels;
	for (int c = 0; c < _channels; ++c) {
		_image_data_ptr[index + c] = values[c];
	}
}