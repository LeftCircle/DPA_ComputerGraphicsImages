#ifndef VIEW_H
#define VIEW_H

#include <GL/glut.h>
#include "controller.h"

class View
{
  public:

    static View* instance()
    {
       if(pView==nullptr)
       {
          pView = new View();
       }
       return pView;
    }

    ~View();

    void init( int argc, char** argv, int _width, int _height);
    void display();
    void reshape( int w, int h );
    void idle();
    void main_loop();
    //void set_pixels( float* p ) { pixels = p; }
    void set_image_proc(ImageProc* img_proc) { _image_proc = img_proc; } 

private:
    
    static View* pView;
    ImageProc* _image_proc;

    View();
    View( const View& );
    View& operator= (const View&);
    
    //float* pixels;
    int width, height;
};

View* create_view();

#endif