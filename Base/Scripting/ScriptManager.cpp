#include "ScriptManager.h"

#include <selene.h>

#include <fstream>
#include <sstream>

#include <Base/XML/XMLTreeNode.h>

#include "Core/Engine/Engine.h"
#include "Core/Input/InputManager.h"
#include "Graphics/Camera/FPSCameraController.h"
#include <Graphics/Renderable/RenderableObject.h>
#include <Graphics/Renderable/RenderableObjectsManager.h>
#include <Graphics/Animation/AnimatedInstanceModel.h>
#include <Core/Component/ComponentManager.h>
#include <Core/Component/CharControllerComponent.h>
#include <Core/Component/ScriptedComponent.h>
#include <Core/Component/FPSCameraComponent.h>
#include <Graphics/CinematicsAction/CinematicsActionManager.h>
#include <PhysX/PhysXManager.h>
#include <Core/Time/TimeManager.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Cinematics/CinematicManager.h>
#include <Graphics/Layer/LayerManager.h>
#include <Graphics/Renderer/3DElement.h>
#include <Sound/SoundManager.h>
#include <GUI/GUI.h>
#include <Core/Component/TriggerComponent.h>
#include <Core/Scene/Scene.h>
#include <Core/Scene/Element.h>
#include <Core/Trigger/TriggerManager.h>

namespace
{
	void StopScriptErrors(int sd, std::string message, std::exception_ptr ex)
	{
		if (ex)
		{
			std::rethrow_exception(ex);
		}
		else
		{
			throw message;
		}
	}
}

/*
 * This class captures errors printed by lua to redirect them as we wish.
 */
class LuaErrorCapturedStdout
{
public:
	LuaErrorCapturedStdout()
	{
		_old = std::cout.rdbuf(_out.rdbuf());
	}

	~LuaErrorCapturedStdout()
	{
		std::cout.rdbuf(_old);
		OutputDebugStringA(_out.str().c_str());
	}

	std::string Content() const
	{
		return _out.str();
	}

private:
	std::stringstream _out;
	std::streambuf* _old;
};

CScriptManager::CScriptManager()
	: m_state(nullptr)
	, m_initialized(false)
{}


CScriptManager::~CScriptManager()
{
	if (m_state)
	{
		delete m_state;
	}
}

void CScriptManager::Initialize(const std::string& file)
{
	LuaErrorCapturedStdout errorCapture;
	m_state = new sel::State{ true };


	CXMLTreeNode scripts;

	scripts.LoadFile(file.c_str());

	if (!scripts.Exists())
	{
		return;
	}

	std::vector<std::string> toRunOnLoad;

	for (int i = 0; i < scripts.GetNumChildren(); ++i)
	{
		auto script = scripts(i);

		std::string name = script.GetPszProperty("name", "", false);
		std::string file = script.GetPszProperty("file", "", false);
		bool rol = script.GetIntProperty("runonload", 0, false);
		if (file != "" && m_loadedScripts.find(name) == m_loadedScripts.end())
		{
			std::ifstream t(file);
			if (t)
			{
				t.seekg(0, std::ios::end);
				size_t size = t.tellg();
				std::string buffer(size, ' ');
				t.seekg(0);
				t.read(&buffer[0], size);

				DEBUG_ASSERT(name != "");

				if ( name != "" )
				{
					m_loadedScripts[name] = buffer;
					if (rol)
					{
						toRunOnLoad.push_back(name);
				}
				}
			}
		}
	}

	RegisterLUAFunctions();

	m_initialized = true;

	for (auto &const name : toRunOnLoad)
	{
		RunScript(name);
}
}

void CScriptManager::RunScript(const std::string& name)
{
	auto it = m_loadedScripts.find(name);
	DEBUG_ASSERT (it != m_loadedScripts.end());
	
	RunCode(it->second);
}

void CScriptManager::RunCode(const std::string& code)
{
	LuaErrorCapturedStdout errorCapture;
	(*m_state)(code.c_str());
}

void CScriptManager::RunFile(const std::string& fileName)
{
	LuaErrorCapturedStdout errorCapture;
	std::ifstream f(fileName);
	std::string code((std::istreambuf_iterator<char>(f)),
					 std::istreambuf_iterator<char>());
	(*m_state)(code.c_str());
}

void CScriptManager::Load(const std::string& xmlFile)
{
	LuaErrorCapturedStdout errorCapture;
	// TODO
}

