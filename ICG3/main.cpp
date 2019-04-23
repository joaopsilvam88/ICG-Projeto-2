#include <GL/glut.h>
#include <iostream>
#include "pipeline.h"
#include "main.h"



void MyGlDraw(void) {

    limparFB();
    InitObj();
}

int main(int argc, char **argv)
{


    ObjData();

    InitOpenGL(&argc, argv);
    InitCallBacks();
    InitDataStructures();

    DrawFunc = MyGlDraw;

    glutMainLoop();

	return 0;
}
