#include "view.h"


View* View::pView = nullptr;


void View::init( int argc, char** argv )
{
	width = 1024;
	height = 768;
	camera_aspect = (float)width/(float)height;

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( width, height );
	glutCreateWindow( "View" );

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glEnable( GL_DEPTH_TEST );

	glutDisplayFunc( [](void){ View::Instance() -> display(); } );
	glutReshapeFunc( [](int w, int h){ View::Instance() -> reshape(w,h); } );
	glutIdleFunc( [](){ View::Instance() -> idle(); } );
}

void View::display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();


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