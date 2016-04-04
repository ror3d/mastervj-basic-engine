#ifndef ANIMATED_MODEL_MANAGER_H
#define ANIMATED_MODEL_MANAGER_H

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

#endif