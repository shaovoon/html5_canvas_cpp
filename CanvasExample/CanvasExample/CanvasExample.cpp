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
	using namespace canvas;

	Canvas ctx("canvas", 640, 480);
	ctx.set_fillStyle("#ffffaa");
	ctx.fillRect(0.0, 0.0, 500.0, 300.0);

	ctx.set_fillStyle("#000000");
	ctx.set_font("20px Arial");
	ctx.fillText("Did you see this?", 10.0, 50.0);

	ctx.beginPath();
	ctx.set_lineWidth(10.0);
	ctx.set_lineCap(LineCap::round);
	ctx.moveTo(20, 20);
	ctx.lineTo(200, 20);
	ctx.stroke();

	ctx.savePng("c:\\temp\\image.png");


    std::cout << "Done!\n";
}

