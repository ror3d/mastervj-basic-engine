#include "AnimatedMesh.h"
#include "AnimatedMeshInstance.h"
#include "Effect/EffectParameters.h"
#include "Effect/EffectManager.h"
#include "Mesh/RenderableVertexs.h"
#include "Material/Material.h"
#include "Renderable/RenderableObjectTechnique.h"
#include <Base/XML/XMLTreeNode.h>
#include <cal3d/cal3d.h>


CAnimatedMesh::CAnimatedMesh(CXMLTreeNode &node)
	: CMesh(node)
{
	std::string path = node.GetPszProperty("path");
	m_CalCoreModel = new CalCoreModel(getName());

	std::string fname;

	CXMLTreeNode l_XML;
	if (l_XML.LoadFile((path+"actor.xml").c_str()))
	{
		CXMLTreeNode actor = l_XML["actor"];
		if (actor.Exists())
		{
			for (int i = 0; i < actor.GetNumChildren(); ++i)
			{
				CXMLTreeNode node = actor(i);
				if (std::string(node.GetName()) == "skeleton")
				{
					fname = path + node.GetPszProperty("filename");
					if (!m_CalCoreModel->loadCoreSkeleton(fname))
					{
						DEBUG_ASSERT(!"Could not load skeleton!");
						throw "Could not load skeleton!";
					}
				}
				else if (std::string(node.GetName()) == "mesh")
				{
					fname = path + node.GetPszProperty("filename");
					if (m_CalCoreModel->loadCoreMesh(fname) < 0)
					{
						DEBUG_ASSERT(!"Could not load mesh!");
						throw "Could not load mesh!";
					}
				}
				else if (std::string(node.GetName()) == "material")
				{
					m_materials.push_back(new CMaterial(node));
				}
				else if (std::string(node.GetName()) == "animation")
				{
					std::string fname = path + node.GetPszProperty("filename");
					if (m_CalCoreModel->loadCoreAnimation(fname) < 0)
					{
						DEBUG_ASSERT(!"Could not load animation!");
						throw "Could not load animation!";
					}
				}
			}
		}
	}


	m_CalHardwareModel = new CalHardwareModel(m_CalCoreModel);
	LoadVertexBuffers();
}


CAnimatedMesh::~CAnimatedMesh()
{
	delete m_CalHardwareModel;
	delete m_CalCoreModel;
	for (auto &const m : m_materials)
	{
		delete m;
	}
	m_materials.clear();
}

bool CAnimatedMesh::LoadVertexBuffers()
{
	int nVertices = 0;
	int nFaces = 0;
	CalCoreModel *l_CalCoreModel = m_CalCoreModel;
	for (int i = 0; i<l_CalCoreModel->getCoreMeshCount(); ++i)
	{
		CalCoreMesh *l_CalCoreMesh = l_CalCoreModel->getCoreMesh(i);
		for (int j = 0; j<l_CalCoreMesh->getCoreSubmeshCount(); ++j)
		{
			CalCoreSubmesh *l_CalCoreSubmesh = l_CalCoreMesh->getCoreSubmesh(j);
			nVertices += l_CalCoreSubmesh->getVertexCount();
			nFaces += l_CalCoreSubmesh->getFaceCount();
		}
	}
	MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX * l_Vertexs = (MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX *) malloc(nFaces * 3 * sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));

	CalIndex *l_Faces = (CalIndex *)malloc(nFaces * 3 * sizeof(CalIndex));

	m_CalHardwareModel->setVertexBuffer((char*)l_Vertexs, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setNormalBuffer(((char*)l_Vertexs) + 12, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setWeightBuffer(((char*)l_Vertexs) + 24, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vertexs) + 40, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));


	m_CalHardwareModel->setTextureCoordNum(1);
	m_CalHardwareModel->setTextureCoordBuffer(0, ((char*)l_Vertexs) + 56, sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX));
	m_CalHardwareModel->setIndexBuffer(l_Faces);
	m_CalHardwareModel->load(0, 0, MAXBONES);
	nFaces = m_CalHardwareModel->getTotalFaceCount();
	nVertices = m_CalHardwareModel->getTotalVertexCount();
	CRenderableVertexs *vtxs;
	if (sizeof(CalIndex) == 2)
	{
		vtxs = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_Vertexs, nVertices, l_Faces, nFaces * 3);
	}
	else
	{
		vtxs = new CKGTriangleListRenderableIndexed32Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_Vertexs, nVertices, l_Faces, nFaces * 3);
	}
	free(l_Vertexs);
	free(l_Faces);
	m_renderableVertexs.push_back(vtxs);
	return true;
}

void CAnimatedMesh::Render(CContextManager* context, CalModel *calModel)
{
	for (int l_HardwareMeshId = 0;
		 l_HardwareMeshId < m_CalHardwareModel->getHardwareMeshCount();
		 ++l_HardwareMeshId)
	{
		m_materials[l_HardwareMeshId]->apply();
		m_CalHardwareModel->selectHardwareMesh(l_HardwareMeshId);
		Mat44f l_Transformations[MAXBONES];

		for (int l_BoneId = 0;
			 l_BoneId<m_CalHardwareModel->getBoneCount();
			 ++l_BoneId)
		{
			Quatf l_Quaternion = (const Quatf &)m_CalHardwareModel->getRotationBoneSpace(l_BoneId, calModel->getSkeleton());
			l_Transformations[l_BoneId].SetIdentity();
			l_Transformations[l_BoneId].SetRotByQuat(l_Quaternion);
			CalVector translationBoneSpace = m_CalHardwareModel->getTranslationBoneSpace(l_BoneId, calModel->getSkeleton());
			l_Transformations[l_BoneId].SetPos(Vect3f(translationBoneSpace.x, translationBoneSpace.y, translationBoneSpace.z));
		}
		memcpy(&CEffectManager::m_AnimatedModelEffectParameters.m_Bones,
			   l_Transformations,
			   MAXBONES*sizeof(float) * 4 * 4);

		CEffectTechnique *l_EffectTechnique = m_materials[l_HardwareMeshId]->getRenderableObjectTechique()->GetEffectTechnique();

		l_EffectTechnique->SetConstantBuffer(2, &CEffectManager::m_AnimatedModelEffectParameters.m_Bones);
			m_renderableVertexs[0]
				->RenderIndexed(context,
								l_EffectTechnique,
								m_CalHardwareModel->getFaceCount() * 3,
								m_CalHardwareModel->getStartIndex(),
								m_CalHardwareModel->getBaseVertexIndex());
	}
}
