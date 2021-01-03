
#include <mcheck.h>
#include "triangle.hpp"
#include "triangle_impl.hpp"
#include "glut_utils.h"
using namespace ilrd;

static void DrawFunction();

int main(int argc, char** argv)
{
    DrawInit(argc, argv, 1000, 1000,DrawFunction);


    /* advanced: extra functionality 
    DrawSetKeyboardFunc(KeyboardFunction);
    DrawSetMouseFunc(MouseFunction);
    DrawSetMotionFunc(MotionFunction);
    DrawSetTimerFunc(TimerFunction, 100);
*/
    DrawMainLoop();

    return 0;

}


static void DrawFunction()
{
    /* printf("Drawing\n"); */

    /* draw triangle */
   Triangle tr(0 , 100, 100, 15, 0, 0);

   tr.Draw(COLOR_BLUE);
}