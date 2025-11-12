#ifndef IMAGE_EDITING_H
#define IMAGE_EDITING_H

#include <vector>

#include <image_data.h>
#include <stencil.h>
#include <IFSFunction.h>
#include <lut.h>

class ImageEditor {
public:
	ImageEditor(ImageData& image_data)
		: _starting_image(&image_data), _edited_image(new ImageData(image_data)) {}
	ImageEditor() {};
	~ImageEditor();
	const std::shared_ptr<ImageData> get_starting_image() const { return _starting_image; }
	const std::shared_ptr<ImageData> get_edited_image() const { return _edited_image; }

	void save_edited_image();

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
	void histogram_equalize(const int n_bins = 500);

private:

	std::shared_ptr<ImageData> _starting_image;
	std::shared_ptr<ImageData> _edited_image;

};



#endif // IMAGE_EDITING_H