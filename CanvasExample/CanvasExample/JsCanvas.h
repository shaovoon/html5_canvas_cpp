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
		void addColorStop(double stop, const char* color)
		{
			EM_ASM_({
				var grad = get_gradient(UTF8ToString($0));

				grad.addColorStop($1, UTF8ToString($2));
				}, m_Name.c_str(), stop, color);
		}
		const char* getName() const
		{
			return m_Name.c_str();
		}
	private:
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
		
		void set_fillStyle(unsigned int value)
		{
			char buf[20];
			sprintf(buf, "#%08x", value);
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.fillStyle = UTF8ToString($1);
				}, m_Name.c_str(), buf);
		}
		
		void set_fillStyle(const Gradient& grad)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.fillStyle = get_gradient(UTF8ToString($1));
				}, m_Name.c_str(), grad.getName());
		}
		
		void set_strokeStyle(const char* value)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.strokeStyle = UTF8ToString($1);
				}, m_Name.c_str(), value);
		}
		
		void set_strokeStyle(unsigned int value)
		{
			char buf[20];
			sprintf(buf, "#%08x", value);
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.strokeStyle = UTF8ToString($1);
				}, m_Name.c_str(), buf);
		}
		
		void set_strokeStyle(const Gradient& grad)
		{
			EM_ASM_({
				var ctx = get_canvas(UTF8ToString($0));

				ctx.strokeStyle = get_gradient(UTF8ToString($1));
				}, m_Name.c_str(), grad.getName());
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
			printf("imgData.name(): %s\n", imgData.name());
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

		bool savePng(const char* file)
		{
			// do nothing here.
			return true;
		}

	private:
		std::string m_Name;
	};
}