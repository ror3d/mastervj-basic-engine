#ifndef DEBUGER_IMPLEMENTATION_H
#define DEBUGER_IMPLEMENTATION_H

#include <AntTweakBar.h>
#include <Windows.h>
#include <unordered_map>
#include "DebugHelper.h"

class CDebugHelperImplementation : public CDebugHelper
{
public:
	CDebugHelperImplementation(void *device);
	~CDebugHelperImplementation();

	bool Update(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void Render();

	virtual void Log(const std::string& text) const;

	virtual void RegisterBar(const SDebugBar& bar);
	virtual void RemoveBar(const std::string& bar);

	void CreateMainBar();

	std::unordered_map<std::string, TwBar*> m_Bars;
	SPositionOrientation * m_posRot;
	TwType m_PosRotType;
};

#endif