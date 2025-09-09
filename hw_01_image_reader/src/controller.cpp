#include "controller.h"


Controller* Controller::pController = nullptr;

void Controller::keyboard( unsigned char key, int x, int y )
{
   switch (key)
   {
      case 27: // esc
         exit(0);
         break;
      case 'j':
      case 'J':
         std::cout << "J key pressed\n";
         break;
   }
}