#ifndef IMAGE_EDITING_H
#define IMAGE_EDITING_H

#include <vector>

#include <image_proc.h>

class ImageEditor {
public:
	ImageEditor(ImageProc& img_proc)
		: _starting_image(&img_proc), _edited_image(new ImageProc(img_proc)) {}

	ImageProc* get_starting_image() const { return _starting_image; }
	ImageProc* get_edited_image() const { return _edited_image; }


private:
	ImageProc* _starting_image;
	ImageProc* _edited_image;


	// Possibly save a list of operations that have been applied. Could require a mapping of
	// inputs to operations and how to reverse them.
};



#endif // IMAGE_EDITING_H