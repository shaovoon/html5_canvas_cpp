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
#include <unordered_map>

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

	std::unordered_map<std::string, std::string> g_ColorNameMap;

	const char* getColorValue(const char* color_name);
	void initColorNameMap();

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
			if(color[0] != '#')
				color = getColorValue(color);
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
			if (color[0] != '#')
				color = getColorValue(color);

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
			if (color[0] != '#')
				color = getColorValue(color);

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

	class ShadowOffsetProperty
	{
	public:
		ShadowOffsetProperty() : m_Offset(0.0) {}

		void operator=(double offset)
		{
			m_Offset = offset;
		}

		operator double()
		{
			return m_Offset;
		}

	private:
		// remove copy constructor and assignment operator
		ShadowOffsetProperty(const ShadowOffsetProperty& other) = delete;
		void operator=(const ShadowOffsetProperty& other) = delete;

		double m_Offset;
	};

	class ShadowColorProperty
	{
	public:
		ShadowColorProperty() : m_Color(0) {}

		void operator=(const char* color)
		{
			std::string str = color;
			if (str.substr(0, 5) == "rgba(")
			{
				int clr_index = 0;
				bool started = false;
				std::string temp = "";
				unsigned char a = 0;
				unsigned char r = 0;
				unsigned char g = 0;
				unsigned char b = 0;

				for (size_t i = 5; i < str.size(); ++i)
				{
					char ch = str[i];
					if (started == false && (ch == ' ' || ch == '\t' || ch == ','))
					{
						continue;
					}
					else
						started = true;

					if (started)
					{
						if (ch != ' ' && ch != '\t' && ch != ',' && ch != ')')
							temp += ch;
						else
						{
							double d = strtod(temp.c_str(), nullptr);
							if (clr_index == 0) // red
							{
								r = (unsigned char)(int)(d * 255.0);
							}
							else if (clr_index == 1) // green
							{
								g = (unsigned char)(int)(d * 255.0);
							}
							else if (clr_index == 2) // blue
							{
								b = (unsigned char)(int)(d * 255.0);
							}
							else if (clr_index == 3) // alpha
							{
								a = (unsigned char)(int)(d * 255.0);
							}
							started = false;
							temp = "";
							++clr_index;
						}
					}
				}
				m_Color = (unsigned int)((a << 24) | (r << 16) | (g << 8) | b);

				return;
			}
			if (color[0] != '#')
				color = getColorValue(color);

			unsigned long v = strtoul(color + 1, nullptr, 16);
			unsigned char a = ((v & 0xff000000) >> 24);
			unsigned char r = ((v & 0xff0000) >> 16);
			unsigned char g = ((v & 0xff00) >> 8);
			unsigned char b = (v & 0xff);

			m_Color = (unsigned int)((a << 24) | (r << 16) | (g << 8) | b);
		}


		void operator=(unsigned int color)
		{
			m_Color = color;
		}

		operator unsigned int()
		{
			return m_Color;
		}

		bool isTransparent() const
		{
			return (m_Color == 0);
		}

	private:
		// remove copy constructor and assignment operator
		ShadowColorProperty(const ShadowColorProperty& other) = delete;
		void operator=(const ShadowColorProperty& other) = delete;

		unsigned int m_Color;
	};

	class ShadowBlurProperty
	{
	public:
		ShadowBlurProperty() : m_Blur(0) {}

		void operator=(unsigned int blur)
		{
			m_Blur = blur;
		}

		operator unsigned int()
		{
			return m_Blur;
		}

	private:
		// remove copy constructor and assignment operator
		ShadowBlurProperty(const ShadowBlurProperty& other) = delete;
		void operator=(const ShadowBlurProperty& other) = delete;

		unsigned int m_Blur;
	};


	class Canvas
	{
	public:
		Canvas(const char* name, int width, int height) 
			: surface(nullptr), cr(nullptr), m_Width(width), m_Height(height)
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
			if (shadowColor.isTransparent() == false)
			{
				save();

				setShadowColor(cr);

				cairo_rectangle(cr, x + shadowOffsetX, y + shadowOffsetY, width, height);
				cairo_fill(cr);

				restore();
			}
			cairo_rectangle(cr, x, y, width, height);
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
			if (shadowColor.isTransparent() == false)
			{
				save();

				setShadowColor(cr);

				cairo_rectangle(cr, x + shadowOffsetX, y + shadowOffsetY, width, height);
				cairo_stroke(cr);

				restore();
			}
			cairo_rectangle(cr, x, y, width, height);
			cairo_stroke(cr);
		}

		void fillText(const char* text, double x, double y)
		{
			if (shadowColor.isTransparent() == false)
			{
				save();

				setShadowColor(cr);

				cairo_move_to(cr, x + shadowOffsetX, y + shadowOffsetY);
				cairo_show_text(cr, text);

				restore();
			}
			cairo_move_to(cr, x, y);
			cairo_show_text(cr, text);
		}

		void strokeText(const char* text, double x, double y)
		{
			if (shadowColor.isTransparent() == false)
			{
				save();

				setShadowColor(cr);

				cairo_move_to(cr, x + shadowOffsetX, y + shadowOffsetY);
				cairo_text_path(cr, text);
				cairo_stroke(cr);

				restore();
			}
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
			if (shadowColor.isTransparent() == false)
			{
				save();

				cairo_surface_t* tmp_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_Width, m_Height);
				cairo_t* tmp_cr = cairo_create(tmp_surface);

				double offset_x = shadowOffsetX;
				double offset_y = shadowOffsetY;

				int i = 0;
				cairo_path_t* path = nullptr;
				cairo_path_data_t* data = nullptr;
				path = cairo_copy_path(cr);
				for (i = 0; i < path->num_data; i += path->data[i].header.length) {
					data = &path->data[i];
					switch (data->header.type) {
					case CAIRO_PATH_MOVE_TO:
						cairo_move_to(tmp_cr, data[1].point.x + offset_x, data[1].point.y + offset_y);
						break;
					case CAIRO_PATH_LINE_TO:
						cairo_line_to(tmp_cr, data[1].point.x + offset_x, data[1].point.y + offset_y);
						break;
					case CAIRO_PATH_CURVE_TO:
						cairo_curve_to(tmp_cr,
							data[1].point.x + offset_x, data[1].point.y + offset_y,
							data[2].point.x + offset_x, data[2].point.y + offset_y,
							data[3].point.x + offset_x, data[3].point.y + offset_y);
						break;
					case CAIRO_PATH_CLOSE_PATH:
						cairo_close_path(tmp_cr);
						break;

					}

				}

				setShadowColor(tmp_cr);

				cairo_stroke(tmp_cr);

				cairo_surface_write_to_png(tmp_surface, "c:\\temp\\shadow.png");

				cairo_surface_flush(surface);
				cairo_surface_flush(tmp_surface);
				unsigned char* dest_pixel = cairo_image_surface_get_data(surface);

				unsigned char* src_pixel = cairo_image_surface_get_data(tmp_surface);
				for (int ty = 0; ty < m_Height; ++ty)
				{
					for (int tx = 0; tx < m_Width; ++tx)
					{
						int index = (ty * m_Width + tx) * 4;
						dest_pixel[index] = src_pixel[index];
						dest_pixel[index + 1] = src_pixel[index + 1];
						dest_pixel[index + 2] = src_pixel[index + 2];
						dest_pixel[index + 3] = src_pixel[index + 3];
					}
				}

				cairo_surface_mark_dirty(surface);

				cairo_destroy(tmp_cr);
				cairo_surface_destroy(tmp_surface);
				tmp_cr = nullptr;
				tmp_surface = nullptr;

				cairo_path_destroy(path);

				restore();
			}

			cairo_stroke(cr);
		}

		void fill()
		{
			if (shadowColor.isTransparent() == false)
			{
				save();

				cairo_surface_t* tmp_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_Width, m_Height);
				cairo_t* tmp_cr = cairo_create(tmp_surface);

				double offset_x = shadowOffsetX;
				double offset_y = shadowOffsetY;

				int i=0;
				cairo_path_t* path = nullptr;
				cairo_path_data_t* data = nullptr;
				path = cairo_copy_path(cr);
				for (i = 0; i < path->num_data; i += path->data[i].header.length) {
					data = &path->data[i];
					switch (data->header.type) {
					case CAIRO_PATH_MOVE_TO:
						cairo_move_to(tmp_cr, data[1].point.x + offset_x, data[1].point.y + offset_y);
						break;
					case CAIRO_PATH_LINE_TO:
						cairo_line_to(tmp_cr, data[1].point.x + offset_x, data[1].point.y + offset_y);
						break;
					case CAIRO_PATH_CURVE_TO:
						cairo_curve_to(tmp_cr, 
							data[1].point.x + offset_x, data[1].point.y + offset_y,
							data[2].point.x + offset_x, data[2].point.y + offset_y,
							data[3].point.x + offset_x, data[3].point.y + offset_y);
						break;
					case CAIRO_PATH_CLOSE_PATH:
						cairo_close_path(tmp_cr);
						break;
						
					}
					
				}

				setShadowColor(tmp_cr);

				cairo_fill(tmp_cr);

				cairo_surface_write_to_png(tmp_surface, "c:\\temp\\shadow.png");

				cairo_surface_flush(surface);
				cairo_surface_flush(tmp_surface);
				unsigned char* dest_pixel = cairo_image_surface_get_data(surface);

				unsigned char* src_pixel = cairo_image_surface_get_data(tmp_surface);
				for (int ty = 0; ty < m_Height; ++ty)
				{
					for (int tx = 0; tx < m_Width; ++tx)
					{
						int index = (ty * m_Width + tx) * 4;
						dest_pixel[index] = src_pixel[index];
						dest_pixel[index + 1] = src_pixel[index + 1];
						dest_pixel[index + 2] = src_pixel[index + 2];
						dest_pixel[index + 3] = src_pixel[index + 3];

						dest_pixel[index] = 0xff;
						dest_pixel[index + 1] = src_pixel[index + 1];
						dest_pixel[index + 2] = src_pixel[index + 2];
						dest_pixel[index + 3] = src_pixel[index + 3];

						//alphaBlend(unsigned char src, unsigned char dest, unsigned char alpha);
					}
				}

				cairo_surface_mark_dirty(surface);

				cairo_destroy(tmp_cr);
				cairo_surface_destroy(tmp_surface);
				tmp_cr = nullptr;
				tmp_surface = nullptr;

				cairo_path_destroy(path);

				restore();
			}

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
		ShadowOffsetProperty shadowOffsetX;
		ShadowOffsetProperty shadowOffsetY;
		ShadowColorProperty shadowColor;
		ShadowBlurProperty shadowBlur;
	private:
		// remove copy constructor and assignment operator
		Canvas(const Canvas& other) = delete;
		void operator=(const Canvas& other) = delete;

		void setShadowColor(cairo_t * cr_obj)
		{
			unsigned int color = shadowColor;
			double a = ((color & 0xff000000) >> 24) / 255.0;
			double r = ((color & 0xff0000) >> 16) / 255.0;
			double g = ((color & 0xff00) >> 8) / 255.0;
			double b = (color & 0xff) / 255.0;
			cairo_set_source_rgba(cr_obj, r, g, b, a);
		}

		inline unsigned char alphaBlend(unsigned char src, unsigned char dest, unsigned char alpha)
		{
			unsigned char invAlpha = ~alpha;

			return (unsigned char)((src * alpha + dest * invAlpha) >> 8);
		}

		cairo_surface_t* surface;
		cairo_t* cr;
		int m_Width; 
		int m_Height;
	};

	const char* getColorValue(const char* color_name)
	{
		std::string color_name_str = color_name;
		for (size_t i = 0; i < color_name_str.size(); ++i)
		{
			char ch = color_name_str[i];
			if(ch>='A' && ch <= 'Z')
				color_name_str[i] = tolower(ch);
		}
		initColorNameMap();
		auto it = g_ColorNameMap.find(color_name_str);
		if (it == g_ColorNameMap.end())
			throw std::runtime_error("color name not found");

		return it->second.c_str();
	}

	// https://www.w3schools.com/colors/colors_names.asp
	void initColorNameMap()
	{
		if (g_ColorNameMap.size() == 0)
		{
			g_ColorNameMap.insert(std::make_pair("aliceblue", "#F0F8FF"));
			g_ColorNameMap.insert(std::make_pair("antiquewhite", "#FAEBD7"));
			g_ColorNameMap.insert(std::make_pair("aqua", "#00FFFF"));
			g_ColorNameMap.insert(std::make_pair("aquamarine", "#7FFFD4"));
			g_ColorNameMap.insert(std::make_pair("azure", "#F0FFFF"));
			g_ColorNameMap.insert(std::make_pair("beige", "#F5F5DC"));
			g_ColorNameMap.insert(std::make_pair("bisque", "#FFE4C4"));
			g_ColorNameMap.insert(std::make_pair("black", "#000000"));
			g_ColorNameMap.insert(std::make_pair("blanchedalmond", "#FFEBCD"));
			g_ColorNameMap.insert(std::make_pair("blue", "#0000FF"));

			g_ColorNameMap.insert(std::make_pair("blueviolet", "#8A2BE2"));
			g_ColorNameMap.insert(std::make_pair("brown", "#A52A2A"));
			g_ColorNameMap.insert(std::make_pair("burlywood", "#DEB887"));
			g_ColorNameMap.insert(std::make_pair("cadetblue", "#5F9EA0"));
			g_ColorNameMap.insert(std::make_pair("chartreuse", "#7FFF00"));
			g_ColorNameMap.insert(std::make_pair("chocolate", "#D2691E"));
			g_ColorNameMap.insert(std::make_pair("coral", "#FF7F50"));
			g_ColorNameMap.insert(std::make_pair("cornflowerblue", "#6495ED"));
			g_ColorNameMap.insert(std::make_pair("cornsilk", "#FFF8DC"));
			g_ColorNameMap.insert(std::make_pair("crimson", "#DC143C"));
			g_ColorNameMap.insert(std::make_pair("cyan", "#00FFFF"));
			g_ColorNameMap.insert(std::make_pair("darkblue", "#00008B"));

			g_ColorNameMap.insert(std::make_pair("darkcyan", "#008B8B"));
			g_ColorNameMap.insert(std::make_pair("darkgoldenrod", "#B8860B"));
			g_ColorNameMap.insert(std::make_pair("darkgray", "#A9A9A9"));
			g_ColorNameMap.insert(std::make_pair("darkgrey", "#A9A9A9"));
			g_ColorNameMap.insert(std::make_pair("darkgreen", "#006400"));
			g_ColorNameMap.insert(std::make_pair("darkkhaki", "#BDB76B"));
			g_ColorNameMap.insert(std::make_pair("darkmagenta", "#8B008B"));
			g_ColorNameMap.insert(std::make_pair("darkolivegreen", "#556B2F"));
			g_ColorNameMap.insert(std::make_pair("darkorange", "#FF8C00"));
			g_ColorNameMap.insert(std::make_pair("darkorchid", "#9932CC"));
			g_ColorNameMap.insert(std::make_pair("darkred", "#8B0000"));
			g_ColorNameMap.insert(std::make_pair("darksalmon", "#E9967A"));
			g_ColorNameMap.insert(std::make_pair("darkseagreen", "#8FBC8F"));
			g_ColorNameMap.insert(std::make_pair("darkslateblue", "#483D8B"));
			g_ColorNameMap.insert(std::make_pair("darkslategray", "#2F4F4F"));
			g_ColorNameMap.insert(std::make_pair("darkslategrey", "#2F4F4F"));
			g_ColorNameMap.insert(std::make_pair("darkturquoise", "#00CED1"));
			g_ColorNameMap.insert(std::make_pair("darkviolet", "#9400D3"));
			g_ColorNameMap.insert(std::make_pair("deeppink", "#FF1493"));
			g_ColorNameMap.insert(std::make_pair("deepskyblue", "#00BFFF"));
			g_ColorNameMap.insert(std::make_pair("dimgray", "#696969"));
			g_ColorNameMap.insert(std::make_pair("dimgrey", "#696969"));
			g_ColorNameMap.insert(std::make_pair("dodgerblue", "#1E90FF"));

			g_ColorNameMap.insert(std::make_pair("firebrick", "#B22222"));
			g_ColorNameMap.insert(std::make_pair("floralwhite", "#FFFAF0"));
			g_ColorNameMap.insert(std::make_pair("forestgreen", "#228B22"));
			g_ColorNameMap.insert(std::make_pair("fuchsia", "#FF00FF"));
			g_ColorNameMap.insert(std::make_pair("gainsboro", "#DCDCDC"));
			g_ColorNameMap.insert(std::make_pair("ghostwhite", "#F8F8FF"));
			g_ColorNameMap.insert(std::make_pair("gold", "#FFD700"));
			g_ColorNameMap.insert(std::make_pair("goldenrod", "#DAA520"));
			g_ColorNameMap.insert(std::make_pair("gray", "#808080"));
			g_ColorNameMap.insert(std::make_pair("grey", "#808080"));
			g_ColorNameMap.insert(std::make_pair("green", "#008000"));
			g_ColorNameMap.insert(std::make_pair("greenyellow", "#ADFF2F"));
			g_ColorNameMap.insert(std::make_pair("honeydew", "#F0FFF0"));
			g_ColorNameMap.insert(std::make_pair("hotpink", "#FF69B4"));
			g_ColorNameMap.insert(std::make_pair("indianred", "	#CD5C5C"));
			g_ColorNameMap.insert(std::make_pair("indigo", "#4B0082"));
			g_ColorNameMap.insert(std::make_pair("ivory", "#FFFFF0"));
			g_ColorNameMap.insert(std::make_pair("khaki", "#F0E68C"));

			g_ColorNameMap.insert(std::make_pair("lavender", "#E6E6FA"));
			g_ColorNameMap.insert(std::make_pair("lavenderblush", "#FFF0F5"));
			g_ColorNameMap.insert(std::make_pair("lawngreen", "#7CFC00"));
			g_ColorNameMap.insert(std::make_pair("lemonchiffon", "#FFFACD"));
			g_ColorNameMap.insert(std::make_pair("lightblue", "#ADD8E6"));
			g_ColorNameMap.insert(std::make_pair("lightcoral", "#F08080"));
			g_ColorNameMap.insert(std::make_pair("lightcyan", "#E0FFFF"));
			g_ColorNameMap.insert(std::make_pair("lightgoldenrodyellow", "#FAFAD2"));
			g_ColorNameMap.insert(std::make_pair("lightgray", "#D3D3D3"));
			g_ColorNameMap.insert(std::make_pair("lightgrey", "#D3D3D3"));
			g_ColorNameMap.insert(std::make_pair("lightgreen", "#90EE90"));
			g_ColorNameMap.insert(std::make_pair("lightpink", "#FFB6C1"));
			g_ColorNameMap.insert(std::make_pair("lightsalmon", "#FFA07A"));
			g_ColorNameMap.insert(std::make_pair("lightseagreen", "#20B2AA"));
			g_ColorNameMap.insert(std::make_pair("lightskyblue", "#87CEFA"));
			g_ColorNameMap.insert(std::make_pair("lightslategray", "#778899"));
			g_ColorNameMap.insert(std::make_pair("lightslategrey", "#778899"));
			g_ColorNameMap.insert(std::make_pair("lightsteelblue", "#B0C4DE"));
			g_ColorNameMap.insert(std::make_pair("lightyellow", "#FFFFE0"));
			g_ColorNameMap.insert(std::make_pair("lime", "#00FF00"));
			g_ColorNameMap.insert(std::make_pair("limegreen", "#32CD32"));
			g_ColorNameMap.insert(std::make_pair("linen", "#FAF0E6"));

			g_ColorNameMap.insert(std::make_pair("magenta", "#FF00FF"));
			g_ColorNameMap.insert(std::make_pair("maroon", "#800000"));
			g_ColorNameMap.insert(std::make_pair("mediumaquamarine", "#66CDAA"));
			g_ColorNameMap.insert(std::make_pair("mediumblue", "#0000CD"));
			g_ColorNameMap.insert(std::make_pair("mediumorchid", "#BA55D3"));
			g_ColorNameMap.insert(std::make_pair("mediumpurple", "#9370DB"));
			g_ColorNameMap.insert(std::make_pair("mediumseagreen", "#3CB371"));
			g_ColorNameMap.insert(std::make_pair("mediumslateblue", "#7B68EE"));
			g_ColorNameMap.insert(std::make_pair("mediumspringgreen", "#00FA9A"));
			g_ColorNameMap.insert(std::make_pair("mediumturquoise", "#48D1CC"));
			g_ColorNameMap.insert(std::make_pair("mediumvioletred", "#C71585"));
			g_ColorNameMap.insert(std::make_pair("midnightblue", "#191970"));
			g_ColorNameMap.insert(std::make_pair("mintcream", "#F5FFFA"));
			g_ColorNameMap.insert(std::make_pair("mistyrose", "#FFE4E1"));
			g_ColorNameMap.insert(std::make_pair("moccasin", "#FFE4B5"));
			g_ColorNameMap.insert(std::make_pair("navajowhite", "#FFDEAD"));
			g_ColorNameMap.insert(std::make_pair("navy", "#000080"));
			g_ColorNameMap.insert(std::make_pair("oldlace", "#FDF5E6"));
			g_ColorNameMap.insert(std::make_pair("olive", "#808000"));
			g_ColorNameMap.insert(std::make_pair("olivedrab", "#6B8E23"));
			g_ColorNameMap.insert(std::make_pair("orange", "#FFA500"));
			g_ColorNameMap.insert(std::make_pair("orangered", "#FF4500"));
			g_ColorNameMap.insert(std::make_pair("orchid", "#DA70D6"));

			g_ColorNameMap.insert(std::make_pair("palegoldenrod", "#EEE8AA"));
			g_ColorNameMap.insert(std::make_pair("palegreen", "#98FB98"));
			g_ColorNameMap.insert(std::make_pair("paleturquoise", "#AFEEEE"));
			g_ColorNameMap.insert(std::make_pair("palevioletred", "#DB7093"));
			g_ColorNameMap.insert(std::make_pair("papayawhip", "#FFEFD5"));
			g_ColorNameMap.insert(std::make_pair("peachpuff", "#FFDAB9"));
			g_ColorNameMap.insert(std::make_pair("peru", "#CD853F"));
			g_ColorNameMap.insert(std::make_pair("pink", "#FFC0CB"));
			g_ColorNameMap.insert(std::make_pair("plum", "#DDA0DD"));
			g_ColorNameMap.insert(std::make_pair("powderblue", "#B0E0E6"));
			g_ColorNameMap.insert(std::make_pair("purple", "#800080"));

			g_ColorNameMap.insert(std::make_pair("rebeccapurple", "#663399"));
			g_ColorNameMap.insert(std::make_pair("red", "#FF0000"));
			g_ColorNameMap.insert(std::make_pair("rosybrown", "#BC8F8F"));
			g_ColorNameMap.insert(std::make_pair("royalblue", "#4169E1"));
			g_ColorNameMap.insert(std::make_pair("saddlebrown", "#8B4513"));
			g_ColorNameMap.insert(std::make_pair("salmon", "#FA8072"));
			g_ColorNameMap.insert(std::make_pair("sandybrown", "#F4A460"));
			g_ColorNameMap.insert(std::make_pair("seagreen", "#2E8B57"));
			g_ColorNameMap.insert(std::make_pair("seashell", "#FFF5EE"));
			g_ColorNameMap.insert(std::make_pair("sienna", "#A0522D"));
			g_ColorNameMap.insert(std::make_pair("silver", "#C0C0C0"));
			g_ColorNameMap.insert(std::make_pair("skyblue", "#87CEEB"));
			g_ColorNameMap.insert(std::make_pair("slateblue", "#6A5ACD"));
			g_ColorNameMap.insert(std::make_pair("slategray", "#708090"));
			g_ColorNameMap.insert(std::make_pair("slategrey", "#708090"));
			g_ColorNameMap.insert(std::make_pair("snow", "#FFFAFA"));
			g_ColorNameMap.insert(std::make_pair("springgreen", "#00FF7F"));
			g_ColorNameMap.insert(std::make_pair("steelblue", "#4682B4"));
			g_ColorNameMap.insert(std::make_pair("tan", "#D2B48C"));
			g_ColorNameMap.insert(std::make_pair("teal", "#008080"));
			g_ColorNameMap.insert(std::make_pair("thistle", "#D8BFD8"));
			g_ColorNameMap.insert(std::make_pair("tomato", "#FF6347"));
			g_ColorNameMap.insert(std::make_pair("turquoise", "#40E0D0"));
			g_ColorNameMap.insert(std::make_pair("violet", "#EE82EE"));
			g_ColorNameMap.insert(std::make_pair("wheat", "#F5DEB3"));
			g_ColorNameMap.insert(std::make_pair("white", "#FFFFFF"));
			g_ColorNameMap.insert(std::make_pair("whitesmoke", "#F5F5F5"));
			g_ColorNameMap.insert(std::make_pair("yellow", "#FFFF00"));
			g_ColorNameMap.insert(std::make_pair("yellowgreen", "#9ACD32"));
		}
	}

}