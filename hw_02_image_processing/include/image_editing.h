#ifndef IMAGE_EDITING_H
#define IMAGE_EDITING_H

#include <vector>

#include <image_data.h>

class ImageEditor {
public:
	ImageEditor(ImageData& image_data)
		: _starting_image(&image_data), _edited_image(new ImageData(image_data)) {}
	~ImageEditor();
	ImageData* get_starting_image() const { return _starting_image; }
	ImageData* get_edited_image() const { return _edited_image; }

	void 

	// Image editing functions
	void gamma_filter(float gamma);

private:
	ImageData* _starting_image;
	ImageData* _edited_image;


	// Possibly save a list of operations that have been applied. Could require a mapping of
	// inputs to operations and how to reverse them.
};



#endif // IMAGE_EDITING_H