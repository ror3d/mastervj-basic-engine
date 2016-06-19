#ifndef SPEAKER_COMPONENT_H
#define SPEAKER_COMPONENT_H

#include "Component.h"

class CSpeakerComponent : public CComponent
{

protected:

	virtual void Init();

public:
	CSpeakerComponent(const std::string& name, CXMLTreeNode& node, CElement* Owner);
	CSpeakerComponent(const std::string& name, CElement* Owner);
	virtual ~CSpeakerComponent();

	virtual void Update(float ElapsedTime);

	virtual void Destroy();

	void Play( const std::string audioClipName, bool loop );
	void Stop();
};

#endif
