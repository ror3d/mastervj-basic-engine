#include <Windows.h>
#include "AntTweakBar.h"
#include <string>
#include "Engine/Engine.h"

#include "Debug/BarHelper.h"
#include "Debug/BarCalls.h"
#include <Core/Input/InputManagerImplementation.h>
#include <Graphics/Material/MaterialParameter.h>
#include <Graphics/Camera/FPSCameraController.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Scene/SceneRendererCommandManager.h>
#include <Base/Scripting/ScriptManager.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Material/Material.h>
#include <Graphics/Renderable/RenderableObjectTechniqueManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Layer/LayerManager.h>
#include <Graphics/Renderable/RenderableObjectsManager.h>
#include <Graphics/Renderable/RenderableObject.h>
#include <vector>
#include <tchar.h>

#define IDD_DIALOG1 101
#define IDC_EDIT1 1001

std::string INPUT_TEXT;

INT_PTR CALLBACK CBarCalls::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
				INPUT_TEXT = "";
				return FALSE;
			}
			else if (characters == 0)
			{
				MessageBox(hDlg, "No characters entered.", "Error", MB_OK);

				EndDialog(hDlg, TRUE);
				INPUT_TEXT = "";
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

			INPUT_TEXT = name;
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

void CBarCalls::OpenParticleDialog(CParticleSystemClass * particle)
{
	CParticleSystemInstance* particleInstance = (CParticleSystemInstance*)CEngine::GetSingleton().getLayerManager()->get("particles")->get("particle");
	INT_PTR status = DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	if (status == 1 && INPUT_TEXT != "")
	{
		CParticleSystemClass * newParticle = CEngine::GetSingleton().getParticleManager()->get(INPUT_TEXT);

		if (newParticle == NULL)
		{
			newParticle = new CParticleSystemClass(*particle);
			newParticle->setName(INPUT_TEXT);
			CEngine::GetSingleton().getParticleManager()->add(newParticle);
			CBarHelper::CreateParticleParametersBar(newParticle, particleInstance);
			CBarHelper::ReloadBars();
		}
		else
		{
			MessageBox(NULL, "Name already in use", "Invalid name", MB_OK);
		}

		INPUT_TEXT = "";
	}
}

void TW_CALL CBarCalls::RemoveBar(void* bar)
{
	std::vector<std::string> * barnames = (std::vector<std::string> *) bar;
	std::string nameToRemove = barnames->at(0);
	CDebugHelper::GetDebugHelper()->RemoveBar(nameToRemove);
}

void TW_CALL CBarCalls::OpenParticleParameters(void *part)
{
	CParticleSystemInstance* particleInstance = (CParticleSystemInstance*)CEngine::GetSingleton().getLayerManager()->get("particles")->get("particle");
	CParticleSystemClass * particle = (CParticleSystemClass *)part;

	if (particle->getName() == "new_particle")
	{
		OpenParticleDialog(particle);
	}
	else
	{
		CBarHelper::CreateParticleParametersBar(particle, particleInstance);
	}
}

void TW_CALL CBarCalls::CloneParticle(void * part)
{
	CParticleSystemInstance* particleInstance = (CParticleSystemInstance*)CEngine::GetSingleton().getLayerManager()->get("particles")->get("particle");
	CParticleSystemClass * particle = (CParticleSystemClass *)part;

	OpenParticleDialog(particle);
}

void TW_CALL CBarCalls::OpenParticlesBar(void *particlesMap)
{
	std::map<std::string, CMaterial*> * mapParticles = (std::map<std::string, CMaterial*>*) particlesMap;
	CDebugHelper::SDebugBar bar;
	bar.name = "Particles List";

	CBarHelper::AddCloseBar(bar);

	for (auto it = mapParticles->begin(); it != mapParticles->end(); ++it)
	{
		if (it->second->getName() != "new_particle")
		{
			CDebugHelper::SDebugVariable var = {};
			var.name = it->second->getName();
			var.type = CDebugHelper::BUTTON;
			var.callback = OpenParticleParameters;
			var.ptr = it->second;

			bar.variables.push_back(var);
		}
	}
	CDebugHelper::GetDebugHelper()->RegisterBar(bar);
}

void TW_CALL CBarCalls::ReloadParticles(void* _app)
{
	CEngine::GetSingleton().getEffectsManager()->Reload();
	CEngine::GetSingleton().getRenderableObjectTechniqueManager()->Reload();
	CEngine::GetSingleton().getMaterialManager()->reload();
	CEngine::GetSingleton().getStaticMeshManager()->Reload();
	CEngine::GetSingleton().getParticleManager()->reload();
	CEngine::GetSingleton().getLayerManager()->Reload();
	CEngine::GetSingleton().getSceneRendererCommandManager()->Reload();
}

void TW_CALL CBarCalls::EqualVarsFloat(void *rng)
{
	range<float>* rang = (range<float>*) rng;

	rang->second = rang->first;
}

void TW_CALL CBarCalls::EqualVarsColor(void *rng)
{
	range<CColor>* rang = (range<CColor>*) rng;

	rang->second = rang->first;
}

void TW_CALL CBarCalls::EqualVarsVect3f(void *rng)
{
	range<Vect3f>* rang = (range<Vect3f>*) rng;

	rang->second = rang->first;
}

void TW_CALL CBarCalls::SaveToFile(void *_app)
{
	CEngine::GetSingleton().getParticleManager()->writeFile();
	CEngine::GetSingleton().getMaterialManager()->writeFile();
}

void TW_CALL CBarCalls::LoadTexture(void *part)
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

		b = CopyFile(ofn.lpstrFile, (path + "\\" + texturePath).c_str(), 1);

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

void TW_CALL CBarCalls::DeleteParticle(void *part)
{
	CParticleSystemClass * particle = (CParticleSystemClass*)part;

	CParticleSystemInstance* particleInstance = (CParticleSystemInstance*)CEngine::GetSingleton().getLayerManager()->get("particles")->get("particle");
	particleInstance->setParticleClass(CEngine::GetSingleton().getParticleManager()->get("new_particle"));

	std::vector<std::string>* nameVector = new std::vector<std::string>();
	nameVector->push_back(particle->getName());
	RemoveBar(static_cast<void*>(nameVector));

	CEngine::GetSingleton().getParticleManager()->remove(particle->getName());

	CBarHelper::ReloadBars();
}