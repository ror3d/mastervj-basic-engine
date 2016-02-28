#include "StaticMesh.h"
#include <vector>

#include "Texture/Texture.h"
#include "Material/Material.h"
#include "Renderable/RenderableVertexs.h"

#include <Core/Engine/Engine.h>

#include <Renderable/RenderableObjectTechnique.h>

#include "VertexTypes.h"


CStaticMesh::CStaticMesh()
	: CNamed("")
	, m_BsCenter(0.0f, 0.0f, 0.0f)
	, m_AabbMin(0.0f, 0.0f, 0.0f)
	, m_AabbMax(0.0f, 0.0f, 0.0f)
	, m_BsRadius(0.0f)
{
}


CStaticMesh::~CStaticMesh()
{
	Destroy();
}


bool CStaticMesh::Load(const std::string &FileName)
{
	m_fileName = FileName;
	FILE *l_meshFile = NULL;
	fopen_s(&l_meshFile, FileName.c_str(), "rb");

	if (l_meshFile == NULL)
	{
		return false;
	}
	else
	{
		bool shouldReadMaterials = true;
		//Header---------------------
		unsigned short l_header, l_VertexType, l_numMaterials, l_NumBytes;
		fread(&l_header, sizeof(unsigned short), 1, l_meshFile);   //lectura del header
		if (l_header != 0xFE55 && l_header != 0xFE56)
		{
			return false;
		}
		if (l_header == 0xFE56)
		{
			shouldReadMaterials = false;
		}

		//Materials---------------------

		fread(&l_numMaterials, sizeof(unsigned short), 1, l_meshFile); //lectura del numero de materiales
		if (l_numMaterials == 0)
		{
			return false;
		}
		m_materials.resize(l_numMaterials);


		if (shouldReadMaterials)
		{
			for (int i = 0; i < l_numMaterials; ++i)
			{  //lectura de los materiales
				unsigned short l_NumChars;
				fread(&l_NumChars, sizeof(unsigned short), 1, l_meshFile);

				char *l_MaterialName = new char[l_NumChars + 1];
				fread(&l_MaterialName[0], sizeof(char), l_NumChars + 1, l_meshFile);

				m_materials[i] = CEngine::GetSingletonPtr()->getMaterialManager()->get(l_MaterialName);

				delete[] l_MaterialName;
			}
		}

		//Vertex & Index---------------------

		for (int i = 0; i < l_numMaterials; ++i)
		{
			//Vertices---------------------
			unsigned short l_VertexType;
			fread(&l_VertexType, sizeof(unsigned short), 1, l_meshFile);

			unsigned short l_NumVertexs;
			fread(&l_NumVertexs, sizeof(unsigned short), 1, l_meshFile);

			unsigned long l_NumBytes = 0;
			if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
				l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_VERTEX)*l_NumVertexs;
			else if (l_VertexType == MV_POSITION_COLOR_VERTEX::GetVertexType())
				l_NumBytes = sizeof(MV_POSITION_COLOR_VERTEX)*l_NumVertexs;
			else if (l_VertexType == MV_POSITION_TEXTURE_VERTEX::GetVertexType())
				l_NumBytes = sizeof(MV_POSITION_TEXTURE_VERTEX)*l_NumVertexs;
			else if (l_VertexType == MV_POSITION_COLOR_TEXTURE_VERTEX::GetVertexType())
				l_NumBytes = sizeof(MV_POSITION_COLOR_TEXTURE_VERTEX)*l_NumVertexs;
			else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::GetVertexType())
				l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX)*l_NumVertexs;
			else if (l_VertexType == MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX::GetVertexType())
				l_NumBytes = sizeof(MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX)*l_NumVertexs;
			else
			{
				throw std::runtime_error("unrecognized vertex type");
			}

			void *l_VtxsData = new char[l_NumBytes];
			fread(l_VtxsData, 1, l_NumBytes, l_meshFile);

			//Indices---------------------
			unsigned short l_IndexType;
			fread(&l_IndexType, sizeof(unsigned short), 1, l_meshFile);

			unsigned short l_NumIndexsFile;
			unsigned int l_numVertexs;

			if (l_IndexType == 16)
			{
				fread(&l_NumIndexsFile, sizeof(unsigned short), 1, l_meshFile);
				l_NumBytes = sizeof(unsigned short)*l_NumIndexsFile;
				l_numVertexs = (unsigned int)l_NumIndexsFile;//Antes m_NumIndexs
			}
			else if (l_IndexType == 32)
			{
				fread(&l_NumIndexsFile, sizeof(unsigned int), 1, l_meshFile);
				l_NumBytes = sizeof(unsigned int)*l_NumIndexsFile;
				l_numVertexs = l_NumIndexsFile;
			}
			else
			{
				DEBUG_ASSERT(!"Num Index Error");
			}

			void *l_IdxData = new char[l_NumBytes];
			fread(l_IdxData, 1, l_NumBytes, l_meshFile);

			CRenderableVertexs *l_RV = NULL;

			if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
			{
				if (l_IndexType == 16)
					l_RV = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexsFile);
				else
					l_RV = new CKGTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexsFile);
			}
			else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::GetVertexType())
			{
				if (l_IndexType == 16)
					l_RV = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexsFile);
				else
					l_RV = new CKGTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexsFile);
			}
			else if (l_VertexType == MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX::GetVertexType())
			{
				if (l_IndexType == 16)
					l_RV = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexsFile);
				else
					l_RV = new CKGTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexsFile);
			}
			else
			{
				DEBUG_ASSERT( !"Vertex layout not implemented!" );
			}

			m_renderableVertexs.push_back(l_RV);

			delete[] l_VtxsData;
			delete[] l_IdxData;
		}

		//Footer---------------------
		unsigned short l_footer;
		fread(&l_footer, sizeof(unsigned short), 1, l_meshFile);
		if (l_footer != 0x55FE)
		{
			return false;
		}
		//Recogo medidas bounding box
		
	}

	fclose(l_meshFile);

	return true;
}

void CStaticMesh::Render(CContextManager *_context) const
{
	for (size_t i = 0; i<m_renderableVertexs.size(); ++i)
	{
		CMaterial *l_Material = m_materials[i];
		if (l_Material != NULL && l_Material->getRenderableObjectTechique() != NULL)
	{
			l_Material->apply();
			m_renderableVertexs[i]->RenderIndexed(_context,
				l_Material->getRenderableObjectTechique()->GetEffectTechnique());
		}

	}
}


bool CStaticMesh::Reload()
{
	Destroy();
	if (Load(m_fileName))
		return true;
	else
		return false;
}

void CStaticMesh::Destroy()
{
	for (auto it = m_renderableVertexs.begin(); it != m_renderableVertexs.end(); ++it)
	{
		delete *it;
	}
	m_renderableVertexs.clear();
	m_materials.clear();
}
