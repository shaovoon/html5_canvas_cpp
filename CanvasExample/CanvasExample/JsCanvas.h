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
		void strokeText(const char* text, double x, double y)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.strokeText( UTF8ToString($1), $2, $3);
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
		void clip()
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.clip();
				}, m_Name.c_str());
		}
		void arc(double xc, double yc,
			double radius,
			double angle1, double angle2)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.arc($1, $2, $3, $4, $5);
				}, m_Name.c_str(), xc, yc, radius, angle1, angle2);
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

		void scale(double sx, double sy)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.scale($1, $2);
				}, m_Name.c_str(), sx, sy);
		}
		void translate(double tx, double ty)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.translate($1, $2);
				}, m_Name.c_str(), tx, ty);
		}
		void rotate(double angle)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.rotate($1);
				}, m_Name.c_str(), angle);
		}

		void transform(double xx, double xy, double yx, double yy, double x0, double y0)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.transform($1, $2, $3, $4, $5, $6);
				}, m_Name.c_str(), xx, xy, yx, yy, x0, y0);
		}

		void setTransform(double xx, double xy, double yx, double yy, double x0, double y0)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.setTransform($1, $2, $3, $4, $5, $6);
				}, m_Name.c_str(), xx, xy, yx, yy, x0, y0);
		}

		void set_miterLimit(double limit)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.miterLimit = limit;
				}, m_Name.c_str(), limit);
		}

		void drawImage(const char* image, double x, double y)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

                var img = document.getElementById(UTF8ToString($1));
                ctx.drawImage(img, $2, $3);
            }, m_Name.c_str(), image, x, y);
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