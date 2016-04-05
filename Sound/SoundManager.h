#pragma once


#include <string>
#include <vector>
#include <unordered_map>
#include "ISoundManager.h"
#include <Graphics/Camera/Camera.h>
#include <Graphics/Renderer/3DElement.h>

class SoundManager
{

public:
	void SetPath(const std::string &path) { m_Path = path; }

	virtual bool Init() = 0;

	virtual void Update(const CCamera *camera) = 0;

	virtual void Load(const std::string &soundbanks_filename, const std::string &speakers_filename) = 0;

	virtual bool Reload() = 0;

	virtual bool LoadSoundBank(const std::string &bank) = 0;
	virtual bool UnloadSoundBank(const std::string &bank) = 0;

	virtual void RegisterSpeaker(const C3DElement* _speaker) = 0;
	virtual void UnregisterSpeaker(const C3DElement*  _speaker) = 0;

	bool LoadSoundBanksXML(std::string filename);
	bool LoadSpeakersXML(std::string filename);

	virtual void PlayEvent(const SoundEvent &_event) = 0;
	virtual void PlayEvent(const SoundEvent &_event, const std::string &_speaker) = 0;
	virtual void PlayEvent(const SoundEvent &_event, const C3DElement*  _speaker) = 0;

	virtual void SetSwitch(const SoundSwitchValue &switchvalue) = 0;
	virtual void SetSwitch(const SoundSwitchValue &switchvalue, const std::string &_speaker) = 0;
	virtual void SetSwitch(const SoundSwitchValue &switchvalue, const C3DElement*  _speaker) = 0;

	virtual void BroadcastRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const std::string &_speaker) = 0;
	virtual void SetRTPCValue(const SoundRTPC &_rtpc, float value, const C3DElement*  _speaker) = 0;

	virtual void BroadcastState(const SoundStateValue &_state) = 0;
};