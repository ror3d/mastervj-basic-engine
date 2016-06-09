#pragma once
#include <vector>
#include <string>

#include <Base/Utils/Named.h>
#include <Base/Math/MathTypes.h>
#include "VertexTypes.h"
#include <Utils/TMapManager.h>

class CRenderManager;
class CContextManager;
class CRenderableVertexs;
class CMaterial;
class CXMLTreeNode;
class CPhysxColliderShapeDesc;

class CStaticMesh : public CNamed
{
	struct MeshFile
	{
		typedef unsigned short VertexType_t;
		struct Mesh
		{
			VertexType_t vertexType;
			void * vertexes;
			unsigned int nVertexes;

			unsigned short indexSize;
			void * indices;
			unsigned int nIndices;

			Mesh()
				: vertexes(nullptr)
				, indices(nullptr)
				, vertexType(0)
				, nVertexes(0)
				, indexSize(0)
				, nIndices(0)
			{ }

			Mesh( Mesh&& m )
				: vertexes(m.vertexes)
				, indices(m.indices)
				, vertexType(m.vertexType)
				, nVertexes(m.nVertexes)
				, indexSize(m.indexSize)
				, nIndices(m.nIndices)
			{
				m.vertexes = nullptr;
				m.indices = nullptr;
				m.vertexType = 0;
				m.nVertexes = 0;
				m.indexSize = 0;
				m.nIndices = 0;
			}

			~Mesh()
			{
				if ( vertexes ) delete[] vertexes;
				if ( indices ) delete[] indices;
			}
		};
		std::vector<std::string> materials;
		std::vector<Mesh> meshes;

		bool Load( const std::string &fileName );
	};

	std::vector<Vect3f> getVect3fArray( const MeshFile::Mesh& mesh);

	template<typename T>
	std::vector<Vect3f> getVect3fArrayInternal( const T* vertexes, unsigned int nVtx );

protected:
	Vect3f m_BsCenter, m_AabbMin, m_AabbMax;
	float m_BsRadius;
	std::vector<CRenderableVertexs*> m_renderableVertexs;
	std::vector<TMapManager<CMaterial>::Ref> m_materials;
	std::string m_fileName;

	bool m_meshLoaded;
	//void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride);

public:
	CStaticMesh(const CXMLTreeNode& node);
	virtual ~CStaticMesh();

	bool Load ();
	bool Reload ();
	void Render (CContextManager *_context);
	void Destroy();

	inline void setMaterial(unsigned int idx, TMapManager<CMaterial>::Ref mat)
	{
		DEBUG_ASSERT(idx < m_materials.size());
		m_materials[idx] = std::move(mat);
	}
	Vect3f GetBsCenter(){ return m_BsCenter;  }
	Vect3f GetAabbMin(){ return m_AabbMin;  }
	Vect3f GetAabbMax(){ return m_AabbMax; }

	bool FillColliderDescriptor( CPhysxColliderShapeDesc* shapeDesc );
};
