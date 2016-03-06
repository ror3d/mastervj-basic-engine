#include "DebugPerf.h"

#include "Engine/Engine.h"

#include <d3d11_1.h>
#include <atlbase.h>

namespace
{
	CComPtr<ID3DUserDefinedAnnotation> pPerf;
}

ScopedDebugPerf::ScopedDebugPerf(const std::wstring& mark)
{
#if defined(_DEBUG)
	if (pPerf == nullptr)
	{
		HRESULT hr = CEngine::GetSingleton().getContextManager()->GetDeviceContext()->QueryInterface(__uuidof(pPerf), reinterpret_cast<void**>(&pPerf));
		if (FAILED(hr))
			return;
	}
	pPerf->BeginEvent(mark.c_str());
#endif
}

ScopedDebugPerf::~ScopedDebugPerf()
{
#if defined(_DEBUG)
	if (pPerf != nullptr)
	{
		pPerf->EndEvent();
	}
#endif
}
