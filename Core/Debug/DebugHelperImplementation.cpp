#include "DebugHelperImplementation.h"
#include "BarHelper.h"
#include "DebugPerf.h"

CDebugHelperImplementation::CDebugHelperImplementation(void *device)
{
	// TODO: inicializar AntTweakBar
	int status = TwInit(TW_DIRECT3D11, device);
	DEBUG_ASSERT(status);

	{
		TwStructMember structMembers[] =
		{
			{ "x", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Position) + offsetof(Vect3f, x), "step=0.5 precision=1" },
			{ "y", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Position) + offsetof(Vect3f, y), "step=0.5 precision=1" },
			{ "z", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Position) + offsetof(Vect3f, z), "step=0.5 precision=1" },
			{ "yaw", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Yaw), "step=0.05 precision=2" },
			{ "pitch", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Pitch), "step=0.05 precision=2" },
			{ "roll", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Roll), "step=0.05 precision=2" }
		};

		m_PosRotType = TwDefineStruct("POSITION_ORIENTATION", structMembers, 6, sizeof(SPositionOrientation), nullptr, nullptr);
	}
}


CDebugHelperImplementation::~CDebugHelperImplementation()
{
	// TODO: finalizar AntTweakBar
	int status = TwTerminate();
	DEBUG_ASSERT(status);
	//delete m_posRot;
	//m_posRot = NULL;
}

void CDebugHelperImplementation::Log(const std::string& text) const
{
	// TODO: OPCIONAL esto va bién tenerlo escribiendo en un fichero
	OutputDebugString(("LOG: " + text).c_str());
}

void CDebugHelperImplementation::Render()
{
	SCOPED_DEBUG_PERF(L"AntTweakBar Draw");

	int status = TwDraw();
	if (!status)
	{
		const char* error = TwGetLastError();
		Log(error);
	}
}

bool CDebugHelperImplementation::Update(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*
	ICameraController* cc = CEngine::GetSingleton().getCameraManager()->GetCurrentCameraController();
	CFPSCameraController* ccfps = dynamic_cast<CFPSCameraController*>(cc);
	if (ccfps)
	{
		m_posRot->Position = ccfps->GetPosition();
		m_posRot->Yaw = ccfps->GetYaw();
		m_posRot->Pitch = ccfps->GetPitch();
	}
	*/
	// TODO: mandarle eventos al AntTweakBar
	return TwEventWin(hWnd, msg, wParam, lParam);
}

void CDebugHelperImplementation::RegisterBar(const SDebugBar& bar)
{
	// TODO: registrar una ventana de debug
	int status = 0;
	std::unordered_map<std::string, TwBar*>::iterator it = m_Bars.find(bar.name);
	if (it != m_Bars.end())
	{
		status = TwDeleteBar(it->second);
		DEBUG_ASSERT(status);
	}

	TwBar* twBar = TwNewBar(bar.name.c_str());

	for (int i = 0; i < bar.variables.size(); ++i)
	{
		if (bar.variables[i].type == BUTTON)
		{
			status = TwAddButton(twBar, bar.variables[i].name.c_str(), bar.variables[i].callback, bar.variables[i].data, "");
			DEBUG_ASSERT(status);
		}
		else
		{
			TwType type = TW_TYPE_FLOAT;
			std::string params = bar.variables[i].params;
			switch (bar.variables[i].type)
			{
			case BOOL:
				type = TW_TYPE_BOOLCPP;
				break;
			case FLOAT:
				type = TW_TYPE_FLOAT;
				break;
			case INT:
				type = TW_TYPE_INT32;
				break;
			case COLOR32:
				type = TW_TYPE_COLOR32;
				params += " coloralpha=true colororder=argb ";
				break;
			case COLOR:
				type = TW_TYPE_COLOR4F;
				break;
			case STRING:
				type = TW_TYPE_CSSTRING(100);
				break;

			case POSITION_ORIENTATION:
				type = m_PosRotType;
				break;

			case NONE:
				type = TW_TYPE_UNDEF;
				break;
			case LABEL:
				type = TW_TYPE_CSSTRING(1);
				break;

			default:
				break;
			}

			switch (bar.variables[i].mode)
			{
			case READ:
				status = TwAddVarRO(twBar, bar.variables[i].name.c_str(), type, bar.variables[i].ptr, params.c_str());
				DEBUG_ASSERT(status);
				break;

			case READ_WRITE:
				status = TwAddVarRW(twBar, bar.variables[i].name.c_str(), type, bar.variables[i].ptr, params.c_str());
				DEBUG_ASSERT(status);
				break;
			case SEPARATOR:
				if (bar.variables[i].type == NONE)
					status = TwAddSeparator(twBar, NULL, NULL);
				else if (bar.variables[i].type == LABEL){
					status = TwAddButton(twBar, "labelName", NULL, NULL, params.c_str());
				}

				assert(status);
				break;

			default:
				break;
			}
		}
	}

	m_Bars[bar.name] = twBar;
}

void CDebugHelperImplementation::RemoveBar(const std::string &bar)
{
	std::unordered_map<std::string, TwBar*>::iterator it = m_Bars.find(bar);

	int status = 0;

	if (it != m_Bars.end())
	{
		status = TwDeleteBar(it->second);
		assert(status);
		m_Bars.erase(it);
	}
}

void CDebugHelperImplementation::CreateMainBar()
{
	CBarHelper::CreateMainBar();
}
