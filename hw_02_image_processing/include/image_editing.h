#ifndef IMAGE_EDITING_H
#define IMAGE_EDITING_H

#include <vector>

#include <image_proc.h>

class ImageEditor {
public:

private:
	ImageProc* _starting_image;
	ImageProc* _edited_image;


	// Possibly save a list of operations that have been applied. Could require a mapping of
	// inputs to operations and how to reverse them.
}



#endif // IMAGE_EDITING_H