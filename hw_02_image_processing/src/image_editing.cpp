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

	const float* input_data = input_image.get_pixel_ptr();
	float* output_data = output_image.get_pixel_ptr();

	// The inner region of the image where the stencil is applied like normal
	for (int write_y = half_width; write_y < height - half_width; write_y++){
		for (int stencil_y_offset = -half_width; stencil_y_offset <= half_width; stencil_y_offset++){
			int stencil_y = stencil_y_offset + half_width;
			int input_y = write_y + stencil_y_offset;
			int in_y_times_width = input_y * width * channels;
			int out_y_times_width = write_y * width * channels;
			// Now process the entire row, and update the output data
			#pragma omp parallel for
			for (int write_x = half_width; write_x < width - half_width; write_x++){
				int write_x_address = write_x * channels;
				// Apply the stencil block then advance
				for (int stencil_x_offset = -half_width; stencil_x_offset <= half_width; stencil_x_offset++){
					int stencil_x = stencil_x_offset + half_width;
					int input_x = write_x * channels + stencil_x_offset * channels;
					int input_address = input_data[input_x + in_y_times_width];
					int output_address = output_data[write_x_address + out_y_times_width];
					for (int c = 0; c<= channels; c++){
						output_data[output_address] += input_data[input_address] * stencil(stencil_x, stencil_y);
					}
				}
			}

		}
	}

	// Now the slower top and bottom regions
	// _stencil_linear_convolution_vertical_boundary(stencil, input_image, output_image, true);
	// _stencil_linear_convolution_vertical_boundary(stencil, input_image, output_image, false);
	// _stencil_linear_convolution_horizontal_boundary(stencil, input_image, output_image, true);
	// _stencil_linear_convolution_horizontal_boundary(stencil, input_image, output_image, false);
}

void ImageEditor::_stencil_linear_convolution_horizontal_boundary(const Stencil& stencil, const ImageData& input_image, ImageData& output_image, bool bottom){
	int width = input_image.get_width();
	int height = input_image.get_height();
	int channels = input_image.get_channels();
	int half_width = stencil.get_halfwidth();

	const float* input_data = input_image.get_pixel_ptr();
	float* output_data = output_image.get_pixel_ptr();
	int start_y = bottom ? 0 : height - half_width;
	int end_y = bottom ? half_width : height;
	for (int write_y = start_y; write_y <= end_y; write_y++){
		for (int stencil_y_offset = -half_width; stencil_y_offset <= half_width; stencil_y_offset++){
			int stencil_y = stencil_y_offset + half_width;
			int input_y = write_y + stencil_y_offset;
			int in_y_times_width = input_y * width * channels;
			int out_y_times_width = write_y * width * channels;
			// Now process the entire row, and update the output data
			for (int write_x = 0; write_x < width; write_x++){
				int write_x_address = write_x * channels;
				// Apply the stencil block then advance
				for (int stencil_x_offset = -half_width; stencil_x_offset <= half_width; stencil_x_offset++){
					int stencil_x = stencil_x_offset + half_width;
					int input_x = write_x * channels + stencil_x_offset * channels;
					int input_address = input_data[input_x + in_y_times_width];
					int output_address = output_data[write_x_address + out_y_times_width];
					for (int c = 0; c<= channels; c++){
						float input_value;
						if (input_x < 0 || input_x >= width || input_y < 0 || input_y >= height){
							input_value = 0.0f;
						} else {
							input_value = input_data[input_address];
						}
						output_data[output_address] += input_value * stencil(stencil_x, stencil_y);
					}
				}
			}
		}
	}
}

