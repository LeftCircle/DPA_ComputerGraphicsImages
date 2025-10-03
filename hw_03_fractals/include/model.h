#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <GL/glut.h>

#include "stencil.h"
#include "image_data.h"
#include "image_editing.h"

const int DEFAULT_STENCIL_HALF_WIDTH = 5;

// A singleton model class that will be used to drive GLUT inputs
class Model
{
public:
    static Model* instance() {
		if(pModel==nullptr)
		{
			pModel = new Model();
		}
		return pModel;
	}
    
	~Model();
    
	//void set_image_data(ImageData* image_data);
	const ImageData* get_modified_image_ptr() const { return image_editor->get_edited_image(); }
    
    // Public variables
    ImageEditor* image_editor;
    Stencil stencil = Stencil(DEFAULT_STENCIL_HALF_WIDTH);

private:
	ImageData* _image_data;

	static Model* pModel;

	Model();
	Model( const Model& );
	Model& operator= (const Model& );

};


Model* create_model(ImageData& image_data);

#endif