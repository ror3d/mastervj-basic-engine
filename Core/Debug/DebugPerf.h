#pragma once
#include <string>

class ScopedDebugPerf
{
public:
	ScopedDebugPerf(const std::wstring& mark);
	~ScopedDebugPerf();
};

#if defined(_DEBUG)
#define SCOPED_DEBUG_PERF(x) ScopedDebugPerf ___perf(x)
#else
#define SCOPED_DEBUG_PERF(x) 
#endif

