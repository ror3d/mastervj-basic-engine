#include <Base/Utils/Utils.h>
#include "SoundManager.h"
#include "ISoundManager.h"

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
	void * VirtualFreeHook(
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

AkGameObjectID GenerateObjectID() 
{
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

bool LoadSoundBanksXML(std::string filename)
{
	//TODO
	
}

bool LoadSpeakersXML(std::string filename)
{

	CXMLTreeNode l_Speakers;
	if (l_Speakers.LoadFile(filename.c_str()))
	{
		int count = l_Speakers.GetNumChildren();

		for (int i = 0; i < count; ++i)
		{
			std::string l_Name = l_Speakers(i).GetPszProperty("name", "");
			Vect3f l_Position = l_Speakers(i).GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f));
			Vect3f l_Orientation = l_Speakers(i).GetVect3fProperty("orientation", Vect3f(0.0f, 0.0f, 0.0f));

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
		return true;
	}
	else
		return false;
}

bool Init()
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
	l_initSettings.uDefaultPoolSize = 512 * 1024 * 1024;
	l_initSettings.uMaxNumPaths = 16;
	l_initSettings.uMaxNumTransitions = 128;

	l_platInitSettings.uLEngineDefaultPoolSize = 512 * 1024 * 1024; //If the value is higher than udefaultPoolSize crasher without saying why.

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

	return true;
}

bool Load(const std::string &soundbanks_filename, const std::string &speakers_filename)
{
	std::string m_SoundBanksFilename = soundbanks_filename;
	std::string m_SpeakersFilename = speakers_filename;

	bool l_IsOK = true;
	l_IsOK = LoadSoundBanksXML("Data\\soundbanks.xml");//TODO
	l_IsOK &= LoadSpeakersXML("Data\\speakers.xml");//TODO

	return l_IsOK;
}

bool initBanks()
{
	//load initialization and main soundbanks
	AkOSChar *path;
	AKRESULT retValue;
	CONVERT_CHAR_TO_OSCHAR(m_Path.c_str(), path);
	retValue = AK::SOUNDENGINE_DLL::SetBasePath(path);
	retValue = AK::StreamMgr::SetCurrentLanguage(L"English(US)");

	AkBankID bankID;
	retValue = AK::SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
	if (retValue != AK_Success)
	{
		return false;
	}
	return true;
}

bool LoadSoundBank(const std::string &bank)
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

bool UnloadSoundBank(const std::string &bank)
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


void RegisterSpeaker(const C3DElement* _speaker)
{
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

void UnregisterSpeaker(const C3DElement* _speaker)
{
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

void Terminate()
{
	AK::SoundEngine::ClearBanks(); //Limpiamos cualquier bank
	AK::SoundEngine::UnregisterAllGameObj();

	AK::SOUNDENGINE_DLL::Term();
}

void Clean()
{
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

void SetListenerPosition(const CCamera *camera)
{
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

void Update(const CCamera *camera)
{
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

