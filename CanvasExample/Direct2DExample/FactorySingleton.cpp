#include "pch.h"
#include "framework.h"
#include "FactorySingleton.h"
#include "Common.h"

ComPtr<ID2D1Factory1> FactorySingleton::m_D2D1Factory1;
ComPtr<IDWriteFactory> FactorySingleton::m_DWriteFactory;
ComPtr<IWICImagingFactory2> FactorySingleton::m_WICImagingFactory2;
std::mutex FactorySingleton::m_mutex;

ComPtr<ID2D1Factory1> FactorySingleton::GetD2D1Factory()
{
	const std::lock_guard<std::mutex> lock(m_mutex);

	if (!m_D2D1Factory1)
	{
		D2D1_FACTORY_OPTIONS fo = {};

#ifdef _DEBUG
		fo.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

		HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
			fo,
			m_D2D1Factory1.GetAddressOf()));
	}

	return m_D2D1Factory1;
}


ComPtr<IDWriteFactory> FactorySingleton::GetDWriteFactory()
{
	const std::lock_guard<std::mutex> lock(m_mutex);

	if (!m_DWriteFactory)
	{
		HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_DWriteFactory),
			reinterpret_cast<IUnknown**>(m_DWriteFactory.GetAddressOf())));
	}
	return m_DWriteFactory;
}

ComPtr<IWICImagingFactory2> FactorySingleton::GetWICImagingFactory()
{
	const std::lock_guard<std::mutex> lock(m_mutex);

	if (!m_WICImagingFactory2)
	{
		CreateInstance(CLSID_WICImagingFactory2, m_WICImagingFactory2);
	}
	return m_WICImagingFactory2;
}

void FactorySingleton::DestroyWICImagingFactory()
{
	const std::lock_guard<std::mutex> lock(m_mutex);

	if (m_WICImagingFactory2)
	{
		m_WICImagingFactory2.Reset();
	}
}
