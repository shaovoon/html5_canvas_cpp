#pragma once
#include <cairo.h>

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
	void set_fillStyle(const char* value)
	{
		unsigned long v = strtoul(value + 1, nullptr, 16);
		double r = ((v & 0xff0000) >> 16) / 255.0;
		double g = ((v & 0xff00) >> 8) / 255.0;
		double b = (v & 0xff) / 255.0;
		cairo_set_source_rgb(cr, r, g, b);
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