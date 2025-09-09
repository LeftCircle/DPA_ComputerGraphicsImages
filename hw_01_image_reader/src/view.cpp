#include "view.h"

View* View::pView = nullptr;


View::View() {
	pixels = nullptr;
	width = 0;
	height = 0;
	camera_aspect = 1.0f;
}


View::~View() {
	// Cleanup if needed
	if (pixels) {
		delete[] pixels;
		pixels = nullptr;
	}
}

void View::init( int argc, char** argv, int _width, int _height)
{
	width = _width;
	height = _height;
	camera_aspect = (float)width/(float)height;

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( width, height );
	glutCreateWindow( "View" );

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glEnable( GL_DEPTH_TEST );

	glutKeyboardFunc([](unsigned char key, int x, int y) { Controller::instance()->keyboard(key, x, y); });
	glutDisplayFunc( [](void){ View::Instance() -> display(); } );
	glutReshapeFunc( [](int w, int h){ View::Instance() -> reshape(w,h); } );
	glutIdleFunc( [](){ View::Instance() -> idle(); } );
}

void View::display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glDrawPixels( width, height, GL_RGB, GL_FLOAT, pixels );

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
	camera_aspect = (float)width/(float)height;

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
	return View::Instance();
}