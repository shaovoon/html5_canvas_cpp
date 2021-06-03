// Copyright 2021 Shao Voon Wong
// No warranties expressed or implied
// use it at your risk!

// Release log
// v0.1.0: first release
// v0.2.0: Setters converted to properties
// v0.3.0: Add getter of basic types
// v0.4.0: Direct2D canvas

#pragma once
#include "pch.h"
#include <unordered_map>

using namespace D2D1;
using namespace Microsoft::WRL;

namespace direct2d_canvas
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
		Gradient(Gradient&& other) noexcept
		{
		}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void addColorStop(float stop, const char* color)
		{
			if(color[0] != '#')
				color = getColorValue(color);
			unsigned long v = strtoul(color + 1, nullptr, 16);
			float r = ((v & 0xff0000) >> 16) / 255.0;
			float g = ((v & 0xff00) >> 8) / 255.0;
			float b = (v & 0xff) / 255.0;
		}
		void addColorStop(float stop, unsigned int color)
		{
			float r = ((color & 0xff0000) >> 16) / 255.0;
			float g = ((color & 0xff00) >> 8) / 255.0;
			float b = (color & 0xff) / 255.0;
		}
	private:
		// remove copy constructor and assignment operator
		Gradient(const Gradient& other) = delete;
		void operator=(const Gradient& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
	};

	enum class RepeatPattern
	{
		repeat,
		no_repeat
	};

	class Pattern
	{
	public:
		Pattern(unsigned char* pixel) 
			: m_Pixel(pixel) {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}
		Pattern(Pattern&& other) noexcept
		{
			m_Pixel = other.m_Pixel;
			other.m_Pixel = nullptr;
		}

		~Pattern()
		{
			if (m_Pixel)
			{
				delete[] m_Pixel;
				m_Pixel = nullptr;
			}
		}

	private:
		// remove copy constructor and assignment operator
		Pattern(const Pattern& other) = delete;
		void operator=(const Pattern& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
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
		GlobalCompositeOperationProperty() {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void operator=(const char* op)
		{
			std::string s = op;
		}
		void operator=(GlobalCompositeOperationType type)
		{
		}

	private:
		// remove copy constructor and assignment operator
		GlobalCompositeOperationProperty(const GlobalCompositeOperationProperty& other) = delete;
		void operator=(const GlobalCompositeOperationProperty& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
	};

	class FillStyleProperty
	{
	public:
		FillStyleProperty() {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void operator=(const char* color)
		{
			if (color[0] != '#')
				color = getColorValue(color);

			unsigned long v = strtoul(color + 1, nullptr, 16);
			float r = ((v & 0xff0000) >> 16) / 255.0;
			float g = ((v & 0xff00) >> 8) / 255.0;
			float b = (v & 0xff) / 255.0;
		}
		void operator=(unsigned int color)
		{
			float r = ((color & 0xff0000) >> 16) / 255.0;
			float g = ((color & 0xff00) >> 8) / 255.0;
			float b = (color & 0xff) / 255.0;
		}
		void operator=(const canvas::Gradient& gradient)
		{
		}
		void operator=(const canvas::Pattern& pat)
		{
		}
	private:
		// remove copy constructor and assignment operator
		FillStyleProperty(const FillStyleProperty& other) = delete;
		void operator=(const FillStyleProperty& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
	};

	class StrokeStyleProperty
	{
	public:
		StrokeStyleProperty() {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void operator=(const char* color)
		{
			if (color[0] != '#')
				color = getColorValue(color);

			unsigned long v = strtoul(color + 1, nullptr, 16);
			float r = ((v & 0xff0000) >> 16) / 255.0;
			float g = ((v & 0xff00) >> 8) / 255.0;
			float b = (v & 0xff) / 255.0;
		}
		void operator=(unsigned int color)
		{
			float r = ((color & 0xff0000) >> 16) / 255.0;
			float g = ((color & 0xff00) >> 8) / 255.0;
			float b = (color & 0xff) / 255.0;
		}
		void operator=(const canvas::Gradient& gradient)
		{
		}
		void operator=(const canvas::Pattern& pat)
		{
		}
	private:
		// remove copy constructor and assignment operator
		StrokeStyleProperty(const StrokeStyleProperty& other) = delete;
		void operator=(const StrokeStyleProperty& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
	};

	class FontProperty
	{
	public:
		FontProperty() {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void operator=(const char* value)
		{
			m_Font = value;
		}
		const char* getFont() const
		{
			return m_Font.c_str();
		}
	private:
		// remove copy constructor and assignment operator
		FontProperty(const FontProperty& other) = delete;
		void operator=(const FontProperty& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
		std::string m_Font;
	};

	class LineCapProperty
	{
	public:
		LineCapProperty() {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void operator=(LineCap cap)
		{
		}
		operator LineCap()
		{
		}
	private:
		// remove copy constructor and assignment operator
		LineCapProperty(const LineCapProperty& other) = delete;
		void operator=(const LineCapProperty& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
	};

	class LineJoinProperty
	{
	public:
		LineJoinProperty() {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void operator=(LineJoin join)
		{
		}
		operator LineJoin()
		{
		}
	private:
		// remove copy constructor and assignment operator
		LineJoinProperty(const LineJoinProperty& other) = delete;
		void operator=(const LineJoinProperty& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
	};

	class LineWidthProperty
	{
	public:
		LineWidthProperty() {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void operator=(float width)
		{
		}
		operator float()
		{
		}
	private:
		// remove copy constructor and assignment operator
		LineWidthProperty(const LineWidthProperty& other) = delete;
		void operator=(const LineWidthProperty& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
	};

	class MiterLimitProperty
	{
	public:
		MiterLimitProperty() {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void operator=(float limit)
		{
		}

		operator float()
		{
		}

	private:
		// remove copy constructor and assignment operator
		MiterLimitProperty(const MiterLimitProperty& other) = delete;
		void operator=(const MiterLimitProperty& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
	};

	class ShadowOffsetProperty
	{
	public:
		ShadowOffsetProperty() : m_Offset(0.0) {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

		void operator=(float offset)
		{
			m_Offset = offset;
		}

		operator float()
		{
			return m_Offset;
		}

	private:
		// remove copy constructor and assignment operator
		ShadowOffsetProperty(const ShadowOffsetProperty& other) = delete;
		void operator=(const ShadowOffsetProperty& other) = delete;

		ComPtr<ID2D1DeviceContext> m_target;
		float m_Offset;
	};

	class ShadowColorProperty
	{
	public:
		ShadowColorProperty() : m_Color(0) {}
		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}

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
							float d = strtod(temp.c_str(), nullptr);
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

		ComPtr<ID2D1DeviceContext> m_target;
		unsigned int m_Color;
	};

	class ShadowBlurProperty
	{
	public:
		ShadowBlurProperty() : m_Blur(0) {}

		void init(ComPtr<ID2D1DeviceContext>& target)
		{
			m_target = target;
		}
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

		ComPtr<ID2D1DeviceContext> m_target;
		unsigned int m_Blur;
	};


	class Canvas
	{
	public:
		Canvas(const char* name, int width, int height) 
			: m_Width(width), m_Height(height)
		{
			// TODO: create surface here.
			
			fillStyle.init(m_target);
			strokeStyle.init(m_target);
			font.init(m_target);
			lineCap.init(m_target);
			lineJoin.init(m_target);
			lineWidth.init(m_target);
			miterLimit.init(m_target);
			globalCompositeOperation.init(m_target);

			fillStyle = "white";
			fillRect(0, 0, m_Width, m_Height);
			fillStyle = "black";
			lineWidth = 1.0;
		}

		~Canvas()
		{
			destroy();
		}

		void beginDraw()
		{

		}

		void endDraw()
		{

		}

		void destroy()
		{
		}

		void fillRect(float x, float y, float width, float height)
		{
		}

		void clearRect(float x, float y, float width, float height)
		{
		}

		void strokeRect(float x, float y, float width, float height)
		{
		}

		void fillText(const char* text, float x, float y)
		{
		}

		void strokeText(const char* text, float x, float y)
		{
		}

		void rect(float x, float y, float width, float height)
		{
		}

		void beginPath()
		{
		}

		void closePath()
		{
		}

		bool isPointInPath(float x, float y)
		{
		}

		void moveTo(float x, float y)
		{
		}

		void lineTo(float x, float y)
		{
		}

		void clip()
		{
		}

		void arc(float xc, float yc,
			float radius,
			float angle1, float angle2)
		{
		}

		void stroke()
		{
		}

		void fill()
		{
		}

		void scale(float sx, float sy)
		{
			m_target->SetTransform(D2D1::Matrix3x2F::Scale(sx, sy));
		}

		void translate(float tx, float ty)
		{
			m_target->SetTransform(D2D1::Matrix3x2F::Translation(tx, ty));
		}

		void rotate(float angle)
		{
			m_target->SetTransform(D2D1::Matrix3x2F::Rotation(angle));
		}

		void transform(float xx, float xy, float yx, float yy, float x0, float y0)
		{
			D2D1::Matrix3x2F mat(xx, xy, yx, yy, x0, y0);
			m_target->SetTransform(mat);
		}

		void setTransform(float xx, float xy, float yx, float yy, float x0, float y0)
		{
			m_target->SetTransform(D2D1::IdentityMatrix());
			D2D1::Matrix3x2F mat(xx, xy, yx, yy, x0, y0);
			m_target->SetTransform(mat);
		}

		// https://github.com/aleksaro/gloom/wiki/Loading-images-with-stb
		void drawImage(const char* image_file, float x0, float y0)
		{
		}

		Gradient createLinearGradient(const char* name, float x0, float y0, float x1, float y1)
		{
		}

		Gradient createRadialGradient(const char* name, float x0, float y0, float r0, float x1, float y1, float r1)
		{
		}

		Pattern createPattern(const char* name, const char* image_file, RepeatPattern rp)
		{
		}

		// https://cairographics.org/manual/cairo-Image-Surfaces.html
		ImageData createImageData(const char* name, int width, int height)
		{
			unsigned char* data = new unsigned char[width * height * 4];
			return std::move(ImageData(data, width, height));
		}

		void putImageData(ImageData& imgData, int x, int y, int srcX = 0, int srcY = 0, int srcWidth = 0, int srcHeight = 0)
		{
		}

		ImageData getImageData(const char* name, int x, int y, int width, int height)
		{
		}

		void save()
		{
		}

		void restore()
		{
		}

		bool savePng(const char* file)
		{
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

		ComPtr<IDXGIFactory2> m_dxfactory;
		ComPtr<ID2D1DeviceContext> m_target;
		ComPtr<IDXGISwapChain1> m_swapChain;
		ComPtr<ID2D1Bitmap1> m_RTBmp;
		ComPtr<ID2D1Bitmap1> m_clock;
		//ComPtr<ID2D1PathGeometry> m_Path;
		ComPtr<ID2D1StrokeStyle> m_StrokeStyle;

		float hypotenuse(float x1, float y1, float x2, float y2)
		{
			float x = x1 - x2;
			float y = y1 - y2;
			return sqrt((x * x) + (y * y));
		}

		unsigned char clamp(float val, int minimum, int maximum)
		{
			if (val > maximum)
				return maximum;
			else if (val < minimum)
				return minimum;

			return val;
		}

		void setShadowColor()
		{
			/*
			unsigned int color = shadowColor;
			float a = ((color & 0xff000000) >> 24) / 255.0;
			float r = ((color & 0xff0000) >> 16) / 255.0;
			float g = ((color & 0xff00) >> 8) / 255.0;
			float b = (color & 0xff) / 255.0;
			*/
		}

		void getShadowColor(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a)
		{
			unsigned int color = shadowColor;
			*a = ((color & 0xff000000) >> 24);
			*r = ((color & 0xff0000) >> 16);
			*g = ((color & 0xff00) >> 8);
			*b = (color & 0xff);
		}

		inline unsigned char alphaBlend(unsigned char src, unsigned char dest, unsigned char alpha)
		{
			unsigned char invAlpha = 255 - alpha;

			return (unsigned char)((src * alpha + dest * invAlpha) >> 8);
		}

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