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
/*
	Canvas ctx("canvas", 640, 480);
	ctx.set_fillStyle("#ffffaa");
	ctx.fillRect(0.0, 0.0, 500.0, 300.0);

	ctx.set_fillStyle("#000000");
	ctx.set_font("36px Arial");
	//ctx.fillText("Did you see this?", 10.0, 50.0);

	ctx.beginPath();
	ctx.set_lineWidth(10.0);
	ctx.set_lineCap(LineCap::round);
	ctx.moveTo(20, 20);
	ctx.lineTo(200, 20);
	ctx.stroke();

#ifdef __EMSCRIPTEN__
	ctx.drawImage("yes_image", 10.0, 10.0);
#else
	ctx.drawImage("C:\\Users\\shaov\\Pictures\\yes.png", 10.0, 10.0);
#endif
	ctx.set_lineWidth(2.0);
	ctx.set_strokeStyle("#000000");
	ctx.strokeText("Did you see this?", 50.0, 50.0);
*/
	Canvas ctx("canvas", 640, 480);
	auto grad = ctx.createLinearGradient("grad", 0.0, 0.0, 640.0, 0.0);
	//auto grad = ctx.createRadialGradient("grad", 75, 50, 5, 90, 60, 100);
	grad.addColorStop(0.0, "#ff0000");
	grad.addColorStop(1.0, "#0000ff");
	ctx.set_fillStyle(grad);
	ctx.fillRect(0.0, 0.0, 640.0, 480.0);

	//ImageData imgData = ctx.createImageData("imgData", 100, 100);
	ImageData imgData = ctx.getImageData("imgData", 10, 10, 100, 100);
	/*
	for (int y = 0; y < imgData.height(); ++y)
	{
		for (int x = 0; x < imgData.width(); ++x)
		{
			int index = (y * imgData.width() + x) * 4;
			imgData.data()[index] = 0xff;
			imgData.data()[index+1] = 0xff;
			imgData.data()[index+2] = 0xff;
			imgData.data()[index+3] = 0xff;
		}

	}
	*/
	ctx.putImageData(imgData, 0, 0, 200, 200);

	ctx.savePng("c:\\temp\\image.png");


    std::cout << "Done!\n";
}