void CScriptManager::RegisterLUAFunctions()
{
	LuaErrorCapturedStdout errorCapture;

	bool loaded;

	(*m_state)["DebugPrint"] = [](const std::string& s)
	{
		OutputDebugString((s+"\n").c_str());
	};

//	loaded = (*m_state).Load("Data/Scripting/CharacterController.lua");
//	loaded = (*m_state).Load("Data/Scripting/CinematicsActionManager.lua");

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
		.SetClass<Vect3f, float, float, float>(
			"x", &Vect3f::x,
			"y", &Vect3f::y,
			"z", &Vect3f::z);

	(*m_state)["C3DElement"]
		.SetClass<C3DElement>(
			"GetPitch", &C3DElement::GetPitch,
			"SetPitch", &C3DElement::SetPitch,
			"GetPosition", &C3DElement::GetPosition,
			"SetPosition", &C3DElement::SetPosition,
			"GetQuat", &C3DElement::GetQuat,
			"SetQuat", &C3DElement::SetQuat,
			"GetRoll", &C3DElement::GetRoll,
			"SetRoll", &C3DElement::SetRoll,
			"GetScale", &C3DElement::GetScale,
			"SetScale", &C3DElement::SetScale,
			"GetTransform", &C3DElement::GetTransform,
			"GetVisible", &C3DElement::GetVisible,
			"SetVisible", &C3DElement::SetVisible,
			"GetYaw", &C3DElement::GetYaw,
			"SetYaw", &C3DElement::SetYaw,
			"SetYawPitchRoll", &C3DElement::SetYawPitchRoll,
			"GetPrevPosition", &C3DElement::GetPrevPosition);

	/*
	(*m_state)["CRenderableObject"]
		.SetClass<CRenderableObject>(
			"GetName", &CRenderableObject::getName,
			"SetPosition", &CRenderableObject::SetPosition,
			"GetPosition", &CRenderableObject::GetPosition,
			"SetYaw", &CRenderableObject::SetYaw,
			"GetYaw", &CRenderableObject::GetYaw,
			"SetVisible", &CRenderableObject::SetVisible,
			"IsVisible", &CRenderableObject::GetVisible,
			"AsAnimatedInstance", &CRenderableObject::AsAnimatedInstance,
			"GetCamera", &CRenderableObject::GetCamera,
			"GetCharacterController", &CRenderableObject::GetCharacterController,
			"GetTriggerComponent", &CRenderableObject::GetTriggerComponent);
			
	(*m_state)["CAnimatedInstanceModel"]
		.SetClass<CAnimatedInstanceModel, CXMLTreeNode&>(
			"ClearCycle", &CAnimatedInstanceModel::ClearCycle,
			"BlendCycle", &CAnimatedInstanceModel::BlendCycle,
			"ExecuteAction", &CAnimatedInstanceModel::ExecuteAction,
			"IsCycleAnimationActive", &CAnimatedInstanceModel::IsCycleAnimationActive,
			"IsActionAnimationActive", &CAnimatedInstanceModel::IsActionAnimationActive);
			*/

	(*m_state)["CElement"]
		.SetClass<CElement>(
			"GetName", &CElement::getName,
			"SetPosition", &CElement::SetPosition,
			"GetPosition", &CElement::GetPosition,
			"SetYaw", &CElement::SetYaw,
			"GetYaw", &CElement::GetYaw,
			"SetEnabled", &CElement::SetEnabled,
			"IsEnabled", &CElement::GetEnabled,
			"GetCamera", &CElement::GetCamera,
			"GetCharacterController", &CElement::GetCharacterController,
			"GetTriggerComponent", &CElement::GetTriggerComponent);
			

	(*m_state)["CCharacterControllerComponent"]
		.SetClass<CCharacterControllerComponent, const std::string&, CElement*>(
			"IsGrounded", &CCharacterControllerComponent::IsGrounded,
			"Move", &CCharacterControllerComponent::Move,
			"SetPos", &CCharacterControllerComponent::SetPosition);

	(*m_state)["CFPSCameraComponent"]
		.SetClass<CFPSCameraComponent, const std::string&, CElement*>(
			"SetAsCurrent", &CFPSCameraComponent::SetAsCurrentCamera);


	(*m_state)["TriggerComponent"]
		.SetClass<CTriggerComponent, const std::string&, CElement*>(
		"GetStateTrigger", &CTriggerComponent::GetStateTrigger,
		"SetStateTrigger", &CTriggerComponent::SetStateTrigger,
		"GetName", &CTriggerComponent::getName,
		"GetTriggerLocalName", &CTriggerComponent::GetTriggerLocalName);

	(*m_state)["ICameraController"]
		.SetClass<ICameraController>(
			"SetPosition", &ICameraController::SetPosition,
			"GetPosition", &ICameraController::GetPosition
			);

	(*m_state)["Rect"]
		.SetClass<Rectf, float, float, float, float, bool>(
			"x", &Rectf::x,
			"y", &Rectf::y,
			"w", &Rectf::w,
			"h", &Rectf::h,
			"isRelative", &Rectf::relative
		);
	(*m_state)("CGui = {};");
	auto gui = (*m_state)["CGui"];
	gui["BeginFrame"] = [](Rectf r, int alignToParent, int alignSelf) -> void { CGUI::GetInstance()->BeginFrame(r, (Rectf::Alignment)alignToParent, (Rectf::Alignment)alignSelf); };
	gui["EndFrame"] = []() -> void { CGUI::GetInstance()->EndFrame(); };
	gui["Image"] = [](std::string spriteName, Rectf r, int alignToParent, int alignSelf) -> void { CGUI::GetInstance()->Image(spriteName, r, (Rectf::Alignment)alignToParent, (Rectf::Alignment)alignSelf); };
	//(*m_state)["CGui.Button"] = static_cast<CGUI::MouseButtonState (*) ( const std::string&, const Rectf&, const Vect2f&, Rectf::Alignment, Rectf::Alignment)>(&CGUI::Button);
	gui["Button"] = [](std::string skin, Rectf r, int alignToParent, int alignSelf) -> int { return (int)CGUI::GetInstance()->Button(skin, r, (Rectf::Alignment)alignToParent, (Rectf::Alignment)alignSelf); };
	gui["Text"] = [](std::string font, std::string text, Rectf bounds, int alignToParent, int alignSelf, bool overflowX) -> void { return CGUI::GetInstance()->Text(font, text, bounds, (Rectf::Alignment)alignToParent, (Rectf::Alignment)alignSelf, overflowX); };

	gui["SetFontColor"] = [](float r, float g, float b) { CGUI::GetInstance()->SetFontColor(CColor(r, g, b)); };

	//Engine References
	(*m_state)["CPhysicsManager"].SetObj(
		*CEngine::GetSingleton().getPhysXManager(),
		"moveCharController", &CPhysXManager::moveCharacterController,
		"createController", &CPhysXManager::createController);

	(*m_state)["CInputManager"].SetObj<CInputManager>(
		*CInputManager::GetInputManager(),
		"GetAxis", &CInputManager::GetAxis,
		"IsActionActive", &CInputManager::IsActionActive);

	(*m_state)["timeManager"].SetObj(
		*CEngine::GetSingleton().getTimerManager(),
		"GetElapsedTime", &CTimerManager::getElapsedTime);

	(*m_state)["CCameraManager"].SetObj(
		*CEngine::GetSingleton().getCameraManager(),
		"SetCurrentCameraController", &CCameraManager::SetCurrentCameraController,
		"GetCurrentCamera", &CCameraManager::GetCurrentCameraController);

	(*m_state)["CinematicsActionManager"].SetObj(
		*CEngine::GetSingleton().getCinematicsActionManager(),
		"m_FileName", &CCinematicsActionManager::m_FileName);

	(*m_state)["CCinematicsManager"].SetObj(
		*CEngine::GetSingleton().getCinematicManager(),
		"Play", &CCinematicManager::Play,
		"PlayByName", &CCinematicManager::PlayByName,
		"StopByName", &CCinematicManager::StopByName,
		"PauseByName", &CCinematicManager::PauseByName);

	(*m_state)["CSoundManager"].SetObj(
		*CEngine::GetSingleton().getSoundManager(),
		//"PlayEvent", &CSoundManager::PlayEvent, 
		"LaunchSoundEventDefaultSpeaker", &CSoundManager::LaunchSoundEventDefaultSpeaker,
		"LaunchSoundEventXMLpeaker", &CSoundManager::LaunchSoundEventXMLSpeaker,
		"LaunchSoundEventDynamicSpeaker", &CSoundManager::LaunchSoundEventDynamicSpeaker,
		"SetVolume", &CSoundManager::SetVolume);
	(*m_state)["CTriggerManager"].SetObj(
		*CEngine::GetSingleton().getTriggerManager(),
		"GetTrigger", &CTriggerManager::get);

	(*m_state)["DebugPrint"] = [](const std::string& s)
	{
		OutputDebugString((s+"\n").c_str());
	};
}
