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
// 			// Accumulate values from the reference image
// 			for (int j = y_reference_min; j <= y_reference_max; j++) {
// 				if (j < 0 || j >= height) {
// 					continue;
// 				}
// 				int stencil_y = j - y;
// 				for (int i = x_reference_min; i <= x_reference_max; i++) {
// 					if (i < 0 || i >= width) {
// 						continue;
// 					}
// 					int stencil_x = i - x;
// 					input_image.get_pixel_values(i, j, reference_pixel);
// 					for (int c = 0; c < channels; ++c) {
// 						new_pixel[c] += reference_pixel[c] * stencil(stencil_x, stencil_y);
// 					}
// 				}
// 			} // End stencil accumulation
// 			output_image.set_pixel_values(x, y, new_pixel);
// 		}
// 	}
// }

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

	for (int j=0; j < height; j++) {
		int jmin = j - half_width;
		int jmax = j + half_width;
		//#pragma omp parallel for
		for(int i = 0; i < width; i++)
		{
			int imin = i - half_width;
			int imax = i + half_width;
			std::vector<float> pixel(channels,0.0);
			std::vector<float> sample(channels,0.0);
			for(int jj=jmin;jj<=jmax;jj++)
			{
				int stencilj = jj-j + half_width;
				int jjj = jj;
				if(jjj < 0 ){ jjj += height; }
				if(jjj >= height ){ jjj -= height; }
				for(int ii=imin; ii<=imax; ii++)
				{
					int stencili = ii-i + half_width;
					int iii = ii;
					if(iii < 0 ){ iii += width; }
					if(iii >= width ){ iii -= width; }
					const float& stencil_value = stencil(stencili, stencilj);
					//std::cout << "Stencil value at (" << stencili << ", " << stencilj << "): " << stencil_value << std::endl;
					input_image.get_pixel_values(iii,jjj,sample);
				for(size_t c=0;c<sample.size();c++){ pixel[c] += sample[c] * stencil_value; }
				}
			}
			//std::cout << std::endl;
			output_image.set_pixel_values(i,j,pixel);
		}
	}
}