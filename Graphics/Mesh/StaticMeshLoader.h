#pragma once

#include <vector>
#include <map>
#include <string>

#include <Base/Utils/Named.h>
#include <Base/Math/MathTypes.h>
#include "VertexTypes.h"

class CMesh;
class CPhysxColliderShapeDesc;
class CXMLTreeNode;

class CStaticMeshLoader
{
public:
	struct MeshFile
	{
		typedef unsigned short VertexType_t;
		struct MeshData
		{
			VertexType_t vertexType;
			void * vertexes;
			unsigned int nVertexes;

			unsigned short indexSize;
			void * indices;
			unsigned int nIndices;

			MeshData()
				: vertexes(nullptr)
				, indices(nullptr)
				, vertexType(0)
				, nVertexes(0)
				, indexSize(0)
				, nIndices(0)
			{ }

			MeshData( MeshData&& m )
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

			~MeshData()
			{
				if ( vertexes ) delete[] vertexes;
				if ( indices ) delete[] indices;
			}
		};
		std::vector<std::string> materials;
		std::vector<MeshData> meshes;

		bool Load( const std::string &fileName );
	};
private:
	std::map<std::string, MeshFile*> m_meshFiles;

	bool LoadMesh(const std::string& meshFileName);

	template<typename T>
	std::vector<Vect3f> GetVect3fArrayInternal( const T* vertexes, unsigned int nVtx );

	std::vector<Vect3f> GetVect3fArray(const MeshFile::MeshData& mesh);

public:

	CStaticMeshLoader();
	~CStaticMeshLoader();

	CMesh* GetMesh(const std::string& meshFileName);
	CMesh* GetMesh(CXMLTreeNode& node);

	bool FillColliderDescriptor( const std::string& coreName, CPhysxColliderShapeDesc* shapeDesc );

	void destroy();
};

