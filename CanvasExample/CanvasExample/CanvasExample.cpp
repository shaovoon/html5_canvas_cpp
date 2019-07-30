// Copyright 2019 Shao Voon Wong
// No warranties expressed or implied
// use it at your risk!

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

	ctx.font = "20px Georgia";
	ctx.fillText("Hello World!", 10, 50);

	ctx.font = "30px Verdana";

	// Create gradient
	auto gradient = ctx.createLinearGradient("gradient", 0, 0, 320, 0);
	gradient.addColorStop(0.0, "magenta");
	gradient.addColorStop(0.5, "blue");
	gradient.addColorStop(1.0, "red");
	// Fill with gradient
	ctx.fillStyle = gradient;
	ctx.fillText("Big smile!", 10, 90);

	ctx.savePng("c:\\temp\\displayText.png");
}

// Display text outline with strokeText()
void displayTextOutline()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.font = "20px Georgia";
	ctx.lineWidth = 1.0;
	ctx.strokeText("Hello World!", 10, 50);

	ctx.font = "30px Verdana";

	// Create gradient
	auto gradient = ctx.createLinearGradient("gradient", 0, 0, 320, 0);
	gradient.addColorStop(0.0, "magenta");
	gradient.addColorStop(0.5, "blue");
	gradient.addColorStop(1.0, "red");

	// Fill with gradient
	ctx.strokeStyle = gradient;
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
	ctx.lineWidth = 10.0;
	ctx.lineCap = LineCap::round;

	LineCap cap = ctx.lineCap; // testing getter
	if (cap == LineCap::round)
		std::cout << "cap == LineCap::round\n";
	else
		std::cout << "Error:cap != LineCap::round\n";

	ctx.moveTo(20, 20);
	ctx.lineTo(200, 20);
	ctx.stroke();

	ctx.savePng("c:\\temp\\drawLine.png");
}

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

void drawQuadraticCurve()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.beginPath();
	ctx.moveTo(20, 20);
	ctx.quadraticCurveTo(20, 100, 200, 20);
	ctx.stroke();

	ctx.savePng("c:\\temp\\drawQuadraticCurve.png");
}

void clearRect()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	auto grad = ctx.createLinearGradient("grad", 0.0, 0.0, 320.0, 0.0);
	grad.addColorStop(0.0, "red");
	grad.addColorStop(1.0, "blue");
	ctx.fillStyle = grad;
	ctx.fillRect(0.0, 0.0, 320.0, 280.0);

	ctx.clearRect(100, 100, 50, 50);
	ctx.savePng("c:\\temp\\clearRect.png");
}

void rotateRect()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	double PI = 3.14159265359;
	ctx.rotate(20 * PI / 180);
	ctx.fillRect(50, 20, 100, 50);

	ctx.savePng("c:\\temp\\rotateRect.png");
}

void saveRestore()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);
	ctx.save();

	double PI = 3.14159265359;
	ctx.rotate(20 * PI / 180);
	ctx.fillRect(50, 20, 100, 50);

	ctx.restore();
	ctx.fillRect(50, 20, 100, 50);

	ctx.savePng("c:\\temp\\saveRestore.png");
}

void repeatPattern()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);
#ifdef __EMSCRIPTEN__
	auto pat = ctx.createPattern("pat", "lamp", RepeatPattern::repeat);
#else
	auto pat = ctx.createPattern("pat", "D:\\GitHub\\html5_canvas_cpp\\CanvasExample\\WebApplication1\\img_lamp.jpg", RepeatPattern::repeat);
#endif
	ctx.rect(0, 0, 150, 100);
	ctx.fillStyle = pat;
	ctx.fill();

	ctx.savePng("c:\\temp\\repeatPattern.png");
}

void compositeOp()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.fillStyle = "red";
	ctx.fillRect(20, 20, 75, 50);
	ctx.globalCompositeOperation = "source-over";
	ctx.fillStyle = "blue";
	ctx.fillRect(50, 50, 75, 50);

	ctx.fillStyle = "red";
	ctx.fillRect(150, 20, 75, 50);
	ctx.globalCompositeOperation = "destination-over";
	ctx.fillStyle = "blue";
	ctx.fillRect(180, 50, 75, 50);

	ctx.savePng("c:\\temp\\compositeOp.png");
}

