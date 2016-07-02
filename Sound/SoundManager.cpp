#include "SoundManager.h"
//#include "ISoundManager.h"


#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/IBytes.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AkFilePackagelowLevelIOBlocking.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>

#include <AkSoundEngineDLL.h>

#include <Graphics/Camera/Camera.h>
//#include <Graphics/Renderer/3DElement.h>
#include "Core/Engine/Engine.h"

#include <Base/XML/XMLTreeNode.h>

#pragma comment(lib, "AkSoundEngineDLL.lib")
//#pragma comment(lib, "AkSoundEngine.lib")
//#pragma comment(lib, "AkMemoryMgr.lib")
//#pragma comment(lib, "AkStreamMgr.lib")
//#pragma comment(lib, "AkMusicEngine.lib")
//#pragma comment(lib, "CommunicationCetral.lib")

namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}

	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
		)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
		)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

AkGameObjectID m_LastGameObjectId; //note: only > 0
std::vector<AkGameObjectID> m_FreeObjectIDs;

AkGameObjectID m_DefaultSpeakerId;
std::unordered_map<std::string, AkGameObjectID> m_NamedSpeakers;
std::unordered_map<const C3DElement*, AkGameObjectID> m_GameObjectSpeakers;

ISoundManager* ISoundManager::InstantiateSoundManager()
{
	return new CSoundManager();
}

CSoundManager::CSoundManager()
	: m_SoundBanksFilename("")
	, m_SpeakersFilename("")
	, m_InitOk(false)
{
	//CHECK	
} 

CSoundManager::~CSoundManager()
{
	//Terminate();
}


void CSoundManager::PlayEvent(const SoundEvent &_event)
{
	if (!m_InitOk) return;

	PlayEvent(_event, m_DefaultSpeakerId);
}
void CSoundManager::PlayEvent(const SoundEvent &_event, const std::string &_speaker)
{
	if (!m_InitOk) return;

	auto it = m_NamedSpeakers.find(_speaker);
	if (it != m_NamedSpeakers.end())
	{
		PlayEvent(_event, it->second);
	}
	else
	{
		assert(false);
	}
}
void CSoundManager::PlayEvent(const SoundEvent &_event, const C3DElement *_speaker)
{
	if (!m_InitOk) return;

	auto it = m_GameObjectSpeakers.find(_speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		PlayEvent(_event, it->second);
	}
	else
	{
		assert(false);
	}
}
void CSoundManager::PlayEvent(const SoundEvent &_event, const AkGameObjectID &ID)
{
	if (!m_InitOk) return;

	AK::SoundEngine::PostEvent(_event.m_EventName.c_str(), ID);
}

void CSoundManager::SetSwitch(const SoundSwitchValue &switchValue)
{
	if (!m_InitOk) return;

	SetSwitch(switchValue, m_DefaultSpeakerId);
}
void CSoundManager::SetSwitch(const SoundSwitchValue &switchValue, const std::string &_speaker)
{
	if (!m_InitOk) return;

	auto it = m_NamedSpeakers.find(_speaker);
	if (it != m_NamedSpeakers.end())
	{
		SetSwitch(switchValue, it->second);
	}
	else
	{
		assert(false);
	}
}
void CSoundManager::SetSwitch(const SoundSwitchValue &switchValue, const C3DElement *_speaker)
{
	if (!m_InitOk) return;

	auto it = m_GameObjectSpeakers.find(_speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		SetSwitch(switchValue, it->second);
	}
	else
	{
		assert(false);
	}
}
void CSoundManager::SetSwitch(const SoundSwitchValue &switchValue, const AkGameObjectID &ID)
{
	if (!m_InitOk) return;

	AKRESULT l_Result = AK::SoundEngine::SetSwitch(switchValue.m_SoundSwitch.m_SwitchName.c_str(), switchValue.valueName.c_str(), ID);
	assert(l_Result);
}

void CSoundManager::SetRTPCValue(const SoundRTPC &_rtpc, float value)
{
	if (!m_InitOk) return;

	SetRTPCValue(_rtpc, value, m_DefaultSpeakerId);
}
void CSoundManager::SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker)
{
	if (!m_InitOk) return;

	std::unordered_map<std::string, AkGameObjectID>::iterator it;
	it = m_NamedSpeakers.find(_speaker);
	if (it != m_NamedSpeakers.end())
	{
		SetRTPCValue(_rtpc, value, it->second);
	}
	else
	{
		assert(false);
	}
}
void CSoundManager::SetRTPCValue(const SoundRTPC &_rtpc, float value, const C3DElement*_speaker)
{
	if (!m_InitOk) return;

	std::unordered_map<const C3DElement*, AkGameObjectID>::iterator it;
	it = m_GameObjectSpeakers.find(_speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		SetRTPCValue(_rtpc, value, it->second);
	}
	else
	{
		assert(false);
	}
}
void CSoundManager::SetRTPCValue(const SoundRTPC &_rtpc, float value, const AkGameObjectID& ID)
{
	if (!m_InitOk) return;

	AKRESULT l_Result = AK::SoundEngine::SetRTPCValue(_rtpc.RTPCName.c_str(), AkRtpcValue(value), ID);
	assert (l_Result);
}


