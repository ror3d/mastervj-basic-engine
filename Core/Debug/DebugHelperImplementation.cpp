#include "DebugHelperImplementation.h"

#include "DebugPerf.h"

#include <Core/Engine/Engine.h>
#include <Core/Input/InputManagerImplementation.h>
#include <Graphics/Material/MaterialParameter.h>
#include <Graphics/Camera/FPSCameraController.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Scene/SceneRendererCommandManager.h>
#include <Base/Scripting/ScriptManager.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Material/Material.h>
#include <Graphics/Particles/ParticleSystemManager.h>
#include <Graphics/Renderable/RenderableObjectTechniqueManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Layer/LayerManager.h>
#include <Graphics/Renderable/RenderableObjectsManager.h>
#include <Graphics/Renderable/RenderableObject.h>
#include <Graphics/Particles/ParticleSystemInstance.h>

#include <Commdlg.h>
#include <tchar.h>

#include <PhysX/PhysXManager.h>

#include <iostream>

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

void TW_CALL RemoveBar(void* bar)
{
	std::vector<std::string> * barnames = (std::vector<std::string> *) bar;
	std::string nameToRemove = barnames->at(0);
	CDebugHelper::GetDebugHelper()->RemoveBar(nameToRemove);
}

void TW_CALL SwitchCameraCallback(void* _app)
{
	static std::string prevCamera = "__debug";
	if (CEngine::GetSingleton().getCameraManager()->GetCurrentCameraControllerName() == std::string("__debug"))
	{
		CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController(prevCamera);
	}
	else
	{
		prevCamera = CEngine::GetSingleton().getCameraManager()->GetCurrentCameraControllerName();
		CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController("__debug");
	}

}

void TW_CALL ReloadParticles(void* _app)
{
	CEngine::GetSingleton().getEffectsManager()->Reload();
	CEngine::GetSingleton().getRenderableObjectTechniqueManager()->Reload();
	CEngine::GetSingleton().getMaterialManager()->reload();
	CEngine::GetSingleton().getStaticMeshManager()->Reload();
	CEngine::GetSingleton().getParticleManager()->reload();
	CEngine::GetSingleton().getLayerManager()->Reload();
	CEngine::GetSingleton().getSceneRendererCommandManager()->Reload();
}

void TW_CALL EqualVarsFloat(void *rng)
{
	range<float>* rang = (range<float>*) rng;

	rang->second = rang->first;
}

void TW_CALL EqualVarsColor(void *rng)
{
	range<CColor>* rang = (range<CColor>*) rng;

	rang->second = rang->first;
}

void TW_CALL EqualVarsVect3f(void *rng)
{
	range<Vect3f>* rang = (range<Vect3f>*) rng;

	rang->second = rang->first;
}

void TW_CALL SaveToFile(void *part)
{
	CEngine::GetSingleton().getParticleManager()->writeFile();
	CEngine::GetSingleton().getMaterialManager()->writeFile();
}

void TW_CALL LoadTexture(void *part)
{
	CParticleSystemClass * particle = (CParticleSystemClass*)part;

	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrDefExt = 0;
	ofn.lpstrFile = new TCHAR[MAX_PATH];
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrInitialDir = 0;
	ofn.lpstrTitle = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	LPSTR currentDir = new TCHAR[MAX_PATH];
	currentDir[0] = '\0';
	GetCurrentDirectory(MAX_PATH, currentDir);

	std::string path = std::string(currentDir);

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	GetOpenFileName(&ofn);

	BOOL b = 0;

	//Opened file
	if (_tcslen(ofn.lpstrFile) != 0)
	{
		_splitpath_s(ofn.lpstrFile, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);

		std::string texturePath = "Data\\Textures\\" + std::string(fname) + std::string(ext);

		b = CopyFile(ofn.lpstrFile, (path + texturePath).c_str(), 1);

		SetCurrentDirectory(currentDir);

		CTexture * tex = new CTexture();
		tex->load(texturePath);

		CMaterial * mat = CEngine::GetSingleton().getMaterialManager()->get(particle->getName());

		if (mat == NULL)
		{
			CMaterial * matCopy = new CMaterial(*particle->material);

			matCopy->setName(particle->getName());
			matCopy->SetTextureAtStage(tex, 0);

			particle->material = matCopy;

			CEngine::GetSingleton().getMaterialManager()->add(matCopy);
		}
		else
		{
			mat->SetTextureAtStage(tex, 0);
		}
	}
}

