#include "image_editing.h"

ImageEditor::~ImageEditor() {}


void ImageEditor::save_edited_image() {
	if (_edited_image){
		_edited_image->oiio_write();
	}
}

void ImageEditor::save_edited_image(const std::string& file_extension){
	if (_edited_image){
		_edited_image->oiio_write(file_extension);
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

void ImageEditor::downscale(const int new_width, const int new_height, bool ignore_alpha_of_zero){
	if (new_width > _edited_image->get_width() || new_height > _edited_image->get_height()){
		std::cout << "Width or height is not smaller" << std::endl;
		return;
	}

	// Create a new image
	ImageData new_img(new_width, new_height, _edited_image->get_channels());
	new_img.set_pixel_values(0.0f);
	const int channels = _edited_image->get_channels();
	const float dx_new = 1.0 / static_cast<float>(new_width);
	const float dy_new = 1.0 / static_cast<float>(new_height);

	const float new_widthf = static_cast<float>(new_width);
	const float new_heightf = static_cast<float>(new_height);

	const int old_width = _edited_image->get_width();
	const int old_height = _edited_image->get_height();
	const float old_widthf = static_cast<float>(old_width);
	const float old_heihgtf = static_cast<float>(old_height);
	std::vector<float> sum_of_weights(new_width * new_height, 0);
	// We only have to loop over each pixel in the OG image. Each pixel is basically falling into a 2D bucket based on the size
	// of the new image
	for(int j = 0; j < old_height; j++){
		float y = static_cast<float>(j) / old_heihgtf;
		int new_j = int(y * new_heightf);
		float new_y = static_cast<float>(new_j) / new_heightf;
		for(int i = 0; i < old_width; i++){
			float x = static_cast<float>(i) / old_widthf;
			int new_i = int(x * new_widthf);
			float new_x = static_cast<float>(new_i) / new_widthf;
			// Add the value to the bucket
			float x_w = x - new_x - dx_new;
			float y_w = y - new_y - dy_new;
			float weight = x_w * x_w + y_w * y_w;
			if (ignore_alpha_of_zero && channels == 4){
				float alpha = _edited_image->get_pixel_value(i, j, 3);
				if (alpha == 0.0f){
					continue;
				}
			}
			sum_of_weights[new_j * new_width + new_i] += weight;
			for (int c = 0; c < channels; c++){
				float pc = _edited_image->get_pixel_value(i, j, c);
				new_img.add_value(new_i, new_j, c, pc * weight);
			}
		}
	}

	// Now divide all values by the weight
	new_img.divide_each_pixel_by(sum_of_weights);
	_edited_image->set_to(new_img);

}

void ImageEditor::quantize(int levels){
	int w = _edited_image->get_width();
	int h = _edited_image->get_height();
	int channels = _edited_image->get_channels();
	for (int j = 0; j < h; j++){
		#pragma omp parallel for
		for (int i = 0; i < w; i++){
			for (int c = 0; c < channels; c++){
				float cc = _edited_image->get_pixel_value(i, j, c);
				cc = (float)(int)(cc * levels) / (float)levels;
				_edited_image->set_pixel_value(i, j, c, cc);
			}
		}
	}
}

void ImageEditor::palette_match(const std::vector<float>& colors){
	// Start by quantizing the image to the same number of colors as the 
	// palette. Then create a dict of color to closest palette
	const int n_colors = colors.size() / 3;
	quantize(n_colors);

	// Create a hash function for RGB color vectors
    auto color_hash = [](const std::vector<float>& color) {
        size_t h1 = std::hash<float>{}(color[0]);
        size_t h2 = std::hash<float>{}(color[1]);
        size_t h3 = std::hash<float>{}(color[2]);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
	};

	std::unordered_map<std::vector<float>, std::vector<float>, decltype(color_hash)> color_map(10, color_hash);

	int w = _edited_image->get_width();
    int h = _edited_image->get_height();

	// Collect unique quantized colors
    for (int j = 0; j < h; j++){
        for (int i = 0; i < w; i++){
            auto quantized_color = _edited_image->get_pixel_values(i, j);
            quantized_color.resize(3); // Only RGB
            
            if (color_map.find(quantized_color) == color_map.end()){
                // Find closest palette color
                float min_dist = std::numeric_limits<float>::max();
                std::vector<float> closest_color(3);
                
                for (int p = 0; p < n_colors; p++){
                    float dr = colors[p*3] - quantized_color[0];
                    float dg = colors[p*3+1] - quantized_color[1];
                    float db = colors[p*3+2] - quantized_color[2];
                    float dist = dr*dr + dg*dg + db*db;
                    
                    if (dist < min_dist){
                        min_dist = dist;
                        closest_color = {colors[p*3], colors[p*3+1], colors[p*3+2]};
                    }
                }
                color_map[quantized_color] = closest_color;
            }
			// Early out once we have mapped all colors
			// if (color_map.size() >= static_cast<size_t>(n_colors)){
			// 	break;
			// }
        }
    }
    
    // Apply palette mapping
    for (int j = 0; j < h; j++){
        for (int i = 0; i < w; i++){
            auto quantized_color = _edited_image->get_pixel_values(i, j);
            quantized_color.resize(3);
            auto it = color_map.find(quantized_color);
            if (it != color_map.end()){
                _edited_image->set_first_three_channels(i, j, it->second);
            } else{
				std::cout << "Color not found in map!" << std::endl;
			}
        }
    }
}

void ImageEditor::julia_set(const Point& center, const double range, const IFSFunction& fract, const LUT<Color>& color_lut){
	double w = (double)_edited_image->get_width();
	double h = (double)_edited_image->get_height();
	double R = 2.0; // b/c bounds are -1, 1
	for (int j = 0; j < _edited_image->get_height(); j++){
		#pragma omp parallel for
		for (int i = 0; i < _edited_image->get_width(); i++){
			Point P;
			P.x = 2.0 * (double)i / w - 1.0;
			P.y = 2.0 * (double)j / h - 1.0;
			P *= range;
			P += center;
			Point fractal_point = fract(P);
			
			double rate = fractal_point.magnitude() / R;
			Color frac_col(0.0, 0.0, 0.0);
			if (rate <= 1.0){
				frac_col = color_lut.lerp(rate);
			}
			std::vector<float> color_vec(3, 0.0);
			color_vec[0] = frac_col.r;
			color_vec[1] = frac_col.g;
			color_vec[2] = frac_col.b;
			_edited_image->set_first_three_channels(i, j, color_vec);
		}
	}
}


void ImageEditor::convert_to_contrast_units() {
	//
	const auto avg = _edited_image->get_average();
	const auto rms = _edited_image->get_rms(avg);
	for (int j = 0; j < _edited_image->get_height(); j++){
		for (int i = 0; i < _edited_image->get_width(); i++){ 
			auto pixels = _edited_image->get_pixel_values(i, j);
			for (int c = 0; c < _edited_image->get_channels(); c++){
				pixels[c] -= avg[c];
				pixels[c] /= rms[c];
			}
			_edited_image->set_pixel_values(i, j, pixels);
		}
	}
}

void ImageEditor::histogram_equalize(const int n_bins, bool ignore_alpha) {
	// Start by getting max and min
	std::vector<float> max = _edited_image->get_max();
	auto min = _edited_image->get_min();
	const int width = _edited_image->get_width();
	const int height = _edited_image->get_height();
	const float n_pixelsf = static_cast<float>(width * height);
	const int og_n_channels = _edited_image->get_channels();
	int n_channels = ignore_alpha ? 3 : og_n_channels;

	// Now create bins for each channel. We need one bin for each channel, so 
	std::vector<int> bins(n_channels * n_bins, 0);
	std::vector<float> bin_width(n_channels, 0.0);
	for (int i = 0; i < n_channels; i++){
		bin_width[i] = (max[i] - min[i]) / static_cast<float>(n_bins);
	}
	
	// Now accumulate the pixels into the bins
	for (int j = 0; j < _edited_image->get_height(); j++){
		for (int i = 0; i < _edited_image->get_width(); i++){
			for (int c = 0; c < n_channels; c++){
				float p = _edited_image->get_pixel_value(i, j, c);
				int bin_index = int((p - min[c]) / bin_width[c]);
				bin_index = std::min(std::max(bin_index, 0), n_bins - 1);
				bins[bin_index * n_channels + c] += 1;
			}
		}
	}
	
	// Create the probability distribution function. Just divide each bin
	// by the total number of pixels
	std::vector<float> pdf(n_channels * n_bins, 0.0f);
	std::vector<float> cdf(n_channels * n_bins, 0.0f);
	std::vector<float> channel_probabilities(n_channels, 0.0);
	for (int i = 0; i < n_channels * n_bins; i++){
		float bin_prob = static_cast<float>(bins[i]) / n_pixelsf;
		channel_probabilities[i % n_channels] += bin_prob;
		pdf[i] = bin_prob;
		cdf[i] = channel_probabilities[i % n_channels];
	}

	// Now loop over each pixel and replace the pixel with the cdf value of the
	// bin that the pixel falls into
	for (int j = 0; j < _edited_image->get_height(); j++){
		for (int i = 0; i < _edited_image->get_width(); i++){
			for (int c = 0; c < n_channels; c++){
				float p = _edited_image->get_pixel_value(i, j, c);
				int bin_index = int((p - min[c]) / bin_width[c]) * n_channels + c;
				bin_index = std::min(std::max(bin_index, 0), n_bins * n_channels - 1);
				_edited_image->set_pixel_value(i, j, c, cdf[bin_index]);
			}
		}
	}
}


ImageData ImageEditor::ensemble_average(const ImageData& img, int half_width){
	const int w = img.get_width();
	const int h = img.get_height();
	const int channels = img.get_channels();
	ImageData avg_img(w, h, channels);

	const float* img_data_ptr = img.get_pixel_ptr();
	for (int j = 0; j < h; j++){
		#pragma omp parallel for
		for (int i = 0; i < w; i++){
			auto avg = average_ensemble2Df(img_data_ptr, w, h, channels, i, j, half_width);
			avg_img.set_pixel_values(i, j, avg);
		}
	}
	return avg_img;
}

void ImageEditor::optical_flow(
	const std::vector<std::string>& image_sequence,
	const ImageData& img_to_flow,
	std::string output_dir	
){
	// Create a double buffer for updating/reading the flowed image
	ImageData flow_img;
	ImageData flow_img_b;
	flow_img = img_to_flow.duplicate();
	flow_img_b = img_to_flow.duplicate();

	ImageData* current = &flow_img;
	ImageData* next = &flow_img_b;
	
	const int w = img_to_flow.get_width();
	const int h = img_to_flow.get_height();
	const int channels = img_to_flow.get_channels();
	const int num_images = image_sequence.size();
	for (int img_idx = 0; img_idx < num_images - 1; img_idx++){
		const ImageData next_img(image_sequence[img_idx + 1].c_str());
		const ImageData curr_img(image_sequence[img_idx].c_str());

		if (!img_to_flow.dimensions_match(next_img) || !img_to_flow.dimensions_match(curr_img)){
			// TODO -> allow for resizing the flow target image
			throw std::invalid_argument("Optical flow target image dimensions do not match image sequence dimensions.");
			return;
		}
		
		// 1. Get the displacement images
		ImageData dIx = curr_img.get_x_gradient();
		ImageData dIy = curr_img.get_y_gradient();

		// 2. Generate ensemble averages Qxy = <<(Id - Iu) * dixy>>
		ImageData Qx = _build_ensemble_average_in_sequence(next_img, curr_img, dIx);
		ImageData Qy = _build_ensemble_average_in_sequence(next_img, curr_img, dIy);

		// 3. Now for the correlation matrix component images
		auto [c00, c11, c_off_diag] = _compute_correlation_matrix_components(dIx, dIy);

		
		ImageData velocity_field(w, h, 2 * channels); // (dx, dy) for each original channel
		_compute_velocity_field(Qx, Qy, c00, c11, c_off_diag, w, h, channels, velocity_field);

		// 5. Now that we have the velocity field, apply that to the iterated image.
		_apply_velocity_field(velocity_field, *current, *next);
		// Save the flowed image
		if (!output_dir.empty()){
			std::string out_filename = output_dir + "/flowed_" + std::to_string(img_idx + 1) + ".png";
			current->oiio_write_to(out_filename);
		}
		std::swap(current, next);
		_edited_image->set_to(*current);
	}

}

ImageData ImageEditor::_build_ensemble_average_in_sequence(
	const ImageData& next_image,
	const ImageData& current_image,
	const ImageData& image_gradient
){
	ImageData Q;

	// Id is the displaced image(the next image in the sequence) and Iu is the current image
	Q = next_image.duplicate();
	Q.subtract_then_multiply(current_image, image_gradient);

	// 2a. Now compute the ensemble averages of these
	Q = ensemble_average(Q);
	return Q;
}

std::tuple<ImageData, ImageData, ImageData> ImageEditor::_compute_correlation_matrix_components(
	const ImageData& dIx,
	const ImageData& dIy
){
	ImageData dIx_sq = dIx.duplicate();
	dIx_sq *= dIx;
	ImageData dIy_sq = dIy.duplicate();
	dIy_sq *= dIy;
	ImageData dIx_dIy = dIx.duplicate();
	dIx_dIy *= dIy;

	ImageData c00 = ensemble_average(dIx_sq);
	ImageData c11 = ensemble_average(dIy_sq);
	ImageData c_off_diag = ensemble_average(dIx_dIy);

	return {c00, c11, c_off_diag};
}

void ImageEditor::_compute_velocity_field(
	const ImageData& Qx,
	const ImageData& Qy,
	const ImageData& c00,
	const ImageData& c11,
	const ImageData& c_off_diag,
	int w, int h, int channels,
	ImageData& velocity_field
){

	// Now compute the velcity field with V = Q * C^-1
	// Inverse of 2x2 matrix C is (1/det) * [c11, -c01;
	//										 -c10, c00]
	// where det = c00 * c11 - c01 * c10
	for (int j = 0; j < h; j++){
		#pragma omp parallel for
		for (int i = 0; i < w; i++){
			// Start with the Qs component
			auto qx = Qx.get_pixel_values(i, j);
			auto qy = Qy.get_pixel_values(i, j);
			
			// Now get the C^-1 components
			auto c00_pix = c00.get_pixel_values(i, j);
			auto c11_pix = c11.get_pixel_values(i, j);
			auto coff_diag = c_off_diag.get_pixel_values(i, j);
			for (int c = 0; c < channels; c++){
				float det = c00_pix[c] * c11_pix[c] - coff_diag[c] * coff_diag[c];
				if (std::abs(det) < 1e-6){
					// Singular matrix, set velocity to zero
					velocity_field.set_pixel_value(i, j, c * 2, 0.0f);
					velocity_field.set_pixel_value(i, j, c * 2 + 1, 0.0f);
				} else {
					float inv_c00 = c11_pix[c] / det;
					float inv_c11 = c00_pix[c] / det;
					float inv_coff_diag = -coff_diag[c] / det;

					// Now compute D = Q * C^-1
					// Q = [qx, qy] = 1 x 2 matrix. 
					// This way Q * C^-1 = 1 x 2 * 2 x 2 = 1 x 2 matrix so we get (dx, dy)
					float dx = qx[c] * inv_c00 + qy[c] * inv_coff_diag;
					float dy = qx[c] * inv_coff_diag + qy[c] * inv_c11;
					
					velocity_field.set_pixel_value(i, j, c * 2, dx);
					velocity_field.set_pixel_value(i, j, c * 2 + 1, dy);
				}
			}
		}
	}
}

void ImageEditor::_apply_velocity_field(
	const ImageData& velocity_field,
	const ImageData& input_image,
	ImageData& output_image
){
	const int w = input_image.get_width();
	const int h = input_image.get_height();
	const int channels = input_image.get_channels();
	for (int j = 0; j < h; j++){
		float y = (float)j;
		#pragma omp parallel for
		for (int i = 0; i < w; i++){
			float x = (float)i;
			// We have per channel velocity, so we need to do the bilinear interp for just a given channel
			for (int c = 0; c < channels; c++){
				float dx = velocity_field.get_pixel_value(i, j, c * 2);
				float dy = velocity_field.get_pixel_value(i, j, c * 2 + 1);
				// Read from current, write to next
				float c_interp = input_image.interpolate_bilinear(x + dx, y + dy, c);
				output_image.set_pixel_value(i, j, c, c_interp);
			}
		}
	}
}

void ImageEditor::bilinear_interpolate_each_channel(){
	const int w = _edited_image->get_width();
	const int h = _edited_image->get_height();
	const int channels = _edited_image->get_channels();
	ImageData interp_img(w, h, channels);

	for (int j = 0; j < h; j++){
		//#pragma omp parallel for
		for (int i = 0; i < w; i++){
			for (int c = 0; c < channels; c++){
				float x = static_cast<float>(i);
				float y = static_cast<float>(j);
				float interp_val = _edited_image->interpolate_bilinear(x, y, c);
				interp_img.set_pixel_value(i, j, c, interp_val);
			}
			//auto values = _edited_image->interpolate_bilinear((float)i, (float)j);
			//interp_img.set_pixel_values(i, j, values);
		}
	}
	_edited_image->set_to(interp_img);
}