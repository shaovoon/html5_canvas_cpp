#pragma once
#include <cairo.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

	unsigned int fromRGBA(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
	{
		return (unsigned int)((a << 24) | (r << 16) | (g << 8) | b);
	}

	unsigned int fromRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return fromRGBA(0xff, r, g, b);
	}

	unsigned int fromRGBA(double a, double r, double g, double b)
	{
		unsigned int ia = a * 255.0;
		unsigned int ir = r * 255.0;
		unsigned int ig = g * 255.0;
		unsigned int ib = b * 255.0;
		ia &= 0xff;
		ir &= 0xff;
		ig &= 0xff;
		ib &= 0xff;
		return (unsigned int)((ia << 24) | (ir << 16) | (ig << 8) | ib);
	}

	unsigned int fromRGB(double a, double r, double g, double b)
	{
		return fromRGBA(1.0, r, g, b);
	}
	// https://cairographics.org/manual/cairo-Image-Surfaces.html
	class ImageData
	{
	public:
		ImageData(unsigned char* data, int width, int height) 
			: m_Data(data), m_Width(width), m_Height(height) {}
		ImageData(ImageData&& other)
		{
			m_Data = other.m_Data;
			m_Width = other.m_Width;
			m_Height = other.m_Height;

			other.m_Data = nullptr;
		}
		~ImageData()
		{
			if (m_Data)
			{
				delete[] m_Data;
				m_Data = nullptr;
			}
		}
		unsigned char* data() const
		{
			return m_Data;
		}
		int width() const
		{
			return m_Width;
		}
		int height() const
		{
			return m_Height;
		}
	private:
		unsigned char* m_Data;
		int m_Width;
		int m_Height;
	};
	class Gradient
	{
	public:
		Gradient(cairo_pattern_t* pattern) : m_Pattern(pattern) {}
		Gradient(Gradient&& other)
		{
			m_Pattern = other.m_Pattern;
			other.m_Pattern = nullptr;
		}

		~Gradient()
		{
			if (m_Pattern)
			{
				cairo_pattern_destroy(m_Pattern);
				m_Pattern = nullptr;
			}
		}

		void addColorStop(double stop, const char* color)
		{
			if (strlen(color) > 7)
			{
				unsigned long v = strtoul(color + 1, nullptr, 16);
				double a = ((v & 0xff000000) >> 24) / 255.0;
				double r = ((v & 0xff0000) >> 16) / 255.0;
				double g = ((v & 0xff00) >> 8) / 255.0;
				double b = (v & 0xff) / 255.0;
				cairo_pattern_add_color_stop_rgba(m_Pattern, stop, r, g, b, a);
			}
			else
			{
				unsigned long v = strtoul(color + 1, nullptr, 16);
				double r = ((v & 0xff0000) >> 16) / 255.0;
				double g = ((v & 0xff00) >> 8) / 255.0;
				double b = (v & 0xff) / 255.0;
				cairo_pattern_add_color_stop_rgb(m_Pattern, stop, r, g, b);
			}
		}
		cairo_pattern_t* getPattern() const
		{
			return m_Pattern;
		}
	private:
		cairo_pattern_t* m_Pattern;
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
			destroy();
		}

		void destroy()
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
			set_strokeStyle(value);
		}

		void set_fillStyle(unsigned int value)
		{
			set_strokeStyle(value);
		}

		void set_fillStyle(const Gradient& grad)
		{
			set_strokeStyle(grad);
		}

		void set_strokeStyle(const Gradient& grad)
		{
			cairo_set_source(cr, grad.getPattern());
		}

		void set_strokeStyle(const char* value)
		{
			if (strlen(value) > 7)
			{
				unsigned long v = strtoul(value + 1, nullptr, 16);
				double a = ((v & 0xff000000) >> 24) / 255.0;
				double r = ((v & 0xff0000) >> 16) / 255.0;
				double g = ((v & 0xff00) >> 8) / 255.0;
				double b = (v & 0xff) / 255.0;
				cairo_set_source_rgba(cr, r, g, b, a);
			}
			else
			{
				unsigned long v = strtoul(value + 1, nullptr, 16);
				double r = ((v & 0xff0000) >> 16) / 255.0;
				double g = ((v & 0xff00) >> 8) / 255.0;
				double b = (v & 0xff) / 255.0;
				cairo_set_source_rgb(cr, r, g, b);
			}
		}

		void set_strokeStyle(unsigned int value)
		{
			double a = ((value & 0xff000000) >> 24) / 255.0;
			double r = ((value & 0xff0000) >> 16) / 255.0;
			double g = ((value & 0xff00) >> 8) / 255.0;
			double b = (value & 0xff) / 255.0;
			cairo_set_source_rgba(cr, r, g, b, a);
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

		void strokeText(const char* text, double x, double y)
		{
			cairo_move_to(cr, x, y);
			cairo_text_path(cr, text);
			cairo_stroke(cr);
		}

		void beginPath()
		{
			cairo_new_path(cr);
		}

		void closePath()
		{
			cairo_close_path(cr);
		}

		void moveTo(double x, double y)
		{
			cairo_move_to(cr, x, y);
		}

		void lineTo(double x, double y)
		{
			cairo_line_to(cr, x, y);
		}

		// https://www.geeksforgeeks.org/cubic-bezier-curve-implementation-in-c/
		// https://pomax.github.io/bezierinfo/
		void bezierCurveTo(double cp1x, double cp1y, double cp2x, double cp2y, double endx, double endy)
		{
			double x[4];
			double y[4];
			cairo_get_current_point(cr, &x[0], &y[0]);
			x[1] = cp1x;
			x[2] = cp2x;
			x[3] = endx;
			y[1] = cp1y;
			y[2] = cp2y;
			y[3] = endy;
			double xu = 0.0, yu = 0.0, u = 0.0;
			int i = 0;
			for (u = 0.0; u <= 1.0; u += 0.0001)
			{
				xu = pow(1 - u, 3) * x[0] + 3 * u * pow(1 - u, 2) * x[1] + 3 * pow(u, 2) * (1 - u) * x[2]
					+ pow(u, 3) * x[3];
				yu = pow(1 - u, 3) * y[0] + 3 * u * pow(1 - u, 2) * y[1] + 3 * pow(u, 2) * (1 - u) * y[2]
					+ pow(u, 3) * y[3];

				cairo_line_to(cr, xu, yu);
			}
		}

		void quadraticCurveTo(double cpx, double cpy, double endx, double endy)
		{
			double x[3];
			double y[3];
			cairo_get_current_point(cr, &x[0], &y[0]);
			x[1] = cpx;
			x[2] = endx;
			y[1] = cpy;
			y[2] = endy;
			double xu = 0.0, yu = 0.0, u = 0.0;
			int i = 0;
			for (u = 0.0; u <= 1.0; u += 0.0001)
			{
				xu = pow(1 - u, 2) * x[0] + 2 * (1 - u) * u * x[1] + (u * u) * x[2];
				yu = pow(1 - u, 2) * y[0] + 2 * (1 - u) * u * y[1] + (u * u) * y[2];

				cairo_line_to(cr, xu, yu);
			}
		}

		void clip()
		{
			cairo_clip(cr);
		}

		void arc(double xc, double yc,
			double radius,
			double angle1, double angle2)
		{
			cairo_arc(cr, xc, yc, radius, angle1, angle2);
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

		void scale(double sx, double sy)
		{
			cairo_scale(cr, sx, sy);
		}

		void translate(double tx, double ty)
		{
			cairo_translate(cr, tx, ty);
		}

		void rotate(double angle)
		{
			cairo_rotate(cr, angle);
		}

		void transform(double xx, double xy, double yx, double yy, double x0, double y0)
		{
			_cairo_matrix mat = {};
			mat.xx = xx;
			mat.xy = xy;
			mat.yx = yx;
			mat.yy = yy;
			mat.x0 = x0;
			mat.y0 = y0;

			cairo_transform(cr, &mat);
		}

		void setTransform(double xx, double xy, double yx, double yy, double x0, double y0)
		{
			cairo_identity_matrix(cr);
			transform(xx, xy, yx, yy, x0, y0);
		}

		void set_miterLimit(double limit)
		{
			cairo_set_miter_limit(cr, limit);
		}

		// https://github.com/aleksaro/gloom/wiki/Loading-images-with-stb
		void drawImage(const char* image_file, double x0, double y0)
		{
			int width, height, channels;
			//stbi_set_flip_vertically_on_load(true);
			unsigned char* image = stbi_load(image_file,
				&width,
				&height,
				&channels,
				STBI_rgb);

			ImageData imgData = createImageData("imgData", width, height);
			unsigned char* dest_pixel = imgData.data();
			for (int y = 0; y < imgData.height(); ++y)
			{
				for (int x = 0; x < imgData.width(); ++x)
				{
					int index = (y * imgData.width() + x) * 4;
					int src_index = (y * imgData.width() + x) * 3;
					dest_pixel[index] = image[src_index + 2];
					dest_pixel[index + 1] = image[src_index + 1];
					dest_pixel[index + 2] = image[src_index + 0];
					dest_pixel[index + 3] = 0xff;
				}

			}

			putImageData(imgData, x0, y0, 0, 0, width, height);

			stbi_image_free((void*)image);
		}

		Gradient createLinearGradient(const char* name, double x0, double y0, double x1, double y1)
		{
			return std::move(Gradient(cairo_pattern_create_linear(x0, y0, x1, y1)));
		}

		Gradient createRadialGradient(const char* name, double x0, double y0, double r0, double x1, double y1, double r1)
		{
			return std::move(Gradient(cairo_pattern_create_radial(x0, y0, r0, x1, y1, r1)));
		}

		// https://cairographics.org/manual/cairo-Image-Surfaces.html
		ImageData createImageData(const char* name, int width, int height)
		{
			unsigned char* data = new unsigned char[width * height * 4];
			return std::move(ImageData(data, width, height));
		}

		void putImageData(ImageData& imgData, int x, int y, int srcX = 0, int srcY = 0, int srcWidth = 0, int srcHeight = 0)
		{
			cairo_surface_flush(surface);
			unsigned char* dest_pixel = cairo_image_surface_get_data(surface);
			int dest_width = cairo_image_surface_get_width(surface);
			int dest_height = cairo_image_surface_get_height(surface);
			if (srcWidth == 0)
				srcWidth = imgData.width();
			if (srcHeight == 0)
				srcHeight = imgData.height();

			unsigned char* src_pixel = imgData.data();
			for (int ty = srcY, dirtyY2 = y; ty < imgData.height() && ty < srcHeight && dirtyY2 < dest_height; ++ty, ++dirtyY2)
			{
				for (int tx = srcX, dirtyX2 = x; tx < imgData.width() && tx < srcWidth && dirtyX2 < dest_width; ++tx, ++dirtyX2)
				{
					int src_index = (ty * imgData.width() + tx) * 4;
					int dest_index = (dirtyY2 * dest_width + dirtyX2) * 4;
					dest_pixel[dest_index] = src_pixel[src_index];
					dest_pixel[dest_index + 1] = src_pixel[src_index + 1];
					dest_pixel[dest_index + 2] = src_pixel[src_index + 2];
					dest_pixel[dest_index + 3] = src_pixel[src_index + 3];
				}
			}

			cairo_surface_mark_dirty(surface);
		}

		ImageData getImageData(const char* name, int x, int y, int width, int height)
		{
			ImageData imgData = createImageData(name, width, height);
			unsigned char* src_pixel = cairo_image_surface_get_data(surface);
			int src_width = cairo_image_surface_get_width(surface);
			int src_height = cairo_image_surface_get_height(surface);

			unsigned char* dest_pixel = imgData.data();
			for (int ty = y, dy = 0; ty < src_height && dy < height; ++ty, ++dy)
			{
				for (int tx = x, dx = 0; tx < src_width && dx < width; ++tx, ++dx)
				{
					int src_index = (ty * src_width + tx) * 4;
					int dest_index = (dy * width + dx) * 4;
					dest_pixel[dest_index] = src_pixel[src_index];
					dest_pixel[dest_index + 1] = src_pixel[src_index + 1];
					dest_pixel[dest_index + 2] = src_pixel[src_index + 2];
					dest_pixel[dest_index + 3] = src_pixel[src_index + 3];
				}
			}
			return std::move(imgData);
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