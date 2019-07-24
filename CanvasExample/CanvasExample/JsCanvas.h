#pragma once
#include <string>
#include <emscripten.h>

class Canvas
{
public: 
	Canvas(const char* name, int width, int height) : m_Name(name)
	{
		EM_ASM_({
			add_canvas(UTF8ToString($0))
			}, m_Name.c_str());
	}

	void fillRect(double x, double y, double width, double height)
	{
		EM_ASM_({
			var ctx = get_canvas(UTF8ToString($0));

			ctx.fillRect($1, $2, $3, $4);
			}, m_Name.c_str(), x, y, width, height);
	}
	void set_fillStyle(const char* value)
	{
		EM_ASM_({
			var ctx = get_canvas(UTF8ToString($0));

			ctx.fillStyle = UTF8ToString($1);
			}, m_Name.c_str(), value);
	}
	bool savePng(const char* file)
	{
		// do nothing here.
		return true;
	}

private:
	std::string m_Name;
};