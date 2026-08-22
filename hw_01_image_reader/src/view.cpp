#include "view.h"

View* View::pView = nullptr;


View::View() {
	width = 0;
	height = 0;
	img_width = 0;
	img_height = 0;
}


View::~View() {
	// Cleanup if needed
}

void View::init( int argc, char** argv, int _width, int _height)
{
	width = _width;
	height = _height;
	img_width = _width;
	img_height = _height;

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( width, height );
	glutCreateWindow( "View" );

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glEnable( GL_DEPTH_TEST );

	glutKeyboardFunc([](unsigned char key, int x, int y) { Controller::instance()->keyboard(key, x, y); });
	glutDisplayFunc( [](void){ View::instance() -> display(); } );
	glutReshapeFunc( [](int w, int h){ View::instance() -> reshape(w,h); } );
	glutIdleFunc( [](){ View::instance() -> idle(); } );
}

void View::display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	if (_image_proc->get_channels() == 4){
		glDrawPixels( img_width, img_height, GL_RGBA, GL_FLOAT, _image_proc->get_pixel_ptr() );
	}
	else if (_image_proc->get_channels() == 3){
		glDrawPixels( img_width, img_height, GL_RGB, GL_FLOAT, _image_proc->get_pixel_ptr() );
	}
	else if (_image_proc->get_channels() == 1){
		glDrawPixels( img_width, img_height, GL_LUMINANCE, GL_FLOAT, _image_proc->get_pixel_ptr() );
	} else {
		// Default to RGB if channels are unexpected
		glDrawPixels( img_width, img_height, GL_RGB, GL_FLOAT, _image_proc->get_pixel_ptr() );
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void View::idle()
{
	
}

void View::reshape( int w, int h )
{
	width = w;
	height = h;

	glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

}

void View::main_loop()
{
	glutMainLoop();
}

View* create_view()
{
	return View::instance();
}