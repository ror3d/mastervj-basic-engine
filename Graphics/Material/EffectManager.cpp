#include "EffectManager.h"
#include "EffectTechnique.h"

#include <XML/XMLTreeNode.h>


CEffectManager::CEffectManager()
{
}


CEffectManager::~CEffectManager()
{
}


void CEffectManager::Reload()
{
}


void CEffectManager::Load(const std::string &Filename)
{
	CXMLTreeNode effects;
	effects.LoadFile(Filename.c_str());
}


CEffectVertexShader * CEffectManager::GetVertexShader(const std::string &VertexShader)
{
	return nullptr;
}


CEffectPixelShader * CEffectManager::GetPixelShader(const std::string &PixelShader)
{
	return nullptr;
}