void ImageEditor::_stencil_linear_convolution_vertical_boundary(const Stencil& stencil, const ImageData& input_image, ImageData& output_image, bool left){
	int width = input_image.get_width();
	int height = input_image.get_height();
	int channels = input_image.get_channels();
	int half_width = stencil.get_halfwidth();

	const float* input_data = input_image.get_pixel_ptr();
	float* output_data = output_image.get_pixel_ptr();
	int start_x = left ? 0 : width - half_width;
	int end_x = left ? width - half_width : width;
	for (int write_y = half_width; write_y <= height - half_width; write_y++){
		for (int stencil_y_offset = -half_width; stencil_y_offset <= half_width; stencil_y_offset++){
			int stencil_y = stencil_y_offset + half_width;
			int input_y = write_y + stencil_y_offset;
			int in_y_times_width = input_y * width * channels;
			int out_y_times_width = write_y * width * channels;
			// Now process the entire row, and update the output data
			for (int write_x = start_x; write_x < end_x; write_x++){
				int write_x_address = write_x * channels;
				// Apply the stencil block then advance
				for (int stencil_x_offset = -half_width; stencil_x_offset <= half_width; stencil_x_offset++){
					int stencil_x = stencil_x_offset + half_width;
					int input_x = write_x * channels + stencil_x_offset * channels;
					int input_address = input_data[input_x + in_y_times_width];
					int output_address = output_data[write_x_address + out_y_times_width];
					for (int c = 0; c<= channels; c++){
						float input_value;
						if (input_x < 0 || input_x >= width || input_y < 0 || input_y >= height){
							input_value = 0.0f;
						} else {
							input_value = input_data[input_address];
						}
						output_data[output_address] += input_value * stencil(stencil_x, stencil_y);
					}
				}
			}
		}
	}
}

// void ImageEditor::bounded_linear_convolution(const Stencil& stencil, const ImageData& input_image, ImageData& output_image) {
// 	if (stencil.get_halfwidth() < 1) {
// 		throw std::invalid_argument("Stencil half-width must be at least 1.");
// 	}
// 	if (input_image.get_width() != output_image.get_width() ||
// 		input_image.get_height() != output_image.get_height() ||
// 		input_image.get_channels() != output_image.get_channels()) {
// 		throw std::invalid_argument("Input and output images must have the same dimensions and channels.");
// 	}

// 	int width = input_image.get_width();
// 	int height = input_image.get_height();
// 	int channels = input_image.get_channels();
// 	int half_width = stencil.get_halfwidth();

// 	const float* input_data = input_image.get_pixel_ptr();
// 	float* output_data = output_image.get_pixel_ptr();

// 	for (int y = 0; y < height; y++) {
// 		int y_reference_min = y - half_width;
// 		int y_reference_max = y + half_width;
// 		#pragma omp parallel for
// 		for (int x = 0; x < width; x++) {
// 			std::vector<float> reference_pixel(channels, 0.0f);
// 			std::vector<float> new_pixel(channels, 0.0f);
// 			int x_reference_min = x - half_width;
// 			int x_reference_max = x + half_width;
// 			__m128 new_pixel_vec = _mm_setzero_ps();
// 			__m128 reference_pixel_vec = _mm_setzero_ps();
// 			float simd_result[4];
// 			// Accumulate values from the reference image
// 			for (int j = y_reference_min; j <= y_reference_max; j++) {
// 				if (j < 0 || j >= height) {
// 					continue;
// 				}
// 				int stencil_y = j - y + half_width;
// 				for (int i = x_reference_min; i <= x_reference_max; i++) {
// 					if (i < 0 || i >= width) {
// 						continue;
// 					}
// 					int stencil_x = i - x + half_width;
					
// 					//input_image.get_pixel_values(i, j, reference_pixel);
// 					const float* reference_pixel_ptr = &input_data[input_image.get_index(i, j)];
// 					float stencil_val = stencil(stencil_x, stencil_y);
					
// 					__m128 stencil_vec = _mm_set1_ps(stencil_val); // broadcast stencil value
// 					// load the channels into a simd register
// 					// Assume channels is 3
// 					__m128 pixel_vec = _mm_loadu_ps(reference_pixel_ptr);
// 					// multiply
// 					__m128 result_vec = _mm_mul_ps(pixel_vec, stencil_vec);

// 					// write back to new_pixel
// 					new_pixel_vec = _mm_add_ps(new_pixel_vec, result_vec);


// 					// for (int c = 0; c < channels; ++c) {
// 					// 	new_pixel[c] += reference_pixel[c] * stencil(stencil_x, stencil_y);
// 					// }
// 				}
// 			} // End stencil accumulation
// 			_mm_storeu_ps(simd_result, new_pixel_vec);
// 			for (int c = 0; c < channels; ++c) {
// 				new_pixel[c] = simd_result[c];
// 			}
// 			output_image.set_pixel_values(x, y, new_pixel);
// 		}
// 	}
// }

void ImageEditor::_stencil_simd_convolution(const float* __restrict__ stencil, const int half_width, const float* __restrict__ input_pixels, ImageData& output_image) {
	
}
