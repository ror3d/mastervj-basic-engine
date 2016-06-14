#include <Windows.h>
#include "AntTweakBar.h"
#include <string>
#include "Engine/Engine.h"

#include "Debug/BarHelper.h"
#include "Debug/BarCalls.h"
#include <Core/Input/InputManagerImplementation.h>
#include <Graphics/Material/MaterialParameter.h>
#include <Graphics/Camera/FPSCameraController.h>
#include <Graphics/Camera/CameraManager.h>
#include <Graphics/Scene/SceneRendererCommandManager.h>
#include <Base/Scripting/ScriptManager.h>
#include <Graphics/Context/ContextManager.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Material/Material.h>
#include <Graphics/Renderable/RenderableObjectTechniqueManager.h>
#include <Graphics/Effect/EffectManager.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Layer/LayerManager.h>
#include <Graphics/Renderable/RenderableObjectsManager.h>
#include <Graphics/Renderable/RenderableObject.h>
#include <Base\Math\Color.h>
#include <vector>
#include <tchar.h>

void CBarHelper::AddFloatParameter(CDebugHelper::SDebugBar &bar, std::string name, float *value, std::string params)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::FLOAT;
	var.mode = CDebugHelper::READ_WRITE;
	var.pFloat = value;
	var.params = params;

	bar.variables.push_back(var);
}

void CBarHelper::AddIntParameter(CDebugHelper::SDebugBar &bar, std::string name, int *value, std::string params)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::INT;
	var.mode = CDebugHelper::READ_WRITE;
	var.pInt = value;
	var.params = params;

	bar.variables.push_back(var);
}

void CBarHelper::AddBoolParameter(CDebugHelper::SDebugBar &bar, std::string name, bool *value, CDebugHelper::Mode mode = CDebugHelper::READ_WRITE)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BOOL;
	var.mode = mode;
	var.pBool = value;

	bar.variables.push_back(var);
}

void CBarHelper::AddSeparator(CDebugHelper::SDebugBar &bar)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = "Separator";
	var.mode = CDebugHelper::SEPARATOR;
	var.type = CDebugHelper::NONE;

	bar.variables.push_back(var);
}

void CBarHelper::AddLabel(CDebugHelper::SDebugBar &bar, std::string name)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = NULL;

	bar.variables.push_back(var);
}

void CBarHelper::AddButtonParticle(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, CParticleSystemClass *value)
{
	CDebugHelper::SDebugVariable var = {};
	
	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = callback;
	var.ptr = value;

	bar.variables.push_back(var);
}

void CBarHelper::AddButtonFloat(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, range<float> *value)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = callback;
	var.ptr = value;

	bar.variables.push_back(var);
}

void CBarHelper::AddButtonColor(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, range<CColorSpace> *value)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = callback;
	var.ptr = value;

	bar.variables.push_back(var);
}

void CBarHelper::AddButtonVect3f(CDebugHelper::SDebugBar &bar, std::string name, CDebugHelper::Callback callback, range<Vect3f> *value)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = name;
	var.type = CDebugHelper::BUTTON;
	var.callback = callback;
	var.ptr = value;

	bar.variables.push_back(var);
}

void CBarHelper::AddRangeFloat(CDebugHelper::SDebugBar &bar, std::string name, range<float> *value, std::string params)
{
	AddButtonFloat(bar, "E " + name, CBarCalls::EqualVarsFloat, value);
	AddFloatParameter(bar, "L " + name, &value->first, params);
	AddFloatParameter(bar, "U " + name, &value->second, params);
}

void CBarHelper::AddRangeColor(CDebugHelper::SDebugBar &bar, std::string name, range<CColorSpace> *value, std::string params)
{
	AddButtonColor(bar, "E " + name, CBarCalls::EqualVarsColor, value);

	AddFloatParameter(bar, "L " + name + " X", &value->first.x, params + " group='Lower " + name + "'");
	AddFloatParameter(bar, "L " + name + " Y", &value->first.y, params + " group='Lower " + name + "'");
	AddFloatParameter(bar, "L " + name + " Z", &value->first.z, params + " group='Lower " + name + "'");
	AddFloatParameter(bar, "L " + name + " W", &value->first.w, params + " group='Lower " + name + "'");

	AddFloatParameter(bar, "U " + name + " X", &value->second.x, params + " group='Upper " + name + "'");
	AddFloatParameter(bar, "U " + name + " Y", &value->second.y, params + " group='Upper " + name + "'");
	AddFloatParameter(bar, "U " + name + " Z", &value->second.z, params + " group='Upper " + name + "'");
	AddFloatParameter(bar, "U " + name + " W", &value->second.w, params + " group='Upper " + name + "'");
}

void CBarHelper::AddRangeVect3f(CDebugHelper::SDebugBar &bar, std::string name, range<Vect3f> *value, std::string params = "")
{
	AddButtonVect3f(bar, "E " + name, CBarCalls::EqualVarsVect3f, value);

	AddFloatParameter(bar, "L " + name + " X", &value->first.x, params + " group='Lower " + name + "'");
	AddFloatParameter(bar, "L " + name + " Y", &value->first.y, params + " group='Lower " + name + "'");
	AddFloatParameter(bar, "L " + name + " Z", &value->first.z, params + " group='Lower " + name + "'");

	AddFloatParameter(bar, "U " + name + " X", &value->second.x, params + " group='Upper " + name + "'");
	AddFloatParameter(bar, "U " + name + " Y", &value->second.y, params + " group='Upper " + name + "'");
	AddFloatParameter(bar, "U " + name + " Z", &value->second.z, params + " group='Upper " + name + "'");
}