void CSoundManager::BroadcastRTPCValue(const SoundRTPC& Rtpc, float value)
{
	if (!m_InitOk) return;

	AKRESULT l_Result = AK::SoundEngine::SetRTPCValue(Rtpc.RTPCName.c_str(), AkRtpcValue(value)); 
	assert (l_Result);
}
void CSoundManager::BroadcastState(const SoundStateValue &_state)
{
	if (!m_InitOk) return;

	AKRESULT l_Result = AK::SoundEngine::SetState(_state.SoundState.c_str(), _state.SSvalueName.c_str());
	assert (l_Result);
}




AkGameObjectID CSoundManager::GenerateObjectID()
{
	if (!m_InitOk) return false;

	AkGameObjectID result;
	if (m_FreeObjectIDs.size() > 0)
	{
		result = m_FreeObjectIDs.back();
		m_FreeObjectIDs.pop_back();
	}
	else
	{
		result = ++m_LastGameObjectId;
	}
	return result;
}

bool CSoundManager::LoadSoundBanksXML(std::string filename)
{
	if (!m_InitOk) return false;


	CXMLTreeNode l_XML;

	if (l_XML.LoadFile((filename).c_str()))
	{
		CXMLTreeNode l_SoundbanksInfo = l_XML["SoundBanksInfo"];

			for (int i = 0; i < l_SoundbanksInfo.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Soundbanks = l_SoundbanksInfo(i);

				if (l_Soundbanks.GetName() == std::string("SoundBanks"))

				if (l_Soundbanks.Exists())
				{
					for (int i = 0; i < l_Soundbanks.GetNumChildren(); ++i)
					{
						CXMLTreeNode l_Element = l_Soundbanks(i);
						std::string test = l_Element.GetName();
						if (l_Element.GetName() == std::string("SoundBank"))
						{
							int l_SoundBankID = l_Element.GetIntProperty("Id", 0);
							if (l_SoundBankID != 1355168291) //No es el init.bnk
							{
								for (int j = 0; j < l_Element.GetNumChildren(); ++j)
								{
									CXMLTreeNode l_SoundBankElement = l_Element(j);

									if (l_SoundBankElement.GetName() == std::string("Path"))
									{

										std::string l_Bank = l_SoundBankElement.GetPszKeyword("Path", "");
										LoadSoundBank("Data\\Sound\\Soundbanks\\" + l_Bank);

									}
								}
							}
						}
					}
				}
			}
	}

	return true;
	
}

void CSoundManager::LoadSpeakers(std::string l_Name, Vect3f l_Position, Vect3f l_Orientation)
{
	AkSoundPosition l_SoundPosition = {};

	l_SoundPosition.Position.X = l_Position.x;
	l_SoundPosition.Position.Y = l_Position.y;
	l_SoundPosition.Position.Z = l_Position.z;

	l_SoundPosition.Orientation.X = l_Orientation.x;
	l_SoundPosition.Orientation.Y = l_Orientation.y;
	l_SoundPosition.Orientation.Z = l_Orientation.z;

	AkGameObjectID id = GenerateObjectID();
	m_NamedSpeakers[l_Name] = id;
	AK::SoundEngine::RegisterGameObj(id);
	AK::SoundEngine::SetPosition(id, l_SoundPosition);
}

bool CSoundManager::LoadSpeakersXML(std::string filename)
{
	if (!m_InitOk) return false;


	CXMLTreeNode l_Speakers;
	if (l_Speakers.LoadFile(filename.c_str()))
	{
		int count = l_Speakers.GetNumChildren();

		for (int i = 0; i < count; ++i)
		{
			std::string l_Name = l_Speakers(i).GetPszProperty("name", "");
			Vect3f l_Position = l_Speakers(i).GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f));
			Vect3f l_Orientation = l_Speakers(i).GetVect3fProperty("orientation", Vect3f(0.0f, 0.0f, 0.0f));

			LoadSpeakers(l_Name, l_Position, l_Orientation);
			

		}
		return true;
	}
	else
		return false;
}

