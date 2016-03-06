#include "ScriptManager.h"

#include <selene.h>

#include <fstream>
#include <sstream>
#include "Core/Engine/Engine.h"
#include "Core/Input/InputManager.h"
#include "Core/CharacterController/CharacterControllerInstance.h"
#include "Graphics/Camera/FPSCameraController.h"
#include <Graphics/Renderable/RenderableObject.h>
#include <Graphics/Animation/AnimatedInstanceModel.h>

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
	
	bool loaded = (*m_state).Load("Data/Scripting/CharacterController.lua");

	(*m_state)["CScriptManager"]
		.SetObj(*this,
				"RunCode", &CScriptManager::RunCode,
				"RunFile", &CScriptManager::RunFile,
				"Load", &CScriptManager::Load);
	
	//Character Controller
	(*m_state)["CharacterControllerManager"].SetObj(
		*CEngine::GetSingleton().getCharacterControllerManager(),
		"Create", &CCharacterControllerManager::Create);
	(*m_state)["CPhysicsManager"].SetObj(
		*CEngine::GetSingleton().getPhysXManager(),
		"moveCharController", &CPhysXManager::moveCharacterController);
	(*m_state)["CInputManager"].SetObj<CInputManager>(
		*CInputManager::GetInputManager(),
		"GetAxis", &CInputManager::GetAxis);
	(*m_state)["fpsCamera"].SetObj(
		*CEngine::GetSingleton().getCharacterControllerManager()->get("main")->getFpsCamera(),
		"GetYaw", &CFPSCameraController::GetYaw,
		"GetUp", &CFPSCameraController::GetUp,
		"SetPosition",&CFPSCameraController::SetPosition,
		"Update",&CFPSCameraController::Update);
	(*m_state)["objectModel"].SetObj(
		*CEngine::GetSingleton().getCharacterControllerManager()->get("main")->getObjectModel(),
		"SetPosition", &CRenderableObject::SetPosition,
		"SetYaw", &CRenderableObject::SetYaw);
	(*m_state)["timeManager"].SetObj(
		*CEngine::GetSingleton().getTimerManager(),
		"GetElapsedTime", &CTimerManager::getElapsedTime);
	(*m_state)["animatedModel"].SetObj(
		*((CAnimatedInstanceModel*)CEngine::GetSingleton().getCharacterControllerManager()->get("main")->getObjectModel()),
		"ClearCycle", &CAnimatedInstanceModel::ClearCycle,
		"BlendCycle", &CAnimatedInstanceModel::BlendCycle);
	(*m_state)["Vect3f"]
		.SetClass<Vect3f, float, float, float>("x", &Vect3f::x,
		"y", &Vect3f::y,
		"z", &Vect3f::z);
}
