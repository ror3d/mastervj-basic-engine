#pragma once

#include <Utils/TMapManager.h>

class CAnimatedCoreModel;

class CAnimatedModelManager : public TMapManager<CAnimatedCoreModel>
{
private:
	std::string m_Filename;
public:
	CAnimatedModelManager();
	virtual ~CAnimatedModelManager();
	void Load(const std::string &Filename);
	void Reload();
};
