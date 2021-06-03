#pragma once
#include <wrl.h>
#include <cassert>

template <typename T>
void CreateInstance(REFCLSID clsid, Microsoft::WRL::ComPtr<T>& ptr)
{
	assert(!ptr && "ptr is not null");
	HR(CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER,
		__uuidof(T), reinterpret_cast<void**>(ptr.GetAddressOf())));
}


