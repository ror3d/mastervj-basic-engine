#include "DebugHelperImplementation.h"
#include "Core\Engine\Engine.h"
#include <Core/Input/InputManagerImplementation.h>
#include "Material\MaterialParameter.h"
#include "Camera/FPSCameraController.h"
#include <PhysX/PhysXManager.h>
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
	delete m_posRot;
	m_posRot = NULL;
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
	ICameraController* cc = CEngine::GetSingleton().getCameraManager()->GetCurrentCameraController();
	CFPSCameraController* ccfps = dynamic_cast<CFPSCameraController*>(cc);
	if (ccfps)
	{
		m_posRot->Position = ccfps->GetPosition();
		m_posRot->Yaw = ccfps->GetYaw();
		m_posRot->Pitch = ccfps->GetPitch();
	}
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

void CDebugHelperImplementation::RemoveBar(std::string bar){
	std::unordered_map<std::string, TwBar*>::iterator it = m_Bars.find(bar);
	int status = 0;
	if (it != m_Bars.end())
	{
		status = TwDeleteBar(it->second);
		assert(status);
	}
}

void TW_CALL RemoveBar(void* ba)
{
	//TODO receive string correctly
	//probar a pasar vector con string en [0]
	std::vector<std::string> * barnames = (std::vector<std::string> *) ba;
	std::string nameToRemove = barnames->at(0);
	CDebugHelper::GetDebugHelper()->RemoveBar(nameToRemove);
}

void TW_CALL SwitchCameraCallback(void* _app)
{
	if (CEngine::GetSingleton().getCameraManager()->GetCurrentCameraControllerName() == std::string("__debug"))
	{
		CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController("__fps");
	}
	else
	{
		CEngine::GetSingleton().getCameraManager()->SetCurrentCameraController("__debug");
	}
	
}

void TW_CALL ReloadSceneCommands(void* _app)
{
	CEngine::GetSingleton().getSceneRendererCommandManager()->Reload();
}

void TW_CALL ReloadLua(void* _app){
	CEngine::GetSingleton().getScriptManager()->RegisterLUAFunctions();
}
void TW_CALL CreateScene(void* a)
{
	CPhysxColliderShapeDesc desc;
	desc.shape = CPhysxColliderShapeDesc::Shape::Box;
	desc.density = 1;
	desc.material = "box";
	desc.size = Vect3f(4, 4, 4);
	desc.position = Vect3f(0, 2.0f, 0);
	CEngine::GetSingleton().getPhysXManager()->createActor("boxCol", CPhysXManager::ActorType::Static, desc);
}

void TW_CALL OpenMaterialParams(void *material)
{
	CMaterial * mat = (CMaterial *) material;
	std::vector<CMaterialParameter *> * paramsMaterial = mat->getParameters();
	
	CDebugHelper::SDebugBar barMaterialParams;
	barMaterialParams.name = mat->getName()+ " Parameters";
	for (auto it = paramsMaterial->begin(); it != paramsMaterial->end(); ++it)
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = (*it)->getName();
		if ((*it)->getMaterialType() == CMaterialParameter::TMaterialType::FLOAT){
			var.type = CDebugHelper::FLOAT;
			var.mode = CDebugHelper::READ_WRITE;
			var.pFloat = ((CTemplatedMaterialParameter<float>*)(*it))->getValue();
			var.params = (*it)->getParamValues();
		}		

		barMaterialParams.variables.push_back(var);
	}
	CDebugHelper::GetDebugHelper()->RegisterBar(barMaterialParams);
}

void TW_CALL OpenMaterialsBar(void *materialsMap)
{
	std::map<std::string, CMaterial*> * mapMaterials = (std::map<std::string, CMaterial*>*) materialsMap;
	CDebugHelper::SDebugBar barMaterials;
	barMaterials.name = "Materials List Raw Data";
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "...";
		var.type = CDebugHelper::BUTTON;
		var.callback = RemoveBar;
		std::vector<std::string> * name = new std::vector<std::string>();
		name->push_back(barMaterials.name);
		var.ptr = name;

		barMaterials.variables.push_back(var);
	}
	for (auto it = mapMaterials->begin(); it != mapMaterials->end(); ++it)
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = it->second->getName();
		var.type = CDebugHelper::BUTTON;
		var.callback = OpenMaterialParams;
		var.ptr = it->second;

		barMaterials.variables.push_back(var);
	}
	CDebugHelper::GetDebugHelper()->RegisterBar(barMaterials);
}


void CDebugHelperImplementation::CreateMainBar(){
	//----------------------MAIN BAR------------------------
	CDebugHelper::SDebugBar mainBar;
	mainBar.name = "CApplication";
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "background";
		var.type = CDebugHelper::COLOR;
		var.mode = CDebugHelper::READ_WRITE;
		var.pColor = &CEngine::GetSingleton().getContextManager()->m_BackgroundColor;

		mainBar.variables.push_back(var);
	}
	//m_RenderManager->getFPSCamera()->
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Pos";
		var.type = CDebugHelper::POSITION_ORIENTATION;
		var.mode = CDebugHelper::READ_WRITE;
		m_posRot = new SPositionOrientation();
		ICameraController* cc = CEngine::GetSingleton().getCameraManager()->GetCurrentCameraController();
		CFPSCameraController* ccfps = dynamic_cast<CFPSCameraController*>(cc);
		m_posRot->Position = ccfps->GetPosition();
		m_posRot->Yaw = ccfps->GetYaw();
		var.pPositionOrientation = m_posRot;
		var.ptr = m_posRot;

		mainBar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "switch camera";
		var.type = CDebugHelper::BUTTON;
		var.callback = SwitchCameraCallback;

		mainBar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "mouse speed";
		var.type = CDebugHelper::FLOAT;
		var.mode = CDebugHelper::READ_WRITE;
		var.pFloat = ((CInputManagerImplementation*)CInputManager::GetInputManager())->GetMouseSpeed();
		var.params = " min=0.1 max=10 step=0.1 precision=1 ";

		mainBar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "PhysX: Create Box";
		var.type = CDebugHelper::BUTTON;
		var.callback = CreateScene;
		var.data = this;

		mainBar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Reload LUA";
		var.type = CDebugHelper::BUTTON;
		var.callback = ReloadLua;
		var.data = this;

		mainBar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Separator";
		var.mode = CDebugHelper::SEPARATOR;
		var.type = CDebugHelper::NONE;
		mainBar.variables.push_back(var);
	}

	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Reload Options";
		var.mode = CDebugHelper::SEPARATOR;
		var.type = CDebugHelper::LABEL;
		var.params = "label='Reload Options'";

		mainBar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "  - Commands";
		var.type = CDebugHelper::BUTTON;
		var.callback = ReloadSceneCommands;
		var.data = this;

		mainBar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Separator";
		var.mode = CDebugHelper::SEPARATOR;
		var.type = CDebugHelper::NONE;
		mainBar.variables.push_back(var);
	}
	{
		CDebugHelper::SDebugVariable var = {};
		var.name = "Open Materials RawD";
		var.type = CDebugHelper::BUTTON;
		var.callback = OpenMaterialsBar;
		var.data = this;
		var.ptr = CEngine::GetSingleton().getMaterialManager()->getMaterialsMap();

		mainBar.variables.push_back(var);
	}

	CDebugHelper::GetDebugHelper()->RegisterBar(mainBar);
}
