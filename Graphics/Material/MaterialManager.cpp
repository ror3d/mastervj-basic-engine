#include "MaterialManager.h"

#include "XML/XMLTreeNode.h"


static CContextManager::ERasterizerState ParseRasterizerState(const std::string& str)
{
	if (str == "WIREFRAME")
	{
		return CContextManager::RS_WIREFRAME;
	}
	else if (str == "SOLID")
	{
		return CContextManager::RS_SOLID;
	}
	else
	{
		return (CContextManager::ERasterizerState)-1;
	}
};

static CContextManager::EDepthStencilState ParseDepthStencilState(const std::string& str)
{
	if (str == "DEPTH_ON")
	{
		return CContextManager::DSS_DEPTH_ON;
	}
	else if (str == "OFF")
	{
		return CContextManager::DSS_OFF;
	}
	else
	{
		return (CContextManager::EDepthStencilState) - 1;
	}
};

static CContextManager::EBlendState ParseBlendState(const std::string& str)
{
	if (str == "SOLID")
	{
		return CContextManager::BLEND_SOLID;
	}
	else if (str == "CLASSIC")
	{
		return CContextManager::BLEND_CLASSIC;
	}
	else if (str == "PREMULT")
	{
		return CContextManager::BLEND_PREMULT;
	}
	else
	{
		return (CContextManager::EBlendState) - 1;
	}
};

void CMaterialManager::AddMaterials(const std::string& path)
{
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(path.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["materials"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Material = l_Input(i);

				std::string name = l_Material.GetPszProperty("name");

				std::unordered_map<std::string, CMaterial*>::const_iterator it = m_Materials.find(name);
				if (it != m_Materials.end()) delete it->second;

				CContextManager::ERasterizerState l_RasterizerState = ParseRasterizerState(l_Material.GetPszProperty("rasterizer_state"));
				CContextManager::EDepthStencilState l_DepthStencilState = ParseDepthStencilState(l_Material.GetPszProperty("depth_stencil_state"));
				CContextManager::EBlendState l_BlendState = ParseBlendState(l_Material.GetPszProperty("blend_state"));

				CMaterial *material = new CMaterial(l_RasterizerState, l_DepthStencilState, l_BlendState);

				// TODO: parse debug size & debug color
				material->SetDebugSize(5);

				m_Materials[name] = material;
			}
		}
	}
}


CMaterialManager::~CMaterialManager()
{
	for (std::unordered_map<std::string, CMaterial*>::iterator it = m_Materials.begin(); it != m_Materials.end(); ++it)
	{
		delete it->second;
	}
}
