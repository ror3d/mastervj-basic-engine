#include "ScriptManager.h"

#include <selene.h>


#include <Base/XML/XMLTreeNode.h>

#include "Core/Engine/Engine.h"
#include "Core/Input/InputManager.h"
#include "Graphics/Camera/FPSCameraController.h"
#include <Core/Component/ComponentManager.h>
#include <Core/Component/CharControllerComponent.h>
#include <Core/Component/ScriptedComponent.h>
#include <Core/Component/FPSCameraComponent.h>
#include <Core/Component/FreeCameraComponent.h>
#include <Core/Component/AnimatedInstanceComponent.h>
#include <Graphics/CinematicsAction/CinematicsActionManager.h>
#include <PhysX/PhysXManager.h>
#include <Core/Time/TimeManager.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Cinematics/CinematicManager.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Renderer/3DElement.h>
#include <Sound/SoundManager.h>
#include <GUI/GUI.h>
#include <Core/Component/TriggerComponent.h>
#include <Core/Scene/Scene.h>
#include <Core/Scene/SceneManager.h>
#include <Core/Scene/Element.h>

#include "LuaErrorCapture.h"

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

		DEBUG_ASSERT(name != "");

		if (file != "" && name != "" && m_loadedScripts.find(name) == m_loadedScripts.end())
		{
			m_scriptFiles[name] = file;

			if ( !ReloadScript( name ) )
			{
				continue;
			}

			if (rol)
			{
				toRunOnLoad.push_back(name);
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

void CScriptManager::destroy()
{
}

void CScriptManager::RunScript(const std::string& name)
{
	auto it = m_loadedScripts.find(name);
	DEBUG_ASSERT (it != m_loadedScripts.end());

	RunCode(it->second);
}

bool CScriptManager::ReloadScript( const std::string & name )
{
	if ( m_scriptFiles.find( name ) == m_scriptFiles.end() )
	{
		return false;
	}
	std::ifstream t(m_scriptFiles[name]);
	if ( !t )
	{
		return false;
	}

	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	std::string buffer(size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);

	m_loadedScripts[name] = buffer;

	auto cm = CEngine::GetSingleton().getComponentManager();
	for ( auto &const cName : m_scriptReferences[name] )
	{
		CComponent* c = cm->get( cName );
		CScriptedComponent* cs = dynamic_cast<CScriptedComponent*>( c );
		if ( cs )
		{
			cs->Reload();
		}
	}

	return true;
}

void CScriptManager::RunCode(const std::string& code)
{
	LuaErrorCapturedStdout errorCapture;
	(*m_state)(code.c_str());
}

void CScriptManager::RunCodeIf(const std::string &condition, const std::string &code)
{
	LuaErrorCapturedStdout errorCapture;
	(*m_state)(("_r = true == (" + condition + ");").c_str());
	if ((*m_state)["r"])
	{
		(*m_state)(code.c_str());
	}
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
		"RunScript", &CScriptManager::RunScript);

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
			"z", &Vect3f::z,
			"Normalized", static_cast<Vect3f (Vect3f::*)() const>(&Vect3f::GetNormalized),
			"Length", &Vect3f::Length,
			"Dot", static_cast<float (Vect3f::*)(const Vect3f&) const>(&Vect3f::operator*),
			"Cross", static_cast<Vect3f (Vect3f::*)(const Vect3f&) const>(&Vect3f::operator^),
			"Add", static_cast<Vect3f (Vect3f::*)(const Vect3f&) const>(&Vect3f::operator+),
			"Mul", static_cast<Vect3f (Vect3f::*)(float) const>(&Vect3f::operator*),
			"Subs", static_cast<Vect3f(Vect3f::*)(const Vect3f&) const>(&Vect3f::operator-)
			);



	(*m_state)["CElement"]
		.SetClass<CElement>(
			"GetName", &CElement::getName,
			"SetPosition", &CElement::SetPosition,
			"GetPosition", &CElement::GetPosition,
			"SetRotation", &CElement::SetRotation,
			"GetRotation", &CElement::GetRotation,
			"SetYaw", &CElement::SetYaw,
			"GetYaw", &CElement::GetYaw,
			"GetScale", &CElement::GetScale,
			"SetScale", &CElement::SetScale,
			"SetEnabled", &CElement::SetEnabled,
			"IsEnabled", &CElement::GetEnabled,
			"GetCamera", &CElement::GetCamera,
			"GetFreeCamera", &CElement::GetFreeCamera,
			"GetCharacterController", &CElement::GetCharacterController,
			"GetAnimatedInstanceComponent", &CElement::GetAnimatedInstanceComponent,
			"GetCollider", &CElement::GetPhysxComponent,
			"SendMessage", static_cast<void(CElement::*)(const std::string&)>(&CElement::SendMsg),
			"SendMessageInt", static_cast<void(CElement::*)(const std::string&, int)>(&CElement::SendMsg),
			"SendMessageFloat", static_cast<void(CElement::*)(const std::string&, float)>(&CElement::SendMsg),
			"SendMessageString", static_cast<void(CElement::*)(const std::string&, const std::string&)>(&CElement::SendMsg),
			"SendMessageElement", static_cast<void(CElement::*)(const std::string&, CElement*)>(&CElement::SendMsg),
			"SendMessageIntFloat", static_cast<void(CElement::*)(const std::string&, int, float)>(&CElement::SendMsg),
			"SendMessageIntStr", static_cast<void(CElement::*)(const std::string&, int, const std::string&)>(&CElement::SendMsg),
			"GetTrigger", &CElement::GetTriggerComponent,
			"GetScript", &CElement::GetScript,
			"Clone", &CElement::Clone);


	(*m_state)["CAnimatedInstanceComponent"]
		.SetClass<CAnimatedInstanceComponent, const CAnimatedInstanceComponent&, CElement*>(
			"ExecuteAction", &CAnimatedInstanceComponent::ExecuteAction,
			"BlendCycle", &CAnimatedInstanceComponent::BlendCycle,
			"ClearCycle", &CAnimatedInstanceComponent::ClearCycle,
			"SetAnimationTime", &CAnimatedInstanceComponent::SetAnimationTime,
			"IsCycleAnimationActive", &CAnimatedInstanceComponent::IsCycleAnimationActive,
			"IsActionAnimationActive", &CAnimatedInstanceComponent::IsActionAnimationActive);

	(*m_state)["CCharacterControllerComponent"]
		.SetClass<CCharacterControllerComponent, const CCharacterControllerComponent&, CElement*>(
			"IsGrounded", &CCharacterControllerComponent::IsGrounded,
			"Move", &CCharacterControllerComponent::Move,
			"SetPosition", &CCharacterControllerComponent::SetPosition,
			"Resize", &CCharacterControllerComponent::Resize,
			"GetHeight", &CCharacterControllerComponent::GetHeight,
			"GetRadius", &CCharacterControllerComponent::GetRadius,
			"SetStickToGround", &CCharacterControllerComponent::SetStickToGround);

	(*m_state)["CColliderComponent"]
		.SetClass<CPhysxComponent, const CPhysxComponent&, CElement*>(
			"Move", &CPhysxComponent::Move,
			"Recreate", &CPhysxComponent::Recreate,
			"IsKinematic", &CPhysxComponent::IsKinematic
			);

	( *m_state )["CScriptedComponent"]
		.SetClass<CScriptedComponent, const CScriptedComponent&, CElement*>(
			"GetLuaObject", std::function<sel::Selector(const CScriptedComponent* _this)>([this]( const CScriptedComponent* _this )
				{
					unsigned tid = _this->GetLuaTableId();
					return ( *m_state )["_componentStates"][tid];
				} ) );


	(*m_state)["CFPSCameraComponent"]
		.SetClass<CFPSCameraComponent, const CFPSCameraComponent&, CElement*>(
			"SetAsCurrent", &CFPSCameraComponent::SetAsCurrentCamera,
			"GetCamOffset", &CFPSCameraComponent::GetCamOffset,
			"SetCamOffset", &CFPSCameraComponent::SetCamOffset,
			"Reset", &CFPSCameraComponent::Reset,
			"SetYaw", &CFPSCameraComponent::SetYaw,
			"GetYaw", &CFPSCameraComponent::GetYaw);

	(*m_state)["CFreeCameraComponent"]
		.SetClass<CFreeCameraComponent, const CFreeCameraComponent&, CElement*>(
			"SetAsCurrent", &CFreeCameraComponent::SetAsCurrentCamera,
			"GetForward", &CFreeCameraComponent::GetForward,
			"SetForward", &CFreeCameraComponent::SetForward,
			"GetUp", &CFreeCameraComponent::GetUp,
			"SetUp", &CFreeCameraComponent::SetUp,
			"GetOffset", &CFreeCameraComponent::GetOffset,
			"SetOffset", &CFreeCameraComponent::SetOffset);


	(*m_state)["CTriggerComponent"]
		.SetClass<CTriggerComponent, const CTriggerComponent&, CElement*>(
		"GetName", &CTriggerComponent::getName,
		"Recreate", &CTriggerComponent::Recreate
		);

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
		"createController", &CPhysXManager::createController,
		"RaycastName", &CPhysXManager::RayCastName);

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
		"Play", static_cast<void(CCinematicManager::*)(std::string)>(&CCinematicManager::Play),
		"Stop", &CCinematicManager::Stop,
		"Reverse", &CCinematicManager::Reverse,
		"Pause", &CCinematicManager::Pause);

	(*m_state)["CSoundManager"].SetObj(
		*CEngine::GetSingleton().getSoundManager(),
		//"PlayEvent", &CSoundManager::PlayEvent,
		"LaunchSoundEventDefaultSpeaker", &CSoundManager::LaunchSoundEventDefaultSpeaker,
		"LaunchSoundEventXMLSpeaker", &CSoundManager::LaunchSoundEventXMLSpeaker,
		"LaunchSoundEventDynamicSpeaker", &CSoundManager::LaunchSoundEventDynamicSpeaker,
		"SetVolume", &CSoundManager::SetVolume);

	(*m_state)["CSceneManager"].SetObj(
		*CEngine::GetSingleton().getSceneManager(),
		"AddObjectToScene", &CSceneManager::AddObjectToScene,
		"DestroyObjectFromScene", &CSceneManager::DestroyObjectFromScene,
		"LoadScene", &CSceneManager::LoadScene,
		"UnloadScene", &CSceneManager::UnloadScene,
		"GetObjectById", &CSceneManager::GetObjectById,
		"IsSceneLoaded", &CSceneManager::IsSceneLoaded,
		"IsSceneUnloaded", &CSceneManager::IsSceneUnloaded,
		"StartedUnload", &CSceneManager::StartedUnload,
		"FinishedLoad", &CSceneManager::FinishedLoad);

	(*m_state)["CStaticMeshManager"].SetObj(
		*CEngine::GetSingleton().getStaticMeshManager(),
		"LoadMeshesFile", &CStaticMeshManager::Load);

	( *m_state )["CMaterialManager"].SetObj(
		*CEngine::GetSingleton().getMaterialManager(),
		"LoadMaterialsFile", &CMaterialManager::load);

	(*m_state)["DebugPrint"] = [](const std::string& s)
	{
		OutputDebugString((s+"\n").c_str());
	};
}