void AddFloatParameter(CDebugHelper::SDebugBar &bar, std::string name, float *value, std::string params)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::FLOAT;
	var.mode = CDebugHelper::READ_WRITE;
	var.pFloat = value;
	var.params = params;

	bar.variables.push_back(var);
}

void AddIntParameter(CDebugHelper::SDebugBar &bar, std::string name, int *value, std::string params)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::INT;
	var.mode = CDebugHelper::READ_WRITE;
	var.pInt = value;
	var.params = params;

	bar.variables.push_back(var);
}

void AddBoolParameter(CDebugHelper::SDebugBar &bar, std::string name, bool *value)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BOOL;
	var.mode = CDebugHelper::READ_WRITE;
	var.pBool = value;

	bar.variables.push_back(var);
}

void AddSeparator(CDebugHelper::SDebugBar &bar)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = "Separator";
	var.mode = CDebugHelper::SEPARATOR;
	var.type = CDebugHelper::NONE;

	bar.variables.push_back(var);
}

void AddLabel(CDebugHelper::SDebugBar &bar, std::string name)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = NULL;

	bar.variables.push_back(var);
}

void AddButtonTexture(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, CParticleSystemClass *value)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = callback;
	var.ptr = value;

	bar.variables.push_back(var);
}

void AddColor(CDebugHelper::SDebugBar &bar, std::string name, CColor *value)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::COLOR;
	var.mode = CDebugHelper::READ_WRITE;
	var.pColor = value;

	bar.variables.push_back(var);
}

void AddButtonFloat(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, range<float> *value)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = callback;
	var.ptr = value;

	bar.variables.push_back(var);
}

void AddButtonColor(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, range<CColor> *value)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = callback;
	var.ptr = value;

	bar.variables.push_back(var);
}

void AddButtonVect3f(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, range<Vect3f> *value)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = callback;
	var.ptr = value;

	bar.variables.push_back(var);
}

void AddRangeFloat(CDebugHelper::SDebugBar &bar, std::string name, range<float> *value, std::string params)
{
	AddButtonFloat(bar, "E " + name, EqualVarsFloat, value);
	AddFloatParameter(bar, "L " + name, &value->first, params);
	AddFloatParameter(bar, "U " + name, &value->second, params);
}

void AddRangeColor(CDebugHelper::SDebugBar &bar, std::string name, range<CColor> *value, std::string params = "")
{
	AddButtonColor(bar, "E " + name, EqualVarsColor, value);
	AddColor(bar, "L " + name, &value->first);
	AddColor(bar, "U " + name, &value->second);
}

void AddRangeVect3f(CDebugHelper::SDebugBar &bar, std::string name, range<Vect3f> *value, std::string params = "")
{
	AddButtonVect3f(bar, "E " + name, EqualVarsVect3f, value);

	AddFloatParameter(bar, "L " + name + " X", &value->first.x, params + " group='Lower " + name + "'");
	AddFloatParameter(bar, "L " + name + " Y", &value->first.y, params + " group='Lower " + name + "'");
	AddFloatParameter(bar, "L " + name + " Z", &value->first.z, params + " group='Lower " + name + "'");

	AddFloatParameter(bar, "U " + name + " X", &value->second.x, params + " group='Upper " + name + "'");
	AddFloatParameter(bar, "U " + name + " Y", &value->second.y, params + " group='Upper " + name + "'");
	AddFloatParameter(bar, "U " + name + " Z", &value->second.z, params + " group='Upper " + name + "'");
}