void pointInPath()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.rect(20, 20, 150, 100);
	if (ctx.isPointInPath(20, 50)) {
		ctx.stroke();
	};

	ctx.savePng("c:\\temp\\pointInPath.png");
}

void shadowFillRect()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.shadowOffsetX = 10;
	ctx.shadowOffsetY = 10;
	//ctx.shadowColor = 0x80000000;
	//ctx.shadowColor = "#80000000";
	ctx.shadowColor = "rgba(0,0,0,0.5)";
	ctx.fillStyle = "red";
	ctx.fillRect(20, 20, 100, 80);

	ctx.savePng("c:\\temp\\shadowFillRect.png");
}

void shadowFillText()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	ctx.shadowBlur = 10;
	ctx.shadowOffsetX = 1;
	ctx.shadowOffsetY = 2;
	//ctx.shadowColor = 0x60000000;
	ctx.shadowColor = "rgba(0,0,0,0.5)";
	ctx.font = "30px Georgia";
	ctx.fillText("Hello World!", 10, 50);

	ctx.savePng("c:\\temp\\shadowFillText.png");
}

void shadowFillArc()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

#ifdef __EMSCRIPTEN__
	ctx.drawImage("yes_image", 10.0, 10.0);
#else
	ctx.drawImage("C:\\Users\\shaov\\Pictures\\yes.jpg", 10.0, 10.0);
#endif

	ctx.shadowOffsetX = 5;
	ctx.shadowOffsetY = 5;
	//ctx.shadowColor = 0x60000000;
	ctx.shadowColor = "rgba(0,0,0,0.5)";

	double PI = 3.14159265359;

	ctx.arc(100, 75, 50, 0, 2 * PI);
	ctx.fillStyle = "#FF0000";
	ctx.fill();
	
	ctx.savePng("c:\\temp\\shadowFillArc.png");
}

void shadowStrokeArc()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

#ifdef __EMSCRIPTEN__
	ctx.drawImage("yes_image", 10.0, 10.0);
#else
	ctx.drawImage("C:\\Users\\shaov\\Pictures\\yes.jpg", 10.0, 10.0);
#endif

	ctx.shadowOffsetX = 5;
	ctx.shadowOffsetY = 5;
	//ctx.shadowColor = 0x60000000;
	ctx.shadowColor = "rgba(0,0,0,0.5)";

	double PI = 3.14159265359;

	ctx.arc(100, 75, 50, 0, 2 * PI);
	ctx.strokeStyle = "#FF0000";
	ctx.stroke();

	ctx.savePng("c:\\temp\\shadowStrokeArc.png");
}

void radialGradient()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);

	auto grd = ctx.createRadialGradient("grd", 75, 50, 5, 90, 60, 100);
	grd.addColorStop(0, "red");
	grd.addColorStop(1, "white");

	// Fill with gradient
	ctx.fillStyle = grd;
	ctx.fillRect(10, 10, 150, 100);

	ctx.savePng("c:\\temp\\radialGradient.png");
}

void shadowFillBlur()
{
	using namespace canvas;

	Canvas ctx("canvas", 320, 280);
	/*
#ifdef __EMSCRIPTEN__
	ctx.drawImage("yes_image", 10.0, 10.0);
#else
	ctx.drawImage("C:\\Users\\shaov\\Pictures\\yes.jpg", 10.0, 10.0);
#endif
	*/
	ctx.shadowBlur = 10;
	ctx.shadowOffsetX = 10;
	ctx.shadowOffsetY = 10;
	//ctx.shadowColor = 0x80000000;
	//ctx.shadowColor = "#80000000";
	ctx.shadowColor = "rgba(0,0,0,0.5)";
	ctx.fillStyle = "red";
	ctx.rect(20, 20, 100, 80);
	ctx.fill();

	ctx.savePng("c:\\temp\\shadowFillBlur.png");
}

int main()
{
	//displayText();
	//displayTextOutline();
	//displayImage();
	//drawLine();
	//drawBezier();
	//drawQuadraticCurve();
	//clearRect();
	//rotateRect();
	//saveRestore();
	//repeatPattern();
	//compositeOp();
	//pointInPath();
	//shadowFillRect();
	shadowFillText();
	//shadowFillArc();
	//shadowStrokeArc();
	//radialGradient();
	//shadowFillBlur();

	std::cout << "Done!\n";
}