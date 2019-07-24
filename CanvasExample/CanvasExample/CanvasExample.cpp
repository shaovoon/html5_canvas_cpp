// CanvasExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#ifdef __EMSCRIPTEN__
	#include "JsCanvas.h"
#else
	#include "CppCanvas.h"
#endif


int main()
{
	Canvas ctx("canvas", 640, 480);
	ctx.set_fillStyle("#ffffaa");
	ctx.fillRect(0.0, 0.0, 500.0, 300.0);
	ctx.savePng("c:\\temp\\image.png");
    std::cout << "Done!\n";
}

