#ifndef BAR_HELPER_H
#define BAR_HELPER_H

#include "Debug/DebugHelper.h"
#include <Graphics/Particles/ParticleSystemInstance.h>
#include <Graphics/Particles/ParticleSystemManager.h>

class CBarHelper
{
public:
	static void CreateParticleParametersBar(CParticleSystemClass * particle, CParticleSystemInstance * particleInstance);
	static void AddFloatParameter(CDebugHelper::SDebugBar &bar, std::string name, float *value, std::string params);
	static void AddIntParameter(CDebugHelper::SDebugBar &bar, std::string name, int *value, std::string params);
	static void AddBoolParameter(CDebugHelper::SDebugBar &bar, std::string name, bool *value);
	static void AddSeparator(CDebugHelper::SDebugBar &bar);
	static void AddLabel(CDebugHelper::SDebugBar &bar, std::string name);
	static void AddButtonTexture(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, CParticleSystemClass *value);
	static void AddColor(CDebugHelper::SDebugBar &bar, std::string name, CColor *value);
	static void AddButtonFloat(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, range<float> *value);
	static void AddButtonColor(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, range<CColor> *value);
	static void AddButtonVect3f(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, range<Vect3f> *value);
	static void AddRangeFloat(CDebugHelper::SDebugBar &bar, std::string name, range<float> *value, std::string params);
	static void AddRangeColor(CDebugHelper::SDebugBar &bar, std::string name, range<CColor> *value, std::string params);
	static void AddRangeVect3f(CDebugHelper::SDebugBar &bar, std::string name, range<Vect3f> *value, std::string params);
	static void AddCloseBar(CDebugHelper::SDebugBar &bar);
	static void CreateMainBar();
	static void ReloadBars();
};

#endif