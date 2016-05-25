#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H
#pragma once

#include "ISoundManager.h"

//#include <string>
#include <vector>
#include <unordered_map>

class CSoundManager : public ISoundManager
{

private:
	std::string m_SoundBanksFilename;
	std::string m_SpeakersFilename;
	bool m_InitOk;

private:	
	AkGameObjectID GenerateObjectID();

public:
	int play = 0;
	CSoundManager();
	virtual ~CSoundManager();
	
	virtual bool Init();
	virtual void Update(const CCamera *camera);
	virtual bool Load(const std::string &soundbanks_filename, const std::string &speakers_filename);
	virtual bool Reload();

	bool LoadSoundBank(const std::string &bank);
	bool UnloadSoundBank(const std::string &bank);

	void RegisterSpeaker(const C3DElement* _speaker);
	void UnregisterSpeaker(const C3DElement*  _speaker);

	bool LoadSoundBanksXML(std::string filename);
	bool LoadSpeakersXML(std::string filename);

	void PlayEvent(const SoundEvent &_event);
	void PlayEvent(const SoundEvent &_event, const std::string &_speaker);
	void PlayEvent(const SoundEvent &_event, const C3DElement* _speaker);
	void PlayEvent(const SoundEvent &_event, const AkGameObjectID &ID);

	void SetSwitch(const SoundSwitchValue &switchvalue);
	void SetSwitch(const SoundSwitchValue &switchvalue, const std::string &_speaker);
	void SetSwitch(const SoundSwitchValue &switchvalue, const C3DElement*  _speaker);
	void SetSwitch(const SoundSwitchValue &switchValue, const AkGameObjectID &ID);

	void BroadcastRTPCValue(const SoundRTPC &_rtpc, float value);
	void SetRTPCValue(const SoundRTPC &_rtpc, float value);
	void SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker);
	void SetRTPCValue(const SoundRTPC &_rtpc, float value, const C3DElement*  _speaker);
	void SetRTPCValue(const SoundRTPC &_rtpc, float value, const AkGameObjectID &ID);

	void BroadcastState(const SoundStateValue &_state);

	//self
	bool initBanks();
	void Terminate();
	void destroy() { Terminate(); }
	void Clean();
	void SetListenerPosition(const CCamera *camera);
	void CSoundManager::LaunchSoundEventDefaultSpeaker(std::string _string);
	void CSoundManager::LaunchSoundEventXMLSpeaker(std::string _string, std::string &_speaker);
	void CSoundManager::LaunchSoundEventDynamicSpeaker(std::string _string, C3DElement *_dynamicspeaker);
	void CSoundManager::SetVolume(std::string _string, float value);
	
};
#endif