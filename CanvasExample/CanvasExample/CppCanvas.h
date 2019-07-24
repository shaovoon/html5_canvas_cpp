#pragma once
#include <cairo.h>

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
		Canvas(const char* name, int width, int height) : surface(nullptr), cr(nullptr)
		{
			surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
			cr = cairo_create(surface);
		}
		~Canvas()
		{
			cairo_destroy(cr);
			cairo_surface_destroy(surface);
			cr = nullptr;
			surface = nullptr;
		}
		void fillRect(double x, double y, double width, double height)
		{
			cairo_rectangle(cr, x, y, width, height);
			cairo_stroke_preserve(cr);
			cairo_fill(cr);
		}
		void clearRect(double x, double y, double width, double height) // TODO: to be implemented
		{
			cairo_rectangle(cr, x, y, width, height);
			cairo_stroke_preserve(cr);
			cairo_fill(cr);
		}
		void strokeRect(double x, double y, double width, double height)
		{
			cairo_rectangle(cr, x, y, width, height);
			cairo_stroke_preserve(cr);
			cairo_stroke(cr);
		}
		void set_fillStyle(const char* value)
		{
			unsigned long v = strtoul(value + 1, nullptr, 16);
			double r = ((v & 0xff0000) >> 16) / 255.0;
			double g = ((v & 0xff00) >> 8) / 255.0;
			double b = (v & 0xff) / 255.0;
			cairo_set_source_rgb(cr, r, g, b);
		}
		void set_strokeStyle(const char* value)
		{
			unsigned long v = strtoul(value + 1, nullptr, 16);
			double r = ((v & 0xff0000) >> 16) / 255.0;
			double g = ((v & 0xff00) >> 8) / 255.0;
			double b = (v & 0xff) / 255.0;
			cairo_set_source_rgb(cr, r, g, b);
		}
		void set_font(const char* value)
		{
			std::string v = value;
			size_t font_pos = v.find_first_of(' ');
			if (font_pos != std::string::npos)
			{
				std::string font_str = v.substr(font_pos + 1);

				cairo_select_font_face(cr, font_str.c_str(),
					CAIRO_FONT_SLANT_NORMAL,
					CAIRO_FONT_WEIGHT_NORMAL);
			}
			else
			{
				cairo_select_font_face(cr, value,
					CAIRO_FONT_SLANT_NORMAL,
					CAIRO_FONT_WEIGHT_NORMAL);
			}

			size_t font_size_pos = v.find_first_of("px");
			if (font_size_pos != std::string::npos)
			{
				std::string font_size_str = v.substr(0, font_size_pos);
				unsigned long font_size = strtoul(font_size_str.c_str(), nullptr, 10);
				cairo_set_font_size(cr, (double)font_size);
			}
		}
		void fillText(const char* text, double x, double y)
		{
			cairo_move_to(cr, x, y);
			cairo_show_text(cr, text);
		}
		void moveTo(double x, double y)
		{
			cairo_move_to(cr, x, y);
		}
		void lineTo(double x, double y)
		{
			cairo_line_to(cr, x, y);
		}
		void beginPath()
		{
			cairo_new_path(cr);
		}
		void closePath()
		{
			cairo_close_path(cr);
		}
		void stroke()
		{
			cairo_stroke(cr);
		}
		void fill()
		{
			cairo_fill(cr);
		}
		void set_lineCap(LineCap cap)
		{
			cairo_line_cap_t cairo_cap = CAIRO_LINE_CAP_BUTT;
			if (cap == LineCap::butt)
				cairo_cap = CAIRO_LINE_CAP_BUTT;
			else if (cap == LineCap::round)
				cairo_cap = CAIRO_LINE_CAP_ROUND;
			else if (cap == LineCap::square)
				cairo_cap = CAIRO_LINE_CAP_SQUARE;

			cairo_set_line_cap(cr, cairo_cap);
		}

		void set_lineJoin(LineJoin cap)
		{
			cairo_line_join_t cairo_join = CAIRO_LINE_JOIN_MITER;
			if (cap == LineJoin::miter)
				cairo_join = CAIRO_LINE_JOIN_MITER;
			else if (cap == LineJoin::round)
				cairo_join = CAIRO_LINE_JOIN_ROUND;
			else if (cap == LineJoin::bevel)
				cairo_join = CAIRO_LINE_JOIN_BEVEL;

			cairo_set_line_join(cr, cairo_join);
		}

		void set_lineWidth(double width)
		{
			cairo_set_line_width(cr, width);
		}

		bool savePng(const char* file)
		{
			cairo_status_t status = cairo_surface_write_to_png(surface, file);
			return (status == CAIRO_STATUS_SUCCESS);
		}

	private:
		cairo_surface_t* surface;
		cairo_t* cr;

	};

}