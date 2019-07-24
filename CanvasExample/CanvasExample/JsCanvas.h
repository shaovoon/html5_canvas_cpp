#pragma once
#include <string>
#include <emscripten.h>

namespace canvas
{

	enum class LineCap
	{
		butt,
		round,
		square
	};
	enum class LineJoin
	{
		miter,
		round,
		bevel
	};

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
		void clearRect(double x, double y, double width, double height)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.clearRect($1, $2, $3, $4);
				}, m_Name.c_str(), x, y, width, height);
		}
		void strokeRect(double x, double y, double width, double height)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.strokeRect($1, $2, $3, $4);
				}, m_Name.c_str(), x, y, width, height);
		}

		void set_fillStyle(const char* value)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.fillStyle = UTF8ToString($1);
				}, m_Name.c_str(), value);
		}
		void set_strokeStyle(const char* value)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.strokeStyle = UTF8ToString($1);
				}, m_Name.c_str(), value);
		}
		void set_font(const char* value)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.font = UTF8ToString($1);
				}, m_Name.c_str(), value);
		}
		void fillText(const char* text, double x, double y)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.fillText( UTF8ToString($1), $2, $3);
				}, m_Name.c_str(), text, x, y);
		}
		void moveTo(double x, double y)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.moveTo($1, $2);
				}, m_Name.c_str(), x, y);
		}
		void lineTo(double x, double y)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.lineTo($1, $2);
				}, m_Name.c_str(), x, y);
		}
		void beginPath()
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.beginPath();
				}, m_Name.c_str());
		}
		void closePath()
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.closePath();
				}, m_Name.c_str());
		}
		
		void stroke()
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.stroke();
				}, m_Name.c_str());
		}
		void fill()
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.fill();
				}, m_Name.c_str());
		}

		void set_lineCap(LineCap cap)
		{
			const char* cairo_cap = "butt";
			if (cap == LineCap::butt)
				cairo_cap = "butt";
			else if (cap == LineCap::round)
				cairo_cap = "round";
			else if (cap == LineCap::square)
				cairo_cap = "square";

			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.lineCap = UTF8ToString($1);
				}, m_Name.c_str(), cairo_cap);
		}

		void set_lineJoin(LineJoin cap)
		{
			const char* cairo_join = "miter";
			if (cap == LineJoin::miter)
				cairo_join = "miter";
			else if (cap == LineJoin::round)
				cairo_join = "round";
			else if (cap == LineJoin::bevel)
				cairo_join = "bevel";

			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.lineCap = UTF8ToString($1);
				}, m_Name.c_str(), cairo_join);
		}

		void set_lineWidth(double width)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.lineWidth = $1;
				}, m_Name.c_str(), width);
		}

		bool savePng(const char* file)
		{
			// do nothing here.
			return true;
		}

	private:
		std::string m_Name;
	};
}