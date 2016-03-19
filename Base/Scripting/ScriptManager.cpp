#include "ScriptManager.h"

#include <selene.h>

#include <fstream>
#include <sstream>
#include "Core/Engine/Engine.h"
#include "Core/Input/InputManager.h"
#include "Core/CharacterController/CharacterControllerInstance.h"
#include "Graphics/Camera/FPSCameraController.h"
#include <Graphics/Renderable/RenderableObject.h>
#include <Graphics/Renderable/RenderableObjectsManager.h>
#include <Graphics/Animation/AnimatedInstanceModel.h>
#include <Core/Component/ScriptedComponent.h>
#include <Graphics/CinematicsAction/CinematicsActionManager.h>

namespace {
	void StopScriptErrors(int sd, std::string message, std::exception_ptr ex) {
		if (ex) {
			std::rethrow_exception(ex);
		}
		else throw message;
	}
}

/*
 * This class captures errors printed by lua to redirect them as we wish.
 */
class LuaErrorCapturedStdout {
public:
	LuaErrorCapturedStdout() {
		_old = std::cout.rdbuf(_out.rdbuf());
	}

	~LuaErrorCapturedStdout() {
		std::cout.rdbuf(_old);
		OutputDebugStringA(_out.str().c_str());
	}

	std::string Content() const {
		return _out.str();
	}

private:
	std::stringstream _out;
	std::streambuf *_old;
};

CScriptManager::CScriptManager()
	: m_state(nullptr)
{
}


CScriptManager::~CScriptManager()
{
	if (m_state)
	{
		delete m_state;
	}
}

void CScriptManager::Initialize()
{
	LuaErrorCapturedStdout errorCapture;
	m_state = new sel::State{ true };
}

void CScriptManager::Destroy()
{
}

void CScriptManager::RunCode(const std::string &code)
{
	LuaErrorCapturedStdout errorCapture;
	(*m_state)(code.c_str());
}

void CScriptManager::RunFile(const std::string &fileName)
{
	LuaErrorCapturedStdout errorCapture;
	std::ifstream f(fileName);
	std::string code((std::istreambuf_iterator<char>(f)),
					std::istreambuf_iterator<char>());
	(*m_state)(code.c_str());
}

void CScriptManager::Load(const std::string &xmlFile)
{
	LuaErrorCapturedStdout errorCapture;
	// TODO
}

void CScriptManager::RegisterLUAFunctions()
{
	LuaErrorCapturedStdout errorCapture;
	
	bool loaded;
	loaded = (*m_state).Load("Data/Scripting/CharacterController.lua");
	loaded = (*m_state).Load("Data/Scripting/CinematicsActionManager.lua");
	loaded = (*m_state).Load("Data/Scripting/CinematicsActions.lua");

	(*m_state)["CScriptManager"]
		.SetObj(*this,
				"RunCode", &CScriptManager::RunCode,
				"RunFile", &CScriptManager::RunFile,
				"Load", &CScriptManager::Load);
	
	//Global
	(*m_state)["CXMLTreeNode"]
		.SetClass<CXMLTreeNode>(
		"LoadFile", &CXMLTreeNode::LoadFile,
		"LoadFileFromString", &CXMLTreeNode::LoadFileFromString,
		"GetNumChildren", &CXMLTreeNode::GetNumChildren,
		"GetChild", &CXMLTreeNode::operator(),
		"GetName", &CXMLTreeNode::GetName,
		"GetPszProperty", &CXMLTreeNode::GetPszProperty,
		"GetPszPropertyFromString", &CXMLTreeNode::GetPszPropertyFromString,
		"GetFloatPropertyFromString", &CXMLTreeNode::GetFloatPropertyFromString);
	(*m_state)["Vect3f"]
		.SetClass<Vect3f, float, float, float>("x", &Vect3f::x,
		"y", &Vect3f::y,
		"z", &Vect3f::z);

	//Engine References
	(*m_state)["CPhysicsManager"].SetObj(
		*CEngine::GetSingleton().getPhysXManager(),
		"moveCharController", &CPhysXManager::moveCharacterController,
		"createController", &CPhysXManager::createController);
	(*m_state)["CInputManager"].SetObj<CInputManager>(
		*CInputManager::GetInputManager(),
		"GetAxis", &CInputManager::GetAxis);
	(*m_state)["timeManager"].SetObj(
		*CEngine::GetSingleton().getTimerManager(),
		"GetElapsedTime", &CTimerManager::getElapsedTime);
	(*m_state)["CCameraManager"].SetObj(
		*CEngine::GetSingleton().getCameraManager(),
		"SetCurrentCameraController", &CCameraManager::SetCurrentCameraController);
	(*m_state)["CinematicsActionManager"].SetObj(
		*CEngine::GetSingleton().getCinematicsActionManager(),
		"m_FileName", &CCinematicsActionManager::m_FileName);
	(*m_state)["CCinematicsManager"].SetObj(
		*CEngine::GetSingleton().getCinematicManager(),
		"Play", &CCinematicManager::Play);

	

	//ComponentCharController
	(*m_state)["CRenderableObjectsManagerModels"].SetObj(
		*CEngine::GetSingleton().getLayerManager()->get("models"),
		"GetResource", &CRenderableObjectsManager::GetCastedResource);
	(*m_state)["CAnimatedInstanceModel"]
		.SetClass<CAnimatedInstanceModel, CXMLTreeNode>(
		"GetComponentManager", &CAnimatedInstanceModel::GetComponentManager,
		"AddComponent", &CAnimatedInstanceModel::AddComponent);
	(*m_state)["CComponentManager"]
		.SetClass<CComponentManager>(
		"GetResource", &CComponentManager::get);
	(*m_state)["CScriptedComponent"]		
		.SetClass<CScriptedComponent, const std::string &, CAnimatedInstanceModel *, const
		std::string &, const std::string &, const std::string &, const std::string &, const std::string &>();
	
}

void CScriptManager::RegisterLUAFunctionsAfter()
{
	LuaErrorCapturedStdout errorCapture;

	//Character Controller	
	(*m_state)["fpsCamera"].SetObj(
		*CEngine::GetSingleton().getCharacterControllerManager()->get("main")->getFpsCamera(),
		"GetYaw", &CFPSCameraController::GetYaw,
		"GetUp", &CFPSCameraController::GetUp,
		"SetPosition", &CFPSCameraController::SetPosition,
		"Update", &CFPSCameraController::Update);
	(*m_state)["objectModel"].SetObj(
		*CEngine::GetSingleton().getCharacterControllerManager()->get("main")->getObjectModel(),
		"SetPosition", &CRenderableObject::SetPosition,
		"SetYaw", &CRenderableObject::SetYaw);	
	(*m_state)["animatedModel"].SetObj(
		*((CAnimatedInstanceModel*)CEngine::GetSingleton().getCharacterControllerManager()->get("main")->getObjectModel()),
		"ClearCycle", &CAnimatedInstanceModel::ClearCycle,
		"BlendCycle", &CAnimatedInstanceModel::BlendCycle);
	
}
