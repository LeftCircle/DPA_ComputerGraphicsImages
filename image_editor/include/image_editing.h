#ifndef IMAGE_EDITING_H
#define IMAGE_EDITING_H

#include <vector>

#include <image_data.h>
#include <stencil.h>
#include <IFSFunction.h>
#include <lut.h>
#include <stats.h>

class ImageEditor {
public:
	ImageEditor(ImageData& image_data)
		: _starting_image(&image_data), _edited_image(new ImageData(image_data)) {}
	ImageEditor() {};
	~ImageEditor();
	const std::shared_ptr<ImageData> get_starting_image() const { return _starting_image; }
	const std::shared_ptr<ImageData> get_edited_image() const { return _edited_image; }

	void save_edited_image();
	void save_edited_image(const std::string& file_extension);

	// Image editing functions
	void gamma_filter(float gamma);
	void bounded_linear_convolution(const Stencil& stencil);
	void bounded_linear_convolution(const Stencil& stencil, const ImageData& input_image, ImageData& output_image);
	void wrapping_linear_convolution(const Stencil& stencil);
	void wrapping_linear_convolution(const Stencil& stencil, const ImageData& input_image, ImageData& output_image);
	void flip();
	void clear();
	
	void julia_set(const Point& center, const double range, const IFSFunction& fract, const LUT<Color>& color_lut);
	
	void set_edited_image_to(const ImageData& other_image)  {*_edited_image = other_image; }
	void convert_to_contrast_units();
	void histogram_equalize(const int n_bins = 500, bool ignore_alpha = false);

	void downscale(const int new_width, const int new_height, bool ignore_alpha_of_zero = false);
	void palette_match(const std::vector<float>& colors);
	void quantize(int levels);
	ImageData ensemble_average(const ImageData& image, int half_width = 1);
	ImageData gaussian_average(const ImageData& image, int half_width = 1);
	
	void optical_flow(
		const std::vector<std::string>& image_sequence,
		const ImageData& img_to_flow,
		std::string output_dir = "", 
		int iterations_per_image = 1
	);
	void bilinear_interpolate_each_channel();
	
	
private:
	ImageData _build_ensemble_average_in_sequence(
		const ImageData& next_image,
		const ImageData& current_image,
		const ImageData& image_gradient,
		int ensemble_avg_half_width = 2
	);
	std::tuple<ImageData, ImageData, ImageData> _compute_correlation_matrix_components(
		const ImageData& dIx,
		const ImageData& dIy,
		int ensemble_avg_half_width = 2
	);
	void _compute_velocity_field(
		const ImageData& Qx,
		const ImageData& Qy,
		const ImageData& c00,
		const ImageData& c11,
		const ImageData& c_off_diag,
		int w, int h, int channels,
		ImageData& velocity_field
	);
	void _apply_velocity_field(
		const ImageData& velocity_field,
		const ImageData& input_image,
		ImageData& output_image);

	std::shared_ptr<ImageData> _starting_image;
	std::shared_ptr<ImageData> _edited_image;

};



#endif // IMAGE_EDITING_H