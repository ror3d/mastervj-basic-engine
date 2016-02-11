#include "StaticMesh.h"
#include <vector>

#include "Texture/Texture.h"
#include "Material/Material.h"
#include "Renderable/RenderableVertexs.h"

#include <Core/Engine/Engine.h>

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
		//Header---------------------
		unsigned short l_header, l_VertexType, l_numMaterials, l_NumBytes;
		fread(&l_header, sizeof(unsigned short), 1, l_meshFile);   //lectura del header
		if (l_header != 0xFE55)
		{
			return false;
		}

		//Materials---------------------

		fread(&l_numMaterials, sizeof(unsigned short), 1, l_meshFile); //lectura del numero de materiales
		if (l_numMaterials == 0)
		{
			return false;
		}
		m_materials.resize(l_numMaterials);


		for (int i = 0; i < l_numMaterials; ++i)
		{  //lectura de los materiales
			unsigned short l_NumChars;
			fread(&l_NumChars, sizeof(unsigned short), 1, l_meshFile);

			char *l_MaterialName = new char[l_NumChars + 1];
			fread(&l_MaterialName[0], sizeof(char), l_NumChars + 1, l_meshFile);

			m_materials[i] = CEngine::GetSingletonPtr()->getMaterialManager()->get(l_MaterialName);

			delete[] l_MaterialName;
		}

		//Vertex & Index---------------------

		for (int i = 0; i < l_numMaterials; ++i)
		{
			//Vertices---------------------
			unsigned short l_VertexType;
			fread(&l_VertexType, sizeof(unsigned short), 1, l_meshFile);

			unsigned short l_NumVertexs;
			fread(&l_NumVertexs, sizeof(unsigned short), 1, l_meshFile);

			unsigned long l_NumBytes;
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
				assert(!"Num Index Error");
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
				CalcTangentsAndBinormals(l_VtxsData, (unsigned short*) l_IdxData, l_NumVertexs, l_NumIndexsFile, sizeof(MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX), 0, 12, 24, 40, 56);

				if (l_IndexType == 16)
					l_RV = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexsFile);
				else
					l_RV = new CKGTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexsFile);
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
	}

	return true;
}

void CStaticMesh::Render(CContextManager *_context) const
{
	for (size_t i = 0; i < m_materials.size(); ++i)
	{
		if (m_materials[i]->getName() != "")
		{
			m_materials[i]->apply();
			m_renderableVertexs[i]->RenderIndexed(_context, m_materials[i]->getEffectTechnique());
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

void CStaticMesh::CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride)
{
	Vect3f *tan1 = new Vect3f[VtxCount * 2];
	Vect3f *tan2 = tan1 + VtxCount;
	ZeroMemory(tan1, VtxCount * sizeof(Vect3f)* 2);
	unsigned char *l_VtxAddress = (unsigned char*)VtxsData;

	for (size_t b = 0; b<IdxCount; b += 3)
	{
		unsigned short i1 = IdxsData[b];
		unsigned short i2 = IdxsData[b + 1];
		unsigned short i3 = IdxsData[b + 2];

		Vect3f *v1 = (Vect3f *)&l_VtxAddress[i1*VertexStride + GeometryStride];
		Vect3f *v2 = (Vect3f *)&l_VtxAddress[i2*VertexStride + GeometryStride];
		Vect3f *v3 = (Vect3f *)&l_VtxAddress[i3*VertexStride + GeometryStride];
		Vect2f *w1 = (Vect2f *)&l_VtxAddress[i1*VertexStride + TextureCoordsStride];
		Vect2f *w2 = (Vect2f *)&l_VtxAddress[i2*VertexStride + TextureCoordsStride];
		Vect2f *w3 = (Vect2f *)&l_VtxAddress[i3*VertexStride + TextureCoordsStride];

		float x1 = v2->x - v1->x;
		float x2 = v3->x - v1->x;
		float y1 = v2->y - v1->y;

		float y2 = v3->y - v1->y;
		float z1 = v2->z - v1->z;
		float z2 = v3->z - v1->z;
		float s1 = w2->x - w1->x;
		float s2 = w3->x - w1->x;
		float t1 = w2->y - w1->y;
		float t2 = w3->y - w1->y;
		float r = 1.0F / (s1 * t2 - s2 * t1);

		Vect3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		Vect3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

		assert(i1<VtxCount);
		assert(i2<VtxCount);
		assert(i3<VtxCount);
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (size_t b = 0; b<VtxCount; ++b)
	{
		Vect3f *l_NormalVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + NormalStride];
		Vect3f *l_TangentVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + TangentStride];
		Vect4f *l_TangentVtx4 = (Vect4f *)&l_VtxAddress[b*VertexStride + TangentStride];
		Vect3f *l_BiNormalVtx = (Vect3f *)&l_VtxAddress[b*VertexStride + BiNormalStride];

		const Vect3f& t = tan1[b];
		// Gram-Schmidt orthogonalize
		Vect3f l_VAl = t - (*l_NormalVtx)*((*l_NormalVtx)*t);
		l_VAl.Normalize();
		*l_TangentVtx = l_VAl;

		// Calculate handedness
		Vect3f l_Cross;
		l_Cross = (*l_NormalVtx) ^ (*l_TangentVtx);
		l_TangentVtx4->w = (l_Cross*(tan2[b]))< 0.0f ? -1.0f : 1.0f;
		*l_BiNormalVtx = (*l_NormalVtx) ^ (*l_TangentVtx);
	}

	delete[] tan1;
}