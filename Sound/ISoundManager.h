#ifndef _ISOUNDMANAGER_H
#define _ISOUNDMANAGER_H
#pragma once

#include <string>
//#include <Graphics/Renderer/3DElement.h>
#include <AK/SoundEngine/Common/AkTypes.h>

class CCamera;
class C3DElement;

struct SoundEvent
{
	std::string m_EventName;
	SoundEvent(const std::string EventName) :m_EventName(EventName){};
};
struct SoundSwitch
{
	std::string m_SwitchName;
};
struct SoundSwitchValue
{
	SoundSwitch m_SoundSwitch;
	std::string valueName;
};
struct SoundRTPC
{
	std::string RTPCName;
};
struct SoundStateValue
{
	std::string SSvalueName;
};

class ISoundManager
{
public:
	static ISoundManager* InstantiateSoundManager();
	virtual ~ISoundManager() {}

	void SetPath(const std::string& path){ m_Path = path; }

	virtual bool Init() = 0;
	virtual void Update(const CCamera *Camera) = 0;
	virtual bool Load(const std::string& SoundBanksFilename, const std::string& SpeakersFilename) = 0;
	virtual bool Reload() = 0;
	//std::string m_Path;

	virtual bool LoadSoundBank(const std::string &bank) = 0;
	virtual bool UnloadSoundBank(const std::string &bank) = 0;

	virtual void RegisterSpeaker(const C3DElement* _speaker) = 0;
	virtual void UnregisterSpeaker(const C3DElement* _speaker) = 0;

	virtual void PlayEvent(const SoundEvent &_event) = 0;
	virtual void PlayEvent(const SoundEvent &_event, const std::string &_speaker) = 0;
	virtual void PlayEvent(const SoundEvent &_event, const C3DElement *_speaker) = 0;
	virtual void PlayEvent(const SoundEvent &_event, const AkGameObjectID &ID) = 0;

	virtual void SetSwitch(const SoundSwitchValue& switchValue) = 0;
	virtual void SetSwitch(const SoundSwitchValue& switchValue, const std::string &_speaker) = 0;
	virtual void SetSwitch(const SoundSwitchValue& switchValue, const C3DElement *_speaker) = 0;
	virtual void SetSwitch(const SoundSwitchValue &switchValue, const AkGameObjectID &ID) = 0;

	virtual void BroadcastRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const C3DElement *_speaker) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const AkGameObjectID &ID) = 0;

	virtual void BroadcastState(const SoundStateValue &State) = 0;

protected:
	std::string m_Path;
};
	
#endif