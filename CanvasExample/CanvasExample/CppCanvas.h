// Copyright 2019 Shao Voon Wong
// No warranties expressed or implied
// use it at your risk!

// Release log
// v0.1.0: first release
// v0.2.0: Setters converted to properties
// v0.3.0: Add getter of basic types

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

	unsigned int fromRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return (unsigned int)((r << 16) | (g << 8) | b);
	}



	// https://cairographics.org/manual/cairo-Image-Surfaces.html
	class ImageData
	{
	public:
		ImageData(unsigned char* data, int width, int height) 
			: m_Data(data), m_Width(width), m_Height(height) {}
		ImageData(ImageData&& other) noexcept
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
		// remove copy constructor and assignment operator
		ImageData(const ImageData& other) = delete;
		void operator=(const ImageData& other) = delete;

		unsigned char* m_Data;
		int m_Width;
		int m_Height;
	};

	class Gradient
	{
	public:
		Gradient(cairo_pattern_t* pattern) : m_Pattern(pattern) {}
		Gradient(Gradient&& other) noexcept
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
			unsigned long v = strtoul(color + 1, nullptr, 16);
			double r = ((v & 0xff0000) >> 16) / 255.0;
			double g = ((v & 0xff00) >> 8) / 255.0;
			double b = (v & 0xff) / 255.0;
			cairo_pattern_add_color_stop_rgb(m_Pattern, stop, r, g, b);
		}
		void addColorStop(double stop, unsigned int color)
		{
			double r = ((color & 0xff0000) >> 16) / 255.0;
			double g = ((color & 0xff00) >> 8) / 255.0;
			double b = (color & 0xff) / 255.0;
			cairo_pattern_add_color_stop_rgb(m_Pattern, stop, r, g, b);
		}
		cairo_pattern_t* getPattern() const
		{
			return m_Pattern;
		}
	private:
		// remove copy constructor and assignment operator
		Gradient(const Gradient& other) = delete;
		void operator=(const Gradient& other) = delete;

		cairo_pattern_t* m_Pattern;
	};

	enum class RepeatPattern
	{
		repeat,
		no_repeat
	};

	class Pattern
	{
	public:
		Pattern(cairo_surface_t* surface, cairo_pattern_t* pattern, unsigned char* pixel) 
			: m_Surface(surface), m_Pattern(pattern), m_Pixel(pixel) {}
		Pattern(Pattern&& other) noexcept
		{
			m_Surface = other.m_Surface;
			other.m_Surface = nullptr;
			m_Pattern = other.m_Pattern;
			other.m_Pattern = nullptr;
			m_Pixel = other.m_Pixel;
			other.m_Pixel = nullptr;
		}

		~Pattern()
		{
			if (m_Pattern)
			{
				cairo_pattern_destroy(m_Pattern);
				m_Pattern = nullptr;
			}
			if (m_Surface)
			{
				cairo_surface_destroy(m_Surface);
				m_Surface = nullptr;
			}
			if (m_Pixel)
			{
				delete[] m_Pixel;
				m_Pixel = nullptr;
			}
		}

		cairo_pattern_t* getPattern() const
		{
			return m_Pattern;
		}
	private:
		// remove copy constructor and assignment operator
		Pattern(const Pattern& other) = delete;
		void operator=(const Pattern& other) = delete;

		cairo_surface_t* m_Surface;
		cairo_pattern_t* m_Pattern;
		unsigned char* m_Pixel;
	};

	enum class GlobalCompositeOperationType
	{
		source_over,
		source_atop,
		source_in,
		source_out,
		destination_over,
		destination_atop,
		destination_in,
		destination_out,
		lighter,
		copy,
		exclusive_or,
	};

	class GlobalCompositeOperationProperty
	{
	public:
		GlobalCompositeOperationProperty() 
			: cr(nullptr) {}

		void init(cairo_t* ptr)
		{
			cr = ptr;
		}

		void operator=(const char* op)
		{
			std::string s = op;
			cairo_operator_t cop = CAIRO_OPERATOR_IN;
			if (s == "source-over")
				cop = CAIRO_OPERATOR_OVER;
			else if (s == "source-atop")
				cop = CAIRO_OPERATOR_ATOP;
			else if (s == "source-in")
				cop = CAIRO_OPERATOR_IN;
			else if (s == "source-out")
				cop = CAIRO_OPERATOR_OUT;
			else if (s == "destination-over")
				cop = CAIRO_OPERATOR_DEST_OVER;
			else if (s == "destination-atop")
				cop = CAIRO_OPERATOR_DEST_ATOP;
			else if (s == "destination-in")
				cop = CAIRO_OPERATOR_DEST_IN;
			else if (s == "destination-out")
				cop = CAIRO_OPERATOR_DEST_OUT;
			else if (s == "lighter")
				cop = CAIRO_OPERATOR_LIGHTEN;
			else if (s == "copy")
				cop = CAIRO_OPERATOR_SOURCE;
			else if (s == "xor")
				cop = CAIRO_OPERATOR_XOR;

			cairo_set_operator(cr, cop);
		}
		void operator=(GlobalCompositeOperationType type)
		{
			cairo_operator_t cop = CAIRO_OPERATOR_IN;
			if (type == GlobalCompositeOperationType::source_over)
				cop = CAIRO_OPERATOR_OVER;
			else if (type == GlobalCompositeOperationType::source_atop)
				cop = CAIRO_OPERATOR_ATOP;
			else if (type == GlobalCompositeOperationType::source_in)
				cop = CAIRO_OPERATOR_IN;
			else if (type == GlobalCompositeOperationType::source_out)
				cop = CAIRO_OPERATOR_OUT;
			else if (type == GlobalCompositeOperationType::destination_over)
				cop = CAIRO_OPERATOR_DEST_OVER;
			else if (type == GlobalCompositeOperationType::destination_atop)
				cop = CAIRO_OPERATOR_DEST_ATOP;
			else if (type == GlobalCompositeOperationType::destination_in)
				cop = CAIRO_OPERATOR_DEST_IN;
			else if (type == GlobalCompositeOperationType::destination_out)
				cop = CAIRO_OPERATOR_DEST_OUT;
			else if (type == GlobalCompositeOperationType::lighter)
				cop = CAIRO_OPERATOR_LIGHTEN;
			else if (type == GlobalCompositeOperationType::copy)
				cop = CAIRO_OPERATOR_SOURCE;
			else if (type == GlobalCompositeOperationType::exclusive_or)
				cop = CAIRO_OPERATOR_XOR;

			cairo_set_operator(cr, cop);
		}

	private:
		// remove copy constructor and assignment operator
		GlobalCompositeOperationProperty(const GlobalCompositeOperationProperty& other) = delete;
		void operator=(const GlobalCompositeOperationProperty& other) = delete;

		cairo_t* cr;
	};

	class FillStyleProperty
	{
	public:
		FillStyleProperty() : cr(nullptr) {}

		void init(cairo_t* ptr)
		{
			cr = ptr;
		}

		void operator=(const char* color)
		{
			unsigned long v = strtoul(color + 1, nullptr, 16);
			double r = ((v & 0xff0000) >> 16) / 255.0;
			double g = ((v & 0xff00) >> 8) / 255.0;
			double b = (v & 0xff) / 255.0;
			cairo_set_source_rgb(cr, r, g, b);
		}
		void operator=(unsigned int color)
		{
			double r = ((color & 0xff0000) >> 16) / 255.0;
			double g = ((color & 0xff00) >> 8) / 255.0;
			double b = (color & 0xff) / 255.0;
			cairo_set_source_rgb(cr, r, g, b);
		}
		void operator=(const canvas::Gradient& gradient)
		{
			cairo_set_source(cr, gradient.getPattern());
		}
		void operator=(const canvas::Pattern& pat)
		{
			cairo_set_source(cr, pat.getPattern());
		}
	private:
		// remove copy constructor and assignment operator
		FillStyleProperty(const FillStyleProperty& other) = delete;
		void operator=(const FillStyleProperty& other) = delete;

		cairo_t* cr;
	};

	class StrokeStyleProperty
	{
	public:
		StrokeStyleProperty() : cr(nullptr) {}

		void init(cairo_t* ptr)
		{
			cr = ptr;
		}

		void operator=(const char* color)
		{
			unsigned long v = strtoul(color + 1, nullptr, 16);
			double r = ((v & 0xff0000) >> 16) / 255.0;
			double g = ((v & 0xff00) >> 8) / 255.0;
			double b = (v & 0xff) / 255.0;
			cairo_set_source_rgb(cr, r, g, b);
		}
		void operator=(unsigned int color)
		{
			double r = ((color & 0xff0000) >> 16) / 255.0;
			double g = ((color & 0xff00) >> 8) / 255.0;
			double b = (color & 0xff) / 255.0;
			cairo_set_source_rgb(cr, r, g, b);
		}
		void operator=(const canvas::Gradient& gradient)
		{
			cairo_set_source(cr, gradient.getPattern());
		}
		void operator=(const canvas::Pattern& pat)
		{
			cairo_set_source(cr, pat.getPattern());
		}
	private:
		// remove copy constructor and assignment operator
		StrokeStyleProperty(const StrokeStyleProperty& other) = delete;
		void operator=(const StrokeStyleProperty& other) = delete;

		cairo_t* cr;
	};

	class FontProperty
	{
	public:
		FontProperty() : cr(nullptr) {}

		void init(cairo_t* ptr)
		{
			cr = ptr;
		}

		void operator=(const char* value)
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
	private:
		// remove copy constructor and assignment operator
		FontProperty(const FontProperty& other) = delete;
		void operator=(const FontProperty& other) = delete;

		cairo_t* cr;
	};

	class LineCapProperty
	{
	public:
		LineCapProperty() : cr(nullptr) {}

		void init(cairo_t* ptr)
		{
			cr = ptr;
		}

		void operator=(LineCap cap)
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
		operator LineCap()
		{
			cairo_line_cap_t cairo_cap = cairo_get_line_cap(cr);
			LineCap cap = LineCap::butt;

			if (cairo_cap == CAIRO_LINE_CAP_BUTT)
				cap = LineCap::butt;
			else if (cairo_cap == CAIRO_LINE_CAP_ROUND)
				cap = LineCap::round;
			else if (cairo_cap == CAIRO_LINE_CAP_SQUARE)
				cap = LineCap::square;

			return cap;
		}
	private:
		// remove copy constructor and assignment operator
		LineCapProperty(const LineCapProperty& other) = delete;
		void operator=(const LineCapProperty& other) = delete;

		cairo_t* cr;
	};

	class LineJoinProperty
	{
	public:
		LineJoinProperty() : cr(nullptr) {}

		void init(cairo_t* ptr)
		{
			cr = ptr;
		}

		void operator=(LineJoin join)
		{
			cairo_line_join_t cairo_join = CAIRO_LINE_JOIN_MITER;
			if (join == LineJoin::miter)
				cairo_join = CAIRO_LINE_JOIN_MITER;
			else if (join == LineJoin::round)
				cairo_join = CAIRO_LINE_JOIN_ROUND;
			else if (join == LineJoin::bevel)
				cairo_join = CAIRO_LINE_JOIN_BEVEL;

			cairo_set_line_join(cr, cairo_join);
		}
		operator LineJoin()
		{
			cairo_line_join_t cairo_join = cairo_get_line_join(cr);
			LineJoin join = LineJoin::miter;

			if (cairo_join == CAIRO_LINE_JOIN_MITER)
				join = LineJoin::miter;
			else if (cairo_join == CAIRO_LINE_JOIN_ROUND)
				join = LineJoin::round;
			else if (cairo_join == CAIRO_LINE_JOIN_BEVEL)
				join = LineJoin::bevel;

			return join;
		}
	private:
		// remove copy constructor and assignment operator
		LineJoinProperty(const LineJoinProperty& other) = delete;
		void operator=(const LineJoinProperty& other) = delete;

		cairo_t* cr;
	};

	class LineWidthProperty
	{
	public:
		LineWidthProperty() : cr(nullptr) {}

		void init(cairo_t* ptr)
		{
			cr = ptr;
		}

		void operator=(double width)
		{
			cairo_set_line_width(cr, width);
		}
		operator double()
		{
			return cairo_get_line_width(cr);
		}
	private:
		// remove copy constructor and assignment operator
		LineWidthProperty(const LineWidthProperty& other) = delete;
		void operator=(const LineWidthProperty& other) = delete;

		cairo_t* cr;
	};

	class MiterLimitProperty
	{
	public:
		MiterLimitProperty() : cr(nullptr) {}

		void init(cairo_t* ptr)
		{
			cr = ptr;
		}

		void operator=(double limit)
		{
			cairo_set_miter_limit(cr, limit);
		}

		operator double()
		{
			return cairo_get_miter_limit(cr);
		}

	private:
		// remove copy constructor and assignment operator
		MiterLimitProperty(const MiterLimitProperty& other) = delete;
		void operator=(const MiterLimitProperty& other) = delete;

		cairo_t* cr;
	};

	class Canvas
	{
	public:
		Canvas(const char* name, int width, int height) : surface(nullptr), cr(nullptr)
		{
			surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
			cr = cairo_create(surface);

			fillStyle.init(cr);
			strokeStyle.init(cr);
			font.init(cr);
			lineCap.init(cr);
			lineJoin.init(cr);
			lineWidth.init(cr);
			miterLimit.init(cr);
			globalCompositeOperation.init(cr);
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

		void clearRect(double x, double y, double width, double height)
		{
			ImageData imgData = createImageData("imgData", width, height);

			for (int y = 0; y < imgData.height(); ++y)
			{
				for (int x = 0; x < imgData.width(); ++x)
				{
					int index = (y * imgData.width() + x) * 4;
					imgData.data()[index] = 0xff;
					imgData.data()[index + 1] = 0xff;
					imgData.data()[index + 2] = 0xff;
					imgData.data()[index + 3] = 0xff;
				}

			}
			
			putImageData(imgData, x, y, 0, 0, width, height);
		}

		void strokeRect(double x, double y, double width, double height)
		{
			cairo_rectangle(cr, x, y, width, height);
			cairo_stroke_preserve(cr);
			cairo_stroke(cr);
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

		void rect(double x, double y, double width, double height)
		{
			cairo_rectangle(cr, x, y, width, height);
		}

		void beginPath()
		{
			cairo_new_path(cr);
		}

		void closePath()
		{
			cairo_close_path(cr);
		}

		bool isPointInPath(double x, double y)
		{
			return (cairo_in_stroke(cr, x, y) > 0);
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

		Pattern createPattern(const char* name, const char* image_file, RepeatPattern rp)
		{
			int width, height, channels;
			//stbi_set_flip_vertically_on_load(true);
			unsigned char* image = stbi_load(image_file,
				&width,
				&height,
				&channels,
				STBI_rgb);

			unsigned char* dest_pixel = new unsigned char[width * height * 4];
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					int index = (y * width + x) * 4;
					int src_index = (y * width + x) * 3;
					dest_pixel[index] = image[src_index + 2];
					dest_pixel[index + 1] = image[src_index + 1];
					dest_pixel[index + 2] = image[src_index + 0];
					dest_pixel[index + 3] = 0xff;
				}

			}

			cairo_surface_t* surface = 
				cairo_image_surface_create_for_data(dest_pixel,
					CAIRO_FORMAT_ARGB32,
					width,
					height,
					width * 4);

			cairo_pattern_t* pattern =
				cairo_pattern_create_for_surface(surface);


			cairo_extend_t extend = CAIRO_EXTEND_REPEAT;
			if (rp == RepeatPattern::no_repeat)
				extend = CAIRO_EXTEND_NONE;

			cairo_pattern_set_extend(pattern, extend);

			stbi_image_free((void*)image);

			return std::move(Pattern(surface, pattern, dest_pixel));
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

		void save()
		{
			cairo_save(cr);
		}

		void restore()
		{
			cairo_restore(cr);
		}

		bool savePng(const char* file)
		{
			cairo_status_t status = cairo_surface_write_to_png(surface, file);
			return (status == CAIRO_STATUS_SUCCESS);
		}

		FillStyleProperty fillStyle;
		StrokeStyleProperty strokeStyle;
		FontProperty font;
		LineCapProperty lineCap;
		LineJoinProperty lineJoin;
		LineWidthProperty lineWidth;
		MiterLimitProperty miterLimit;
		GlobalCompositeOperationProperty globalCompositeOperation;
	private:
		// remove copy constructor and assignment operator
		Canvas(const Canvas& other) = delete;
		void operator=(const Canvas& other) = delete;

		cairo_surface_t* surface;
		cairo_t* cr;
	};
}