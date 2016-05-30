#include "MaterialManager.h"

#include "Effect/EffectManager.h"

#include "Material.h"

#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>
#include "Renderable/RenderableObjectTechnique.h"
#include "Texture/Texture.h"
#include "MaterialParameter.h"
#include <Windows.h>

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


void CMaterialManager::writeFile()
{
	CXMLTreeNode NewXML;

	if (NewXML.StartNewFile("Data\\materials2.xml"))
	{
		NewXML.StartElement("materials");

		for (auto mat : m_resources)
		{
			CMaterial * material = mat.second;

			NewXML.StartElement("material");

			NewXML.WritePszProperty("name", material->getName().c_str());
			NewXML.WritePszProperty("vertex_type", material->getRenderableObjectTechique()->getName().c_str());

			NewXML.StartElement("texture");
			NewXML.WriteIntProperty("stage", 0);
			NewXML.WritePszProperty("filename", material->GetTextureAtStage(0)->getName().c_str());
			NewXML.EndElement();
			
			for (auto it = material->getParameters()->begin(); it != material->getParameters()->end(); ++it)
			{

				NewXML.StartElement("parameter");

				CMaterialParameter::TMaterialType matType = (*it)->getMaterialType();
				if (matType == CMaterialParameter::TMaterialType::FLOAT)
				{
					CTemplatedMaterialParameter<float> *param = (CTemplatedMaterialParameter<float>*)(*it);
					NewXML.WritePszProperty("type", "float");
					NewXML.WritePszProperty("name", (*it)->getName().c_str());
					NewXML.WriteFloatProperty("value", *param->getValue());
					NewXML.WritePszProperty("varParams", (*it)->getParamValues().c_str());
				}
				else if (matType == CMaterialParameter::TMaterialType::VECT2F)
				{
					CTemplatedMaterialParameter<Vect2f> *vect2f = (CTemplatedMaterialParameter<Vect2f>*)(*it);
					NewXML.WritePszProperty("type", "Vect2f");
					NewXML.WritePszProperty("name", (*it)->getName().c_str());
					NewXML.WriteVect2fProperty("value", *vect2f->getValue());
					NewXML.WritePszProperty("varParams", (*it)->getParamValues().c_str());
				}
				else if (matType == CMaterialParameter::TMaterialType::VECT3F)
				{
					CTemplatedMaterialParameter<Vect3f> *vect3f = (CTemplatedMaterialParameter<Vect3f>*)(*it);
					NewXML.WritePszProperty("type", "Vect3f");
					NewXML.WritePszProperty("name", (*it)->getName().c_str());
					NewXML.WriteVect3fProperty("value", *vect3f->getValue());
					NewXML.WritePszProperty("varParams", (*it)->getParamValues().c_str());
				}
				else if (matType == CMaterialParameter::TMaterialType::VECT4F)
				{
					CTemplatedMaterialParameter<Vect4f> *vect4f = (CTemplatedMaterialParameter<Vect4f>*)(*it);
					NewXML.WritePszProperty("type", "Vect4f");
					NewXML.WritePszProperty("name", (*it)->getName().c_str());
					NewXML.WriteVect4fProperty("value", *vect4f->getValue());
					NewXML.WritePszProperty("varParams", (*it)->getParamValues().c_str());
				}

				NewXML.EndElement();
			}

			NewXML.EndElement();
		}

		NewXML.EndElement();
		NewXML.EndNewFile();
	}

	DeleteFile("Data\\materials.xml.bkp");
	rename("Data\\materials.xml", "Data\\materials.xml.bkp");
	rename("Data\\materials2.xml", "Data\\materials.xml");
}