#define IDD_DIALOG1 101
#define IDC_EDIT1 1001

std::string enteredChars = "";

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR name[16];
	WORD characters;

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_EDIT1)
		{
			SendMessage(hDlg, DM_SETDEFID, (WPARAM)IDOK, (LPARAM)0);
		}

		switch (wParam)
		{
		case IDOK:
			characters = (WORD)SendDlgItemMessage(hDlg, IDC_EDIT1, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);

			if (characters >= 16)
			{
				MessageBox(hDlg, "Too many characters.", "Error", MB_OK);

				EndDialog(hDlg, TRUE);
				enteredChars = "";
				return FALSE;
			}
			else if (characters == 0)
			{
				MessageBox(hDlg, "No characters entered.", "Error", MB_OK);

				EndDialog(hDlg, TRUE);
				enteredChars = "";
				return FALSE;
			}

			// Put the number of characters into first word of buffer. 
			*((LPWORD)name) = characters;

			// Get the characters. 
			SendDlgItemMessage(hDlg,
				IDC_EDIT1,
				EM_GETLINE,
				(WPARAM)0,       // line 0 
				(LPARAM)name);

			// Null-terminate the string. 
			name[characters] = 0;

			EndDialog(hDlg, TRUE);

			enteredChars = name;
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		return 0;
	}
	return FALSE;

	UNREFERENCED_PARAMETER(lParam);
}


void CreateParticleParametersBar(CParticleSystemClass * particle, CParticleSystemInstance * particleInstance)
{
	particleInstance->setParticleClass(particle);

	CDebugHelper::SDebugBar bar;

	bar.name = particle->getName();

	CDebugHelper::SDebugVariable var = {};

	{
		var.name = "Close bar";
		var.type = CDebugHelper::BUTTON;
		var.callback = RemoveBar;
		std::vector<std::string>* nameVector = new std::vector<std::string>();
		nameVector->push_back(bar.name);
		var.ptr = nameVector;

		bar.variables.push_back(var);
	}

	var = {};

	{
		var.name = "Save to file";
		var.type = CDebugHelper::BUTTON;
		var.callback = SaveToFile;
		var.ptr = particle;

		bar.variables.push_back(var);
	}

	AddButtonTexture(bar, "Load texture", LoadTexture, particleInstance->getParticleClass());

	AddSeparator(bar);

	AddFloatParameter(bar, "Emit rate", &particle->emitRate, "min=1 max=20 step=1 precision=0");
	AddIntParameter(bar, "Num frames", &particle->numFrames, "min=1 max=30 step=1 precision=0");
	AddFloatParameter(bar, "Time per frame", &particle->timePerFrame, "min=0 max=10 step=1 precision=0");
	AddBoolParameter(bar, "LoopFrames", &particle->loopFrames);

	AddSeparator(bar);

	std::vector<CMaterialParameter*> * params = particle->material->getParameters();

	AddFloatParameter(bar, "Sprite X", &((CTemplatedMaterialParameter<Vect2f>*)params->at(0))->getValue()->x, params->at(0)->getParamValues() + " group='Sprite size'");
	AddFloatParameter(bar, "Sprite Y", &((CTemplatedMaterialParameter<Vect2f>*)params->at(0))->getValue()->y, params->at(0)->getParamValues() + " group='Sprite size'");
	AddFloatParameter(bar, "Lerp sprite", ((CTemplatedMaterialParameter<float>*)params->at(1))->getValue(), params->at(1)->getParamValues());
	AddFloatParameter(bar, "Ratio Y", ((CTemplatedMaterialParameter<float>*)params->at(2))->getValue(), params->at(2)->getParamValues());

	AddSeparator(bar);

	AddRangeFloat(bar, "size", &particle->size, "min=0 max=5 step=0.1 precision=1");

	AddSeparator(bar);

	AddRangeVect3f(bar, "velocity", &particle->startVelocity, "min=-1 max=1 step=0.1 precision=1");

	AddSeparator(bar);

	AddRangeVect3f(bar, "acceleration", &particle->acceleration, "min=-1 max=1 step=0.1 precision=1");

	AddSeparator(bar);

	AddRangeFloat(bar, "angle", &particle->startAngle, "min=0 max=360 step=0.5 precision=1");

	AddSeparator(bar);

	AddRangeFloat(bar, "angle speed", &particle->angleSpeed, "min=-1 max=1 step=0.1 precision=1");

	AddSeparator(bar);

	AddRangeFloat(bar, "angle acceleration", &particle->angleAcceleration, "min=-1 max=1 step=0.1 precision=1");

	AddSeparator(bar);

	AddRangeFloat(bar, "life", &particle->life, "min=1 max=5 step=1 precision=0");

	AddSeparator(bar);

	AddRangeColor(bar, "color", &particle->color);

	AddSeparator(bar);

	CDebugHelper::GetDebugHelper()->RegisterBar(bar);
}