void CBarHelper::AddCloseBar(CDebugHelper::SDebugBar &bar)
{
	CDebugHelper::SDebugVariable var = {};

	var.name = "Close bar";
	var.type = CDebugHelper::BUTTON;
	var.callback = CBarCalls::RemoveBar;
	std::vector<std::string>* nameVector = new std::vector<std::string>();
	nameVector->push_back(bar.name);
	var.ptr = nameVector;

	bar.variables.push_back(var);
}

void CBarHelper::CreateParticleParametersBar(CParticleSystemClass * particle, CParticleSystemInstance * particleInstance)
{
	std::vector<CMaterialParameter*> * params = particle->material->getParameters();
	CDebugHelper::SDebugBar bar;
	CDebugHelper::SDebugVariable var = {};

	particleInstance->setParticleClass(particle);
	bar.name = particle->getName();

	AddCloseBar(bar);

	var = {};

	var.name = "Delete particle";
	var.type = CDebugHelper::BUTTON;
	var.callback = CBarCalls::DeleteParticle;
	var.ptr = particle;

	bar.variables.push_back(var);

	var = {};

	var.name = "Clone particle";
	var.type = CDebugHelper::BUTTON;
	var.callback = CBarCalls::CloneParticle;
	var.ptr = particle;

	bar.variables.push_back(var);

	AddButtonParticle(bar, "Load texture", CBarCalls::LoadTexture, particleInstance->getParticleClass());
	AddSeparator(bar);
	AddFloatParameter(bar, "Emit rate", &particle->emitRate, "min=1 max=20 step=1 precision=0");
	AddIntParameter(bar, "Num frames", &particle->numFrames, "min=1 max=30 step=1 precision=0");
	AddFloatParameter(bar, "Time per frame", &particle->timePerFrame, "min=0 max=10 step=1 precision=0");
	AddBoolParameter(bar, "LoopFrames", &particle->loopFrames);
	AddSeparator(bar);
	AddFloatParameter(bar, "Sprite X", &((CTemplatedMaterialParameter<Vect2f>*)params->at(0))->getValue()->x, params->at(0)->getParamValues() + " group='Sprite size'");
	AddFloatParameter(bar, "Sprite Y", &((CTemplatedMaterialParameter<Vect2f>*)params->at(0))->getValue()->y, params->at(0)->getParamValues() + " group='Sprite size'");
	AddFloatParameter(bar, "Lerp sprite", ((CTemplatedMaterialParameter<float>*)params->at(1))->getValue(), params->at(1)->getParamValues());
	AddFloatParameter(bar, "Ratio Y", ((CTemplatedMaterialParameter<float>*)params->at(2))->getValue(), params->at(2)->getParamValues());
	AddSeparator(bar);
	AddRangeFloat(bar, "size", &particle->size, "min=0 max=5 step=0.1 precision=1");
	AddSeparator(bar);
	AddRangeVect3f(bar, "velocity", &particle->startVelocity, "min=-50 max=50 step=0.1 precision=1");
	AddSeparator(bar);
	AddRangeVect3f(bar, "acceleration", &particle->acceleration, "min=-50 max=50 step=0.1 precision=1");
	AddSeparator(bar);
	AddRangeFloat(bar, "angle", &particle->startAngle, "min=0 max=360 step=0.5 precision=1");
	AddSeparator(bar);
	AddRangeFloat(bar, "angle speed", &particle->angleSpeed, "min=-50 max=50 step=0.1 precision=1");
	AddSeparator(bar);
	AddRangeFloat(bar, "angle acceleration", &particle->angleAcceleration, "min=-50 max=50 step=0.1 precision=1");
	AddSeparator(bar);
	AddRangeFloat(bar, "life", &particle->life, "min=1 max=5 step=1 precision=0");
	AddSeparator(bar);
	AddButtonParticle(bar, "toggle color space", CBarCalls::ChangeColorSpace, particle);
	AddBoolParameter(bar, "hsl", &particle->color.first.HSL);
	AddRangeColor(bar, "color", &particle->color, "min=0.0 max=1.0 step=0.01 precision=2");
	AddSeparator(bar);


	CDebugHelper::GetDebugHelper()->RegisterBar(bar);
}

void CBarHelper::CreateMainBar()
{
	CDebugHelper::SDebugBar bar;
	bar.name = "Particle editor";

	CDebugHelper::SDebugVariable var = {};
	var.name = "New particle";
	var.type = CDebugHelper::BUTTON;
	var.callback = CBarCalls::OpenParticleParameters;
	var.ptr = CEngine::GetSingleton().getParticleManager()->get("new_particle");
	bar.variables.push_back(var);

	var = {};

	var.name = "Particle list";
	var.type = CDebugHelper::BUTTON;
	var.callback = CBarCalls::OpenParticlesBar;
	var.ptr = CEngine::GetSingleton().getParticleManager()->getMap();

	bar.variables.push_back(var);

	var = {};

	var.name = "Save to file";
	var.type = CDebugHelper::BUTTON;
	var.callback = CBarCalls::SaveToFile;
	var.ptr = nullptr;

	bar.variables.push_back(var);

	CDebugHelper::GetDebugHelper()->RegisterBar(bar);
}

void CBarHelper::ReloadBars()
{
	std::vector<std::string>* nameVector = new std::vector<std::string>();

	auto map = CEngine::GetSingleton().getParticleManager()->getMap();

	for (auto it = map->begin(); it != map->end(); ++it)
	{
		nameVector->push_back(it->second->getName());
		CBarCalls::RemoveBar(static_cast<void*>(nameVector));
		nameVector->clear();
	}

	nameVector->clear();
	nameVector->push_back("Particle list");
	CBarCalls::RemoveBar(static_cast<void*>(nameVector));

	CBarCalls::OpenParticlesBar(map);
}

