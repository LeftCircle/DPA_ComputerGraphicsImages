#include "image_proc.h"

using namespace OIIO;

ImageProc::ImageProc()
	: _width(0), _height(0), _channels(0), _image_data_ptr(nullptr) {}

ImageProc::~ImageProc() {
}

// Deep copy constructor and assignement operator
ImageProc::ImageProc(const ImageProc& other)
	: _width(other._width), _height(other._height), _channels(other._channels) {
	int data_len = other.get_data_len();
	_image_data_ptr = std::make_unique<float[]>(data_len);
	std::copy(other._image_data_ptr.get(), other._image_data_ptr.get() + data_len, _image_data_ptr.get());
}

ImageProc& ImageProc::operator=(const ImageProc& other) {
	if (this != &other) {
		_width = other._width;
		_height = other._height;
		_channels = other._channels;
		int data_len = other.get_data_len();
		_image_data_ptr = std::make_unique<float[]>(data_len);
		std::copy(other._image_data_ptr.get(), other._image_data_ptr.get() + data_len, _image_data_ptr.get());
	}
	return *this;
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

void ImageProc::set_pixel_values(const std::vector<float>& values) {
	if (values.size() != static_cast<size_t>(get_data_len())) {
		throw std::out_of_range("Invalid values size");
	}
	#pragma omp parallel for
	for (int i = 0; i < get_data_len(); ++i) {
		_image_data_ptr[i] = values[i];
	}
}

/* from https://openimageio.readthedocs.io/en/latest/imageinput.html*/
void ImageProc::oiio_read(const char* filename) {
	auto inp = ImageInput::open(filename);
    if (!inp) {
        std::cerr << "Error opening image file: " << filename << std::endl;
        return;
    }
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

void ImageProc::oiio_write(const char* filename) const {
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