void TW_CALL OpenParticleParameters(void *part)
{
	CParticleSystemInstance* particleInstance = (CParticleSystemInstance*)CEngine::GetSingleton().getLayerManager()->get("particles")->get("particle");
	CParticleSystemClass * particle = (CParticleSystemClass *)part;

	if (particle->getName() == "new_particle")
	{
		INT_PTR status = DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

		if (status == 1 && enteredChars != "")
		{
			CParticleSystemClass * newParticle = CEngine::GetSingleton().getParticleManager()->get(enteredChars);
			
			if (newParticle == NULL)
			{
				newParticle = new CParticleSystemClass(*particle);
				newParticle->setName(enteredChars);
				CEngine::GetSingleton().getParticleManager()->add(newParticle);
				CreateParticleParametersBar(newParticle, particleInstance);
			}
			else
			{
				MessageBox(NULL, "Name already in use", "Invalid name", MB_OK);
			}

			enteredChars = "";
		}
	}
	else
	{
		CreateParticleParametersBar(particle, particleInstance);
	}
}


void TW_CALL OpenParticlesBar(void *particlesMap)
{
	std::map<std::string, CMaterial*> * mapParticles = (std::map<std::string, CMaterial*>*) particlesMap;
	CDebugHelper::SDebugBar barParticles;
	barParticles.name = "Particles List";

	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Close bar";
		var.type = CDebugHelper::BUTTON;
		var.callback = RemoveBar;
		std::vector<std::string>* nameVector = new std::vector<std::string>();
		nameVector->push_back(barParticles.name);
		var.ptr = nameVector;

		barParticles.variables.push_back(var);
	}


	for (auto it = mapParticles->begin(); it != mapParticles->end(); ++it)
	{
		if (it->second->getName() != "new_particle")
		{
			CDebugHelper::SDebugVariable var = {};
			var.name = it->second->getName();
			var.type = CDebugHelper::BUTTON;
			var.callback = OpenParticleParameters;
			var.ptr = it->second;

			barParticles.variables.push_back(var);
		}
	}
	CDebugHelper::GetDebugHelper()->RegisterBar(barParticles);
}


void CDebugHelperImplementation::CreateMainBar()
{
	CDebugHelper::SDebugBar mainBar;
	mainBar.name = "CApplication";

	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "New particle";
		var.type = CDebugHelper::BUTTON;
		var.callback = OpenParticleParameters;
		var.data = this;
		var.ptr = CEngine::GetSingleton().getParticleManager()->get("new_particle");

		mainBar.variables.push_back(var);
	}

	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Particle list";
		var.type = CDebugHelper::BUTTON;
		var.callback = OpenParticlesBar;
		var.data = this;
		var.ptr = CEngine::GetSingleton().getParticleManager()->getMap();

		mainBar.variables.push_back(var);
	}

	CDebugHelper::GetDebugHelper()->RegisterBar(mainBar);
}
