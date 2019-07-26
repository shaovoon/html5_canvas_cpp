// CanvasExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#ifdef __EMSCRIPTEN__
	#include "JsCanvas.h"
#else
	#include "CppCanvas.h"
#endif


// Display text with fillText()
void displayText()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.set_font("20px Georgia");
	ctx.fillText("Hello World!", 10, 50);

	ctx.set_font("30px Verdana");

	// Create gradient
	auto gradient = ctx.createLinearGradient("gradient", 0, 0, 320, 0);
	gradient.addColorStop(0.0, "#ff00ff");
	gradient.addColorStop(0.5, "#0000ff");
	gradient.addColorStop(1.0, "#ff0000");
	// Fill with gradient
	ctx.set_fillStyle(gradient);
	ctx.fillText("Big smile!", 10, 90);

	ctx.savePng("c:\\temp\\displayText.png");
}

// Display text outline with strokeText()
void displayTextOutline()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.set_font("20px Georgia");
	ctx.set_lineWidth(1.0);
	ctx.strokeText("Hello World!", 10, 50);

	ctx.set_font("30px Verdana");

	// Create gradient
	auto gradient = ctx.createLinearGradient("gradient", 0, 0, 320, 0);
	//gradient.addColorStop(0.0, "#ff00ff");
	//gradient.addColorStop(0.5, "#0000ff");
	//gradient.addColorStop(1.0, "#ff0000");

	gradient.addColorStop(0.0, fromRGB(0xff, 0, 0xff));
	gradient.addColorStop(0.5, fromRGB(0, 0, 0xff));
	gradient.addColorStop(1.0, fromRGB(0xff, 0, 0));

	// Fill with gradient
	ctx.set_strokeStyle(gradient);
	ctx.strokeText("Big smile!", 10, 90);

	ctx.savePng("c:\\temp\\displayTextOutline.png");
}

// Display Image
void displayImage()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

#ifdef __EMSCRIPTEN__
	ctx.drawImage("yes_image", 10.0, 10.0);
#else
	ctx.drawImage("C:\\Users\\shaov\\Pictures\\yes.jpg", 10.0, 10.0);
#endif

	ctx.savePng("c:\\temp\\displayImage.png");
}

// Draw line with a round cap
void drawLine()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.beginPath();
	ctx.set_lineWidth(10.0);
	ctx.set_lineCap(LineCap::round);
	ctx.moveTo(20, 20);
	ctx.lineTo(200, 20);
	ctx.stroke();

	ctx.savePng("c:\\temp\\drawLine.png");
}

// Draw line with a round cap
void drawBezier()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.beginPath();
	ctx.moveTo(20, 20);
	ctx.bezierCurveTo(20, 100, 200, 100, 200, 20);
	ctx.stroke();

	ctx.savePng("c:\\temp\\drawBezier.png");
}

int main()
{
	displayText();
	//displayTextOutline();
	//displayImage();
	//drawLine();
	//drawBezier();

	std::cout << "Done!\n";
}