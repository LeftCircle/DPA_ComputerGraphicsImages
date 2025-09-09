#ifndef VIEW_H
#define VIEW_H

#include <GL/glut.h>

class View
{
  public:

    static View* Instance()
    {
       if(pView==nullptr)
       {
          pView = new View();
       }
       return pView;
    }

    ~View();

    void init( int argc, char** argv );
    void display();
    void reshape( int w, int h );
    void idle();
    void main_loop();
    
  private:

    static View* pView;

    View();
    View( const View& );
    View& operator= (const View&);

    int width, height;
    float camera_aspect;
};


#endif