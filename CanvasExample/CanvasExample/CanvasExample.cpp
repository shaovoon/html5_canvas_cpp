// CanvasExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include "JsCanvas.h"
#else
	#include "CppCanvas.h"
#endif


int main()
{
    std::cout << "Hello World!\n";
}

