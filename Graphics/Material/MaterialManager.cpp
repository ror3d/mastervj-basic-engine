#include "MaterialManager.h"

#include "Effect/EffectManager.h"

#include "Material.h"

#include <Core/Engine/Engine.h>
#include <Base/XML/XMLTreeNode.h>
#include "Renderable/RenderableObjectTechnique.h"
#include "Texture/Texture.h"
#include "MaterialParameter.h"

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
	FILE * file = fopen("Data\\materials2.xml", "ab+");

	fputs("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n", file);

	fputs("<materials>\n", file);

	for (auto mat : m_resources)
	{
		CMaterial * material = mat.second;

		std::string materialStr = "\t<material ";
		materialStr += "name=\"" + material->getName() + "\" ";
		materialStr += "vertex_type=\"" + material->getRenderableObjectTechique()->getName() + "\">\n";

		fputs(materialStr.c_str(), file);

		std::string textureStr = "\t\t<texture ";
		textureStr += "stage=\"0\" filename=\"" + material->GetTextureAtStage(0)->getName() + "\"/>\n";

		fputs(textureStr.c_str(), file);

		for (auto it = material->getParameters()->begin(); it != material->getParameters()->end(); ++it)
		{
			std::string paramterStr = "\t\t<parameter ";
			std::string type = "";

			CMaterialParameter::TMaterialType matType = (*it)->getMaterialType();
			if (matType == CMaterialParameter::TMaterialType::FLOAT)
			{
				type = "float";
				CTemplatedMaterialParameter<float> *param = (CTemplatedMaterialParameter<float>*)(*it);
				paramterStr += "type=\"" + type + "\" ";
				paramterStr += "name=\"" + (*it)->getName() + "\" ";
				paramterStr += "value=\"" + std::to_string(*param->getValue()) + "\" ";
				paramterStr += "varParams=\"" + (*it)->getParamValues() + "\"/>\n";
			}
			else if (matType == CMaterialParameter::TMaterialType::VECT2F)
			{
				type = "Vect2f";
				CTemplatedMaterialParameter<Vect2f> *vect2f = (CTemplatedMaterialParameter<Vect2f>*)(*it);
				paramterStr += "type=\"" + type + "\" ";
				paramterStr += "name=\"" + (*it)->getName() + "\" ";
				paramterStr += "value=\"" + std::to_string(vect2f->getValue()->x) + " ";
				paramterStr += std::to_string(vect2f->getValue()->y) + "\" ";
				paramterStr += "varParams=\"" + (*it)->getParamValues() + "\"/>\n";
			}
			else if (matType == CMaterialParameter::TMaterialType::VECT3F)
			{
				type = "Vect3f";
				CTemplatedMaterialParameter<Vect3f> *vect3f = (CTemplatedMaterialParameter<Vect3f>*)(*it);
				paramterStr += "type=\"" + type + "\" ";
				paramterStr += "name=\"" + (*it)->getName() + "\" ";
				paramterStr += "value=\"" + std::to_string(vect3f->getValue()->x) + " ";
				paramterStr += std::to_string(vect3f->getValue()->y) + " ";
				paramterStr += std::to_string(vect3f->getValue()->z) + "\" ";
				paramterStr += "varParams=\"" + (*it)->getParamValues() + "\"/>\n";
			}
			else if (matType == CMaterialParameter::TMaterialType::VECT4F)
			{
				type = "Vect4f";
				CTemplatedMaterialParameter<Vect4f> *vect4f = (CTemplatedMaterialParameter<Vect4f>*)(*it);
				paramterStr += "type=\"" + type + "\" ";
				paramterStr += "name=\"" + (*it)->getName() + "\" ";
				paramterStr += "value=\"" + std::to_string(vect4f->getValue()->x) + " ";
				paramterStr += std::to_string(vect4f->getValue()->y) + " ";
				paramterStr += std::to_string(vect4f->getValue()->z) + " ";
				paramterStr += std::to_string(vect4f->getValue()->w) + "\" ";
				paramterStr += "varParams=\"" + (*it)->getParamValues() + "\"/>\n";
			}

			fputs(paramterStr.c_str(), file);
		}

		fputs("\t</material>\n", file);
	}

	fputs("</materials>\n", file);

	fclose(file);

	DeleteFile("Data\\materials.xml.bkp");
	rename("Data\\materials.xml", "Data\\materials.xml.bkp");
	rename("Data\\materials2.xml", "Data\\materials.xml");
}