bool CSoundManager::Init()
{
	//Initialize Audio Engine
	//Memory
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	//Streaming
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	AkInitSettings l_initSettings;
	AkPlatformInitSettings l_platInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(l_initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(l_platInitSettings);

	//Setting pool sizes for this game
	//This sizes are tuned for this game, each game should have its own optimal values
	l_initSettings.uDefaultPoolSize = 16 * 1024 * 1024;
	l_initSettings.uMaxNumPaths = 16;
	l_initSettings.uMaxNumTransitions = 128;

	l_platInitSettings.uLEngineDefaultPoolSize = 16 * 1024 * 1024; //If the value is higher than udefaultPoolSize crasher without saying why.

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	//AKRESULT eResult = AK::SoundEngine::Init(&l_InitSettings, &l_platInitSetings);

	AKRESULT eResult = AK::SOUNDENGINE_DLL::Init(
		&memSettings, 
		&stmSettings,
		&deviceSettings,
		&l_initSettings,
		&l_platInitSettings,
		&musicInit);

	if (eResult != AK_Success)
	{
		//Then we will run the game without sound
		AK::SOUNDENGINE_DLL::Term();
		//LOGGER->AddNewLog(ELL_ERROR, "CSoundmanager::Init->No se ha podido inicializar el motor de audio.");
		//throw CException (__FILE__, __LINE__, "CSoundmanager::Init->No se ha podido inicializar el motor de audio.");
		return false;
	}

	m_DefaultSpeakerId = GenerateObjectID();
	AK::SoundEngine::RegisterGameObj(m_DefaultSpeakerId);

	m_InitOk = true;

	return true;
}

bool CSoundManager::Load(const std::string &soundbanks_filename, const std::string &speakers_filename)
{
	if (!m_InitOk) return false;

	std::string m_SoundBanksFilename = soundbanks_filename;
	std::string m_SpeakersFilename = speakers_filename;

	bool l_IsOK = true;
	l_IsOK = LoadSoundBanksXML("Data\\Sound\\Soundbanks\\SoundbanksInfo.xml");
	l_IsOK &= LoadSpeakersXML("Data\\speakers.xml");

	/*const SoundEvent &_event
	PlayEvent();*/
	return l_IsOK;
}

bool CSoundManager::Reload()
{
	if (!m_InitOk) return false;

	Clean();
	return Load(m_SoundBanksFilename, m_SpeakersFilename);
}

bool CSoundManager::initBanks()
{
	if (!m_InitOk) return false;

	//load initialization and main soundbanks
	AkOSChar *path;
	AKRESULT retValue;
	//std::string m_Path;
	CONVERT_CHAR_TO_OSCHAR(m_Path.c_str(), path);
	retValue = AK::SOUNDENGINE_DLL::SetBasePath(path);
	retValue = AK::StreamMgr::SetCurrentLanguage(L"English(US)");

	AkBankID bankID;
	retValue = AK::SoundEngine::LoadBank("Data\\Sound\\Soundbanks\\Init.bnk", AK_DEFAULT_POOL_ID, bankID);
	if (retValue != AK_Success)
	{
		return false;
	}
	return true;
}

bool CSoundManager::LoadSoundBank(const std::string &bank)
{
	AkBankID bankID;
	AKRESULT retValue;

	retValue = AK::SoundEngine::LoadBank(bank.c_str(), AK_DEFAULT_POOL_ID, bankID);

	if (retValue != AK_Success)
	{
		return false;
	}
	return true;
}

bool CSoundManager::UnloadSoundBank(const std::string &bank)
{
	AkBankID bankID;
	AKRESULT retValue;

	retValue = AK::SoundEngine::UnloadBank(bank.c_str(), nullptr);
	if (retValue != AK_Success)
	{
		return false;
	}
	return true;

}


void CSoundManager::RegisterSpeaker(const C3DElement* _speaker)
{
	if (!m_InitOk) return;

	assert(m_GameObjectSpeakers.find(_speaker) == m_GameObjectSpeakers.end());

	AkGameObjectID id = GenerateObjectID();
	m_GameObjectSpeakers[_speaker] = id;

	Vect3f l_Position = _speaker->GetPosition();
	float l_Yaw = _speaker->GetYaw();
	float l_Pitch = _speaker->GetPitch();

	Vect3f l_Orientation(cos(l_Yaw) * cos(l_Pitch), sin(l_Pitch), sin(l_Yaw) * cos(l_Pitch));

	AkSoundPosition l_SoundPosition = {};

	l_SoundPosition.Position.X = l_Position.x;
	l_SoundPosition.Position.Y = l_Position.y;
	l_SoundPosition.Position.Z = l_Position.z;

	l_SoundPosition.Orientation.X = l_Orientation.x;
	l_SoundPosition.Orientation.Y = l_Orientation.y;
	l_SoundPosition.Orientation.Z = l_Orientation.z;

	AK::SoundEngine::RegisterGameObj(id);
	AK::SoundEngine::SetPosition(id, l_SoundPosition);

}

void CSoundManager::UnregisterSpeaker(const C3DElement* _speaker)
{
	if (!m_InitOk) return;

	auto it = m_GameObjectSpeakers.find(_speaker);
	if (it != m_GameObjectSpeakers.end())
	{
		AK::SoundEngine::UnregisterGameObj(it->second);

		m_GameObjectSpeakers.erase(it);
	}
	else
	{
		assert(false);
	}
}

void CSoundManager::Terminate()
{
	if (!m_InitOk) return;

	AK::SoundEngine::ClearBanks(); //Limpiamos cualquier bank
	AK::SoundEngine::UnregisterAllGameObj();

	AK::SOUNDENGINE_DLL::Term();
}

void CSoundManager::Clean()
{
	if (!m_InitOk) return;

	//Una vez hecho el clear hay que volver a llamar al bank init (no se si aqui on terminate)
	//TODO
	AK::SoundEngine::ClearBanks(); //Limpiamos cualquier bank

	for (auto it : m_NamedSpeakers)
	{
		AK::SoundEngine::UnregisterGameObj(it.second);
		m_FreeObjectIDs.push_back(it.second);
	}
	m_NamedSpeakers.clear();
}

void CSoundManager::SetListenerPosition(const CCamera *camera)
{
	if (!m_InitOk) return;

	Vect3f l_Position = camera->GetPosition();
	Vect3f l_Orientation = camera->GetLookAt();
	Vect3f l_VectorUp = camera->GetUp();

	AkListenerPosition l_ListenerPosition = {};

	l_ListenerPosition.Position.X = l_Position.x;
	l_ListenerPosition.Position.Y = l_Position.y;
	l_ListenerPosition.Position.Z = l_Position.z;

	l_ListenerPosition.OrientationFront.X = l_Orientation.x;
	l_ListenerPosition.OrientationFront.Y = l_Orientation.y;
	l_ListenerPosition.OrientationFront.Z = l_Orientation.z;

	l_ListenerPosition.OrientationTop.X = l_VectorUp.x;
	l_ListenerPosition.OrientationTop.Y = l_VectorUp.y;
	l_ListenerPosition.OrientationTop.Z = l_VectorUp.z;

	AK::SoundEngine::SetListenerPosition(l_ListenerPosition);
}

void CSoundManager::LaunchSoundEventDefaultSpeaker(std::string _string)
{ 
	if (!m_InitOk) return;

	SoundEvent _event = SoundEvent(_string);
	PlayEvent(_event);
		
}

void CSoundManager::LaunchSoundEventXMLSpeaker(std::string _string, std::string &_speaker)
{
	if (!m_InitOk) return;

	SoundEvent _event = SoundEvent(_string);
	PlayEvent(_event, _speaker);

}

void CSoundManager::LaunchSoundEventDynamicSpeaker(std::string _string, C3DElement *_dynamicspeaker)
{
	if (!m_InitOk) return;

	SoundEvent _event = SoundEvent(_string);
	PlayEvent(_event, _dynamicspeaker);
}

void CSoundManager::SetVolume(std::string _string, float value)
{
	if (!m_InitOk) return;

	SoundRTPC _rtpc;
	_rtpc.RTPCName = _string;
	SetRTPCValue(_rtpc, value);
}


void CSoundManager::Update(const CCamera *camera)
{
	if (!m_InitOk) return;

	for (auto it : m_GameObjectSpeakers)
	{
		Vect3f l_Position = it.first->GetPosition();
		float l_Yaw = it.first->GetYaw();
		float l_Pitch = it.first->GetPitch();

		Vect3f l_Orientation(cos(l_Yaw) * cos(l_Pitch), sin(l_Pitch), sin(l_Yaw) * cos(l_Pitch));

		AkSoundPosition l_SoundPosition = {}; 

		l_SoundPosition.Position.X = l_Position.x;
		l_SoundPosition.Position.Y = l_Position.y;
		l_SoundPosition.Position.Z = l_Position.z;

		l_SoundPosition.Orientation.X = l_Orientation.x;
		l_SoundPosition.Orientation.Y = l_Orientation.y;
		l_SoundPosition.Orientation.Z = l_Orientation.z;

		AK::SoundEngine::SetPosition(it.second, l_SoundPosition);
	}

	SetListenerPosition(camera);	

	//Actualiza WWISE
	AK::SOUNDENGINE_DLL::Tick();

}

void CSoundManager::InitAll(const std::string &soundbanks_filename, const std::string &speakers_filename)
{
	bool init = Init();
	bool initbanks = initBanks();
	bool load = Load(soundbanks_filename, speakers_filename);

}

