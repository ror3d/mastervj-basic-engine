#include "MaterialManager.h"
#include "MaterialParameter.h"

#include "Effect/EffectManager.h"

CMaterialManager::CMaterialManager()
	: m_Filename("")
{
}


CMaterialManager::~CMaterialManager()
{
}


void CMaterialManager::load(const std::string &Filename)
{
	m_Filename = Filename;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Materials = l_XML["materials"];
		if (l_Materials.Exists())
		{
			for (int i = 0; i < l_Materials.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Material = l_Materials(i);

				if (l_Material.GetName() == std::string("material"))
				{
					CMaterial * Material = new CMaterial(l_Material);
					add(Material->getName() , Material);
					void *nextDir = &CEffectManager::m_MaterialEffectParameters.m_RawData[0];
					//--------------------PARAMETERS
					for (int child = 0; child < l_Material.GetNumChildren(); child++){
						CXMLTreeNode l_param = l_Material(child);						
						if (l_param.GetName() == std::string("parameter")){
							
							CMaterialParameter::TMaterialType type;
							if (l_param.GetPszProperty("type") == std::string("float"))
							{
								type = CMaterialParameter::TMaterialType::FLOAT;
								CTemplatedMaterialParameter<float> *param = new CTemplatedMaterialParameter<float>(
									l_param,
									nextDir,
									l_param.GetFloatProperty("value"),
									type);
								Material->getParameters()->push_back(param);
							}
							else if (l_param.GetPszProperty("type") == std::string("Vect2f"))
							{
								type = CMaterialParameter::TMaterialType::VECT2F;
								CTemplatedMaterialParameter<Vect2f> *param = new CTemplatedMaterialParameter<Vect2f>(
									l_param,
									nextDir,
									l_param.GetFloatProperty("value"),
									type);
								Material->getParameters()->push_back(param);
							}
							else if (l_param.GetPszProperty("type") == std::string("Vect3f"))
							{
								type = CMaterialParameter::TMaterialType::VECT3F;
								CTemplatedMaterialParameter<Vect3f> *param = new CTemplatedMaterialParameter<Vect3f>(
									l_param,
									nextDir,
									l_param.GetFloatProperty("value"),
									type);
								Material->getParameters()->push_back(param);
							}
							else if (l_param.GetPszProperty("type") == std::string("Vect4f"))
							{
								type = CMaterialParameter::TMaterialType::VECT4F;
								CTemplatedMaterialParameter<Vect4f> *param = new CTemplatedMaterialParameter<Vect4f>(
									l_param,
									nextDir,
									l_param.GetFloatProperty("value"),
									type);
								Material->getParameters()->push_back(param);
							}

							nextDir = reinterpret_cast<unsigned char*>(nextDir)+sizeof(Vect4f);
						}						
					}					
				}
			}
		}
	}
}


void CMaterialManager::reload()
{
	destroy();
	load(m_Filename);
}