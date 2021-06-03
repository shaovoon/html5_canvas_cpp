#pragma once
#include <mutex>

using namespace D2D1;
using namespace Microsoft::WRL;

class FactorySingleton
{
public:
	static ComPtr<ID2D1Factory1> GetD2D1Factory();
	static ComPtr<IDWriteFactory> GetDWriteFactory();
	static ComPtr<IWICImagingFactory2> GetWICImagingFactory();
	static void DestroyWICImagingFactory();
private:
	static ComPtr<ID2D1Factory1> m_D2D1Factory1;
	static ComPtr<IDWriteFactory> m_DWriteFactory;
	static ComPtr<IWICImagingFactory2> m_WICImagingFactory2;
	static std::mutex m_mutex;
};

