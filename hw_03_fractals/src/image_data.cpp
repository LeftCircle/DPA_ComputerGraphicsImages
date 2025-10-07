#include "image_data.h"

using namespace OIIO;

ImageData::ImageData()
	: _width(0), _height(0), _channels(0), _image_data_ptr(nullptr) {}

ImageData::~ImageData() {
}

// Deep copy constructor and assignement operator
ImageData::ImageData(const ImageData& other)
	: _width(other._width), _height(other._height), _channels(other._channels) {
	_file_name = other._file_name;
	_file_type = other._file_type;
	int data_len = other.get_data_len();
	_image_data_ptr = std::make_unique<float[]>(data_len);
	std::copy(other._image_data_ptr.get(), other._image_data_ptr.get() + data_len, _image_data_ptr.get());
}

ImageData ImageData::duplicate() const {
	return ImageData(*this);
}

ImageData& ImageData::operator=(const ImageData& other) {
	if (this != &other) {
		_width = other._width;
		_height = other._height;
		_channels = other._channels;
		_file_name = other._file_name;
		_file_type = other._file_type;
		int data_len = other.get_data_len();
		_image_data_ptr = std::make_unique<float[]>(data_len);
		std::copy(other._image_data_ptr.get(), other._image_data_ptr.get() + data_len, _image_data_ptr.get());
	}
	return *this;
}

void ImageData::set_dimensions(int width, int height, int channels) {
	_width = width;
	_height = height;
	_channels = channels;
	int data_len = get_data_len();
	_image_data_ptr = std::make_unique<float[]>(data_len);
}

int ImageData::get_data_len() const {
	return _width * _height * _channels;
}

long ImageData::get_index(int x, int y, int channel) const {
	if (x < 0 || x >= _width || y < 0 || y >= _height || channel < 0 || channel >= _channels) {
		throw std::out_of_range("Invalid coordinates or channel");
	}
	return channel + _channels * (x + y * _width);
}

long ImageData::get_index(int x, int y) const {
	if (x < 0 || x >= _width || y < 0 || y >= _height) {
		throw std::out_of_range("Invalid coordinates");
	}
	return _channels * (x + y * _width);
}

void ImageData::set_pixel_values(int x, int y, const std::vector<float>& values) {
	if (x < 0 || x >= _width || y < 0 || y >= _height || values.size() != static_cast<size_t>(_channels)) {
		throw std::out_of_range("Invalid coordinates or values size");
	}
	int index = (y * _width + x) * _channels;
	for (int c = 0; c < _channels; ++c) {
		_image_data_ptr[index + c] = values[c];
	}
}

void ImageData::set_pixel_values(const std::vector<float>& values) {
	if (values.size() != static_cast<size_t>(get_data_len())) {
		throw std::out_of_range("Invalid values size");
	}
	#pragma omp parallel for
	for (int i = 0; i < get_data_len(); ++i) {
		_image_data_ptr[i] = values[i];
	}
}

void ImageData::set_pixel_values(const float val){
	#pragma omp parallel for
	for (int i = 0; i < get_data_len(); ++i) {
		_image_data_ptr[i] = val;
	}
}

void ImageData::set_first_three_channels(int x, int y, const std::vector<float>& values){
	int index = (y * _width + x) * _channels;
	for (int c = 0; c < 3; ++c) {
		_image_data_ptr[index + c] = values[c];
	}
}

void ImageData::add_values(int x, int y, float r, float g, float b, float a){
	assert(_channels >= 4);
	int start_index = (y * _width + x) * _channels;
	_image_data_ptr[start_index] += r;
	_image_data_ptr[start_index + 1] += g;
	_image_data_ptr[start_index + 2] += b;
	_image_data_ptr[start_index + 3] += a;
}

void ImageData::scale_values(float scale_factor){
	const int len = get_data_len();
	#pragma omp parallel for
	for (int i = 0; i < get_data_len(); i++){
		_image_data_ptr[i] *= scale_factor;
	}
}

void ImageData::scale_pixel_values(const int x, const int y, const float scale_factor){
	int index = get_index(x, y);
	for (int c = 0; c < _channels; c++){
		_image_data_ptr[index + c] *= scale_factor;
	}
}

/* from https://openimageio.readthedocs.io/en/latest/imageinput.html*/
void ImageData::oiio_read(const char* filename) {
	auto inp = ImageInput::open(filename);
    if (!inp) {
        std::cerr << "Error opening image file: " << filename << std::endl;
        return;
    }
	_file_name = StringFuncs::get_file_name(std::string(filename));
	_file_type = StringFuncs::get_file_type(std::string(filename));
    const ImageSpec& spec = inp->spec();
    int xres              = spec.width;
    int yres              = spec.height;
    int nchannels         = spec.nchannels;
    std::vector<float> pixels(xres * yres * nchannels);
    //inp->read_image(0 /*subimage*/, 0 /*miplevel*/, 0 /*chbegin*/,
    //                nchannels /*chend*/, make_span(pixels));
    inp->read_image(0, 0, 0, nchannels, TypeDesc::FLOAT, &pixels[0]);
	inp->close();

	set_dimensions(xres, yres, nchannels);
	std::cout << "Image dimensions: " << _width << " x " << _height << " x " << _channels << std::endl;
	set_pixel_values(pixels);

}

void ImageData::oiio_write() {
	if (!_image_data_ptr) {
		std::cerr << "No image data to write." << std::endl;
		return;
	}
	std::string filename = get_output_file_name();
	std::unique_ptr<ImageOutput> out = ImageOutput::create(filename);
	if (!out) {
		std::cerr << "Error creating image file: " << filename << std::endl;
		return;
	}
	ImageSpec spec(_width, _height, _channels, TypeDesc::FLOAT);
	if (!out->open(filename, spec)) {
		std::cerr << "Error opening image file for writing: " << filename << std::endl;
		return;
	}
	out->write_image(TypeDesc::FLOAT, _image_data_ptr.get());
	out->close();
}

std::vector<float> ImageData::get_pixel_values(int x, int y) {
	std::vector<float> pixel_data(_channels);
	int start_index = get_index(x, y);
	for (int c = 0; c < _channels; c++) {
		pixel_data[c] = _image_data_ptr[start_index + c];
	}
	return pixel_data;
}

const std::vector<float> ImageData::get_pixel_values(int x, int y) const {
	std::vector<float> pixel_data(_channels);
	int start_index = get_index(x, y);
	for (int c = 0; c < _channels; c++) {
		pixel_data[c] = _image_data_ptr[start_index + c];
	}
	return pixel_data;
}

void ImageData::get_pixel_values(int x, int y, std::vector<float>& values) const {
	int start_index = get_index(x, y);
	for (int c = 0; c < _channels; c++) {
		values[c] = _image_data_ptr[start_index + c];
	}
}

void ImageData::clear() {
	_width = 0;
	_height = 0;
	_channels = 0;
	_image_data_ptr.reset();
}