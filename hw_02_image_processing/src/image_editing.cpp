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

	for (int y = 0; y < height; y++) {
		int y_reference_min = y - half_width;
		int y_reference_max = y + half_width;
		#pragma omp parallel for
		for (int x = 0; x < width; x++) {
			std::vector<float> reference_pixel(channels, 0.0f);
			std::vector<float> new_pixel(channels, 0.0f);
			int x_reference_min = x - half_width;
			int x_reference_max = x + half_width;
			__m128 new_pixel_vec = _mm_setzero_ps();
			__m128 reference_pixel_vec = _mm_setzero_ps();
			float simd_result[4];
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
					
					//input_image.get_pixel_values(i, j, reference_pixel);
					const float* reference_pixel_ptr = &input_data[input_image.get_index(i, j)];
					float stencil_val = stencil(stencil_x, stencil_y);
					
					__m128 stencil_vec = _mm_set1_ps(stencil_val); // broadcast stencil value
					// load the channels into a simd register
					// Assume channels is 3
					__m128 pixel_vec = _mm_loadu_ps(reference_pixel_ptr);
					// multiply
					__m128 result_vec = _mm_mul_ps(pixel_vec, stencil_vec);

					// write back to new_pixel
					new_pixel_vec = _mm_add_ps(new_pixel_vec, result_vec);


					// for (int c = 0; c < channels; ++c) {
					// 	new_pixel[c] += reference_pixel[c] * stencil(stencil_x, stencil_y);
					// }
				}
			} // End stencil accumulation
			_mm_storeu_ps(simd_result, new_pixel_vec);
			for (int c = 0; c < channels; ++c) {
				new_pixel[c] = simd_result[c];
			}
			output_image.set_pixel_values(x, y, new_pixel);
		}
	}
}

void ImageEditor::_stencil_simd_convolution(const float* __restrict__ stencil, const int half_width, const float* __restrict__ input_pixels, ImageData& output_image) {
	// // // To be implemented in the future
	// int width = output_image.get_width();
	// int height = output_image.get_height();
	// int channels = output_image.get_channels();

	// float* output_data = output_image.get_pixel_ptr();
	// const int stencil_2hw_plus_one = (2 * half_width + 1);
	// // Just do the inner loop where the stencil fully fits
	// for (int y = half_width; y < height - half_width; y++) {
	// 	int y_stencil_min = y - half_width;
	// 	int y_stencil_max = y + half_width;
	// 	#pragma omp parallel for
	// 	for (int x = half_width; x < width - half_width; x++) {
	// 		std::vector<float> reference_pixel(channels, 0.0f);
	// 		std::vector<float> new_pixel(channels, 0.0f);
	// 		int x_stencil_min = x - half_width;
	// 		int x_stencil_max = x + half_width;
	// 		// Simd stencil!
	// 		for (int j = y_stencil_min; j <= y_stencil_max; j++) {
	// 			int stencil_y = j - y + half_width;
	// 			for (int i = x_stencil_min; i + 4 <= x_stencil_max; i+= 4) {
	// 				// Load four values from the stencil
	// 				int stencil_x = i - x + half_width;
	// 				__m128 stencil_values = _mm_loadu_ps(&stencil[(stencil_y * stencil_2hw_plus_one) + stencil_x]);
	// 				input_image.get_pixel_values(i, j, reference_pixel);
	// 				for (int c = 0; c < channels; ++c) {
	// 					new_pixel[c] += reference_pixel[c] * stencil(stencil_x, stencil_y);
	// 				}
	// 			}
	// 		} // End stencil accumulation
	// 		output_image.set_pixel_values(x, y, new_pixel);
	// 	}
	// }
}
