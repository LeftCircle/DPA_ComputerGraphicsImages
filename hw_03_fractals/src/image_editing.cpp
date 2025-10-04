#include "image_editing.h"

ImageEditor::~ImageEditor() {
	delete _edited_image;
	_edited_image = nullptr;
	_starting_image = nullptr;
}


void ImageEditor::save_edited_image() {
	if (_edited_image){
		_edited_image->oiio_write();
	}
}

void ImageEditor::gamma_filter(float gamma) {
	if (gamma <= 0.0f) {
		throw std::invalid_argument("Gamma value must be greater than 0.");
	}
	
	float* data = _edited_image->get_pixel_ptr();

	int data_len = _edited_image->get_data_len();
	#pragma omp parallel for
	for (int i = 0; i < data_len; ++i) {
		data[i] = pow(data[i], gamma);
	}
}

/* Applies a linear convolution on the edited image */
void ImageEditor::bounded_linear_convolution(const Stencil& stencil) {
	const ImageData reference_image = _edited_image->duplicate();
	bounded_linear_convolution(stencil, reference_image, *_edited_image);
}

void ImageEditor::flip() {
	int row_size = _edited_image->get_width() * _edited_image->get_channels();
	int _height = _edited_image->get_height();
	float* _image_data_ptr = _edited_image->get_pixel_ptr();
	std::vector<float> temp_row(row_size);

	for (int y = 0; y < _height / 2; ++y) {
		float* top_row    = _image_data_ptr + y * row_size;
		float* bottom_row = _image_data_ptr + (_height - 1 - y) * row_size;

		std::copy(top_row, top_row + row_size, temp_row.data());
		std::copy(bottom_row, bottom_row + row_size, top_row);
		std::copy(temp_row.data(), temp_row.data() + row_size, bottom_row);
	}
}

void ImageEditor::bounded_linear_convolution(const Stencil& stencil, const ImageData& input_image, ImageData& output_image) {
	if (stencil.get_halfwidth() < 1) {
		throw std::invalid_argument("Stencil half-width must be at least 1.");
	}
	if (input_image.get_width() != output_image.get_width() ||
		input_image.get_height() != output_image.get_height() ||
		input_image.get_channels() != output_image.get_channels()) {
		throw std::invalid_argument("Input and output images must have the same dimensions and channels.");
	}

	int width = input_image.get_width();
	int height = input_image.get_height();
	int channels = input_image.get_channels();
	int half_width = stencil.get_halfwidth();

	for (int y = 0; y < height; y++) {
		int y_reference_min = y - half_width;
		int y_reference_max = y + half_width;
		#pragma omp parallel for
		for (int x = 0; x < width; x++) {
			std::vector<float> reference_pixel(channels, 0.0f);
			std::vector<float> new_pixel(channels, 0.0f);
			int x_reference_min = x - half_width;
			int x_reference_max = x + half_width;
			// Accumulate values from the reference image
			for (int j = y_reference_min; j <= y_reference_max; j++) {
				if (j < 0 || j >= height) {
					continue;
				}
				int stencil_y = j - y + half_width;
				for (int i = x_reference_min; i <= x_reference_max; i++) {
					if (i < 0 || i >= width) {
						continue;
					}
					int stencil_x = i - x + half_width;
					input_image.get_pixel_values(i, j, reference_pixel);
					for (int c = 0; c < channels; ++c) {
						new_pixel[c] += reference_pixel[c] * stencil(stencil_x, stencil_y);
					}
				}
			} // End stencil accumulation
			output_image.set_pixel_values(x, y, new_pixel);
		}
	}
}

void ImageEditor::wrapping_linear_convolution(const Stencil& stencil){
	const ImageData reference_image = _edited_image->duplicate();
	wrapping_linear_convolution(stencil, reference_image, *_edited_image);
}

void ImageEditor::wrapping_linear_convolution(const Stencil& stencil, const ImageData& input_image, ImageData& output_image){
	if (stencil.get_halfwidth() < 1) {
		throw std::invalid_argument("Stencil half-width must be at least 1.");
	}
	if (input_image.get_width() != output_image.get_width() ||
		input_image.get_height() != output_image.get_height() ||
		input_image.get_channels() != output_image.get_channels()) {
		throw std::invalid_argument("Input and output images must have the same dimensions and channels.");
	}

	int width = input_image.get_width();
	int height = input_image.get_height();
	int channels = input_image.get_channels();
	int half_width = stencil.get_halfwidth();

	for (int y = 0; y < height; y++) {
		int y_reference_min = y - half_width;
		int y_reference_max = y + half_width;
		#pragma omp parallel for
		for (int x = 0; x < width; x++) {
			std::vector<float> reference_pixel(channels, 0.0f);
			std::vector<float> new_pixel(channels, 0.0f);
			int x_reference_min = x - half_width;
			int x_reference_max = x + half_width;
			// Accumulate values from the reference image
			for (int j = y_reference_min; j <= y_reference_max; j++) {
				int input_y = j;
				if (input_y < 0){
					input_y = height + input_y;
				} else if (input_y >= height){
					input_y -= height;
				}
				int stencil_y = j - y + half_width;
				for (int i = x_reference_min; i <= x_reference_max; i++) {
					int input_x = i;
					if (input_x < 0) {
						input_x += width;
					} else if (input_x >= width){
						input_x -= width;
					}
					int stencil_x = i - x + half_width;
					input_image.get_pixel_values(input_x, input_y, reference_pixel);
					for (int c = 0; c < channels; ++c) {
						new_pixel[c] += reference_pixel[c] * stencil(stencil_x, stencil_y);
					}
				}
			} // End stencil accumulation
			output_image.set_pixel_values(x, y, new_pixel);
		}
	}
}


void ImageEditor::clear(){
	_edited_image->set_pixel_values(0.0f);
}

// ------------------------------------------------------------------------------------------
// fractal things!
// ------------------------------------------------------------------------------------------

void ImageEditor::fractal_flame(
								int iters,
								std::vector<IFSFunction*>& function_ptrs,
								std::vector<Color>& colors)
	{
	srand48(time(nullptr));
	if (function_ptrs.empty() || colors.empty() || !_edited_image) {
		throw std::runtime_error("Null or empty input to fractal_flame");
	}
	_edited_image->set_pixel_values(0.0f);
	int n_colors = colors.size();
	int n_funcs = function_ptrs.size();
	Point p(2 * drand48() - 1, 2 * drand48() - 1);
	Color color(0.0f, 0.0f, 0.0f);
	int width = _edited_image->get_width();
	int height = _edited_image->get_height();
	std::vector<float> rgb(3, 0.0f);
	int updated_pixels = 0;
	p = Point(2 * drand48() - 1, 2 * drand48() - 1);
	for (int i = 1; i <= iters; i++){
		IFSFunction* rand_func = function_ptrs[lrand48() % n_funcs];
		Color rand_color = colors[lrand48() % n_colors];
		p = (*rand_func)(p);
		color = (color + rand_color) / 2.0f;
		int xp = int((p.x + 1.0f) / 2.0f * width);
		int yp = int((p.y + 1.0f) / 2.0f * height);
		if (xp < 0 || xp >= width || yp < 0 || yp >= height){
			continue;
		} else {
			//std::cout << "updating pixel " << p.x << ", " << p.y << std::endl;
			rgb[0] = color.r; rgb[1] = color.g; rgb[2] = color.b;
			_edited_image->set_first_three_channels(xp, yp, rgb);
			updated_pixels++;
		}
	}
	std::cout << "Updated " << updated_pixels << " pixels" << std::endl;
}