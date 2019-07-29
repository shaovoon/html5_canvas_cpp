// Copyright 2019 Shao Voon Wong
// No warranties expressed or implied
// use it at your risk!

// Release log
// v0.1.0: first release
// v0.2.0: Setters converted to properties
// v0.3.0: Add getter of basic types

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
	
	unsigned int fromRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return (unsigned int)((r << 16) | (g << 8) | b);
	}
	
	// https://cairographics.org/manual/cairo-Image-Surfaces.html
	class ImageData
	{
	public:
		ImageData(const char* name, int width, int height) 
			: m_Name(name), m_Width(width), m_Height(height) {}
		ImageData(ImageData&& other)
		{
			m_Name = std::move(other.m_Name);
			m_Width = other.m_Width;
			m_Height = other.m_Height;
		}
		~ImageData()
		{
			EM_ASM_({
				remove_imgdata(UTF8ToString($0));

			}, m_Name.c_str());
		}
		const char* name() const
		{
			return m_Name.c_str();
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

		std::string m_Name;
		int m_Width;
		int m_Height;
	};

	class Gradient
	{
	public:
		Gradient(const char* name) : m_Name(name) {}
		Gradient(Gradient&& other)
		{
			m_Name = std::move(other.m_Name);
		}
		~Gradient()
		{
			EM_ASM_({
				remove_gradient(UTF8ToString($0));

				}, m_Name.c_str());
		}
		void addColorStop(double stop, const char* color)
		{
			EM_ASM_({
				var grad = get_gradient(UTF8ToString($0));

				grad.addColorStop($1, UTF8ToString($2));
				}, m_Name.c_str(), stop, color);
		}
		void addColorStop(double stop, unsigned int color)
		{
			color &= 0xffffff;
			char buf[20];
			sprintf(buf, "#%06x", color);
			EM_ASM_({
				var grad = get_gradient(UTF8ToString($0));

				grad.addColorStop($1, UTF8ToString($2));
				}, m_Name.c_str(), stop, buf);
		}
		const char* getName() const
		{
			return m_Name.c_str();
		}
	private:
		// remove copy constructor and assignment operator
		Gradient(const Gradient& other) = delete;
		void operator=(const Gradient& other) = delete;
	
		std::string m_Name;
	};

	class FillStyleProperty
	{
	public:
		FillStyleProperty() {}

		void init(const char* name)
		{
			m_Name = name;
		}

		void operator=(const char* color)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.fillStyle = UTF8ToString($1);
				}, m_Name.c_str(), color);
		}
		void operator=(unsigned int color)
		{
			color &= 0xffffff;

			char buf[20];
			sprintf(buf, "#%06x", color);
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.fillStyle = UTF8ToString($1);
				}, m_Name.c_str(), buf);
		}
		void operator=(const canvas::Gradient& gradient)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.fillStyle = get_gradient(UTF8ToString($1));
				}, m_Name.c_str(), gradient.getName());
		}
	private:
		// remove copy constructor and assignment operator
		FillStyleProperty(const FillStyleProperty& other) = delete;
		void operator=(const FillStyleProperty& other) = delete;

		std::string m_Name;
	};

	class StrokeStyleProperty
	{
	public:
		StrokeStyleProperty() {}

		void init(const char* name)
		{
			m_Name = name;
		}

		void operator=(const char* color)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.strokeStyle = UTF8ToString($1);
				}, m_Name.c_str(), color);
		}
		void operator=(unsigned int color)
		{
			color &= 0xffffff;

			char buf[20];
			sprintf(buf, "#%06x", color);
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.strokeStyle = UTF8ToString($1);
				}, m_Name.c_str(), buf);
		}
		void operator=(const canvas::Gradient& gradient)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.strokeStyle = get_gradient(UTF8ToString($1));
				}, m_Name.c_str(), gradient.getName());
		}
	private:
		// remove copy constructor and assignment operator
		StrokeStyleProperty(const StrokeStyleProperty& other) = delete;
		void operator=(const StrokeStyleProperty& other) = delete;

		std::string m_Name;
	};

	class FontProperty
	{
	public:
		FontProperty() {}

		void init(const char* name)
		{
			m_Name = name;
		}

		void operator=(const char* value)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.font = UTF8ToString($1);
				}, m_Name.c_str(), value);
		}
	private:
		// remove copy constructor and assignment operator
		FontProperty(const FontProperty& other) = delete;
		void operator=(const FontProperty& other) = delete;

		std::string m_Name;
	};

	class LineCapProperty
	{
	public:
		LineCapProperty() {}

		void init(const char* name)
		{
			m_Name = name;
		}

		void operator=(LineCap cap)
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
		
		operator LineCap()
		{
			int cap_val = EM_ASM_INT({
				var ctx = get_canvas(UTF8ToString($0));

				if(ctx.lineCap == 'butt')
					return 0;
				else if(ctx.lineCap == 'round')
					return 1;
				else if(ctx.lineCap == 'square')
					return 2;

				return 0;
				}, m_Name.c_str());

			LineCap cap = LineCap::butt;
			if (cap_val == 0)
				cap = LineCap::butt;
			else if (cap_val == 1)
				cap = LineCap::round;
			else if (cap_val == 2)
				cap = LineCap::square;

			return cap;
		}

	private:
		// remove copy constructor and assignment operator
		LineCapProperty(const LineCapProperty& other) = delete;
		void operator=(const LineCapProperty& other) = delete;

		std::string m_Name;
	};

	class LineJoinProperty
	{
	public:
		LineJoinProperty() {}

		void init(const char* name)
		{
			m_Name = name;
		}

		void operator=(LineJoin join)
		{
			const char* cairo_join = "miter";
			if (join == LineJoin::miter)
				cairo_join = "miter";
			else if (join == LineJoin::round)
				cairo_join = "round";
			else if (join == LineJoin::bevel)
				cairo_join = "bevel";

			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.lineJoin = UTF8ToString($1);
				}, m_Name.c_str(), cairo_join);
		}
		operator LineJoin()
		{
			int join_val = EM_ASM_INT({
				var ctx = get_canvas(UTF8ToString($0));

				if(ctx.lineJoin == 'miter')
					return 0;
				else if(ctx.lineJoin == 'round')
					return 1;
				else if(ctx.lineJoin == 'bevel')
					return 2;
				
				}, m_Name.c_str());
				
			LineJoin join = LineJoin::miter;

			if (join_val == 0)
				join = LineJoin::miter;
			else if (join_val == 1)
				join = LineJoin::round;
			else if (join_val == 2)
				join = LineJoin::bevel;

			return join;
		}

	private:
		// remove copy constructor and assignment operator
		LineJoinProperty(const LineJoinProperty& other) = delete;
		void operator=(const LineJoinProperty& other) = delete;

		std::string m_Name;
	};

	class LineWidthProperty
	{
	public:
		LineWidthProperty() {}

		void init(const char* name)
		{
			m_Name = name;
		}

		void operator=(double width)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.lineWidth = $1;
				}, m_Name.c_str(), width);
		}
		operator double()
		{
			return EM_ASM_DOUBLE({
				var ctx = get_canvas(UTF8ToString($0));

				return ctx.lineWidth;
				}, m_Name.c_str());
		}
	private:
		// remove copy constructor and assignment operator
		LineWidthProperty(const LineWidthProperty& other) = delete;
		void operator=(const LineWidthProperty& other) = delete;

		std::string m_Name;
	};

	class MiterLimitProperty
	{
	public:
		MiterLimitProperty() {}

		void init(const char* name)
		{
			m_Name = name;
		}

		void operator=(double limit)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.miterLimit = limit;
				}, m_Name.c_str(), limit);
		}
		operator double()
		{
			return EM_ASM_DOUBLE({
				var ctx = get_canvas(UTF8ToString($0));

				return ctx.miterLimit;
				}, m_Name.c_str());
		}

	private:
		// remove copy constructor and assignment operator
		MiterLimitProperty(const MiterLimitProperty& other) = delete;
		void operator=(const MiterLimitProperty& other) = delete;

		std::string m_Name;
	};

	class Canvas
	{
	public: 
		Canvas(const char* name, int width, int height) : m_Name(name)
		{
			EM_ASM_({
				add_canvas(UTF8ToString($0))
				}, m_Name.c_str());
				
			fillStyle.init(name);
			strokeStyle.init(name);
			font.init(name);
			lineCap.init(name);
			lineJoin.init(name);
			lineWidth.init(name);
			miterLimit.init(name);
		}
		~Canvas()
		{
			EM_ASM_({
				remove_canvas(UTF8ToString($0));

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
		
		void bezierCurveTo(double cp1x, double cp1y, double cp2x, double cp2y, double endx, double endy)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

                ctx.bezierCurveTo($1, $2, $3, $4, $5, $6);
            }, m_Name.c_str(), cp1x, cp1y, cp2x, cp2y, endx, endy);
		}
		
		void quadraticCurveTo(double cpx, double cpy, double endx, double endy)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

                ctx.quadraticCurveTo($1, $2, $3, $4);
            }, m_Name.c_str(), cpx, cpy, endx, endy);
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

		Gradient createLinearGradient(const char* name, double x0, double y0, double x1, double y1)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				var grad = ctx.createLinearGradient($1, $2, $3, $4);
				add_gradient(UTF8ToString($5), grad);
				}, m_Name.c_str(), x0, y0, x1, y1, name);
			
			return std::move(Gradient(name));
		}
		
		Gradient createRadialGradient(const char* name, double x0, double y0, double r0, double x1, double y1, double r1)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				var grad = ctx.createRadialGradient($1, $2, $3, $4, $5, $6);
				add_gradient(UTF8ToString($7), grad);
				}, m_Name.c_str(), x0, y0, r0, x1, y1, r1, name);
			
			return std::move(Gradient(name));
		}

		void drawImage(const char* image, double x, double y)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

                var img = document.getElementById(UTF8ToString($1));
                ctx.drawImage(img, $2, $3);
            }, m_Name.c_str(), image, x, y);
		}

		ImageData createImageData(const char* name, int width, int height)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				var imgdata = ctx.createImageData($1, $2);
				add_imgdata(UTF8ToString($3), imgdata);
				}, m_Name.c_str(), width, height, name);
			
			return std::move(ImageData(name, width, height));
		}
		
		void putImageData(ImageData& imgData, int x, int y)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				var imgdata = get_imgdata(UTF8ToString($3));
				ctx.putImageData(imgdata, $1, $2);
				}, m_Name.c_str(), x, y, imgData.name());
		}
		
		void putImageData(ImageData& imgData, int x, int y, int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				var imgdata = get_imgdata(UTF8ToString($7));
				ctx.putImageData(imgdata, $1, $2, $3, $4, $5, $6);
				}, m_Name.c_str(), x, y, dirtyX, dirtyY, dirtyWidth, dirtyHeight, imgData.name());
		}
		
		ImageData getImageData(const char* name, int x, int y, int width, int height)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				var imgdata = ctx.getImageData($1, $2, $3, $4);
				add_imgdata(UTF8ToString($5), imgdata);
				}, m_Name.c_str(), x, y, width, height, name);
			
			return std::move(ImageData(name, width, height));
		}
		
		void save()
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.save();
				}, m_Name.c_str());
		}

		void restore()
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.restore();
				}, m_Name.c_str());
		}

		bool savePng(const char* file)
		{
			// do nothing here.
			return true;
		}

		FillStyleProperty fillStyle;
		StrokeStyleProperty strokeStyle;
		FontProperty font;
		LineCapProperty lineCap;
		LineJoinProperty lineJoin;
		LineWidthProperty lineWidth;
		MiterLimitProperty miterLimit;

	private:
		// remove copy constructor and assignment operator
		Canvas(const Canvas& other) = delete;
		void operator=(const Canvas& other) = delete;

		std::string m_Name;
	};
}