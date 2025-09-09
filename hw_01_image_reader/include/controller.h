#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <iostream>
#include <GL/glut.h>

// A singleton controller class that will be used to drive GLUT inputs
class Controller
{
  public:

    static Controller* instance()
    {
       if(pController==nullptr)
       {
          pController = new Controller();
       }
       return pController;
    }

    ~Controller();

    void init();

    void keyboard( unsigned char key, int x, int y );

  private:

    static Controller* pController;

    Controller();
    Controller( const Controller& );
    Controller& operator= (const Controller&);

};


#endif