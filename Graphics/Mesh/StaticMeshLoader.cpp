#include "StaticMeshLoader.h"
#include "Mesh.h"

#include "Material/Material.h"
#include "RenderableVertexs.h"

#include <Core/Engine/Engine.h>
#include <Graphics/Material/MaterialManager.h>

#include <Graphics/Mesh/CookedMeshManager.h>
#include <Graphics/Mesh/StaticMeshManager.h>
#include <Graphics/Mesh/StaticMesh.h>
#include <PhysX/PhysXManager.h>
#include <Base/XML/XMLTreeNode.h>

#include <algorithm>

CStaticMeshLoader::CStaticMeshLoader()
{}


CStaticMeshLoader::~CStaticMeshLoader()
{
	destroy();
}

void CStaticMeshLoader::destroy()
{
	for (auto &const meshFile : m_meshFiles)
	{
		delete meshFile.second;
	}
	m_meshFiles.clear();
}

bool CStaticMeshLoader::LoadMesh(const std::string& meshFileName)
{
	MeshFile *mf = new MeshFile();
	if (!mf->Load(meshFileName))
	{
		delete mf;
		return false;
	}
	m_meshFiles[meshFileName] = mf;
	return true;
}

CMesh* CStaticMeshLoader::GetMesh(CXMLTreeNode& node)
{
	std::string fName = node.GetPszProperty("filename", "", true);
	std::string name = node.GetPszProperty("name", "", false);
	CMesh* m = GetMesh(fName);
	if (m)
	{
		m->setName(name);
	}
	return m;
}

CMesh* CStaticMeshLoader::GetMesh(const std::string& meshFileName)
{
	MeshFile *mf;
	CMesh *m;
	std::vector<CRenderableVertexs*> vtxs;
	std::vector<CMaterial *> mats;

	auto it = m_meshFiles.find(meshFileName);
	if (it == m_meshFiles.end())
	{
		if (!LoadMesh(meshFileName))
		{
			return nullptr;
		}
		mf = m_meshFiles[meshFileName];
	}
	else
	{
		mf = it->second;
	}


	for ( auto const &matName : mf->materials )
	{
		mats.push_back(CEngine::GetSingletonPtr()->getMaterialManager()->get( matName ));
	}

	for ( auto const &mesh : mf->meshes )
	{
		CRenderableVertexs *l_RV = NULL;

		unsigned short l_VertexType = mesh.vertexType;
		unsigned short l_IndexType = mesh.indexSize;

		void * l_VtxsData = mesh.vertexes;
		void * l_IdxData = mesh.indices;

		unsigned int l_NumVertexs = mesh.nVertexes;
		unsigned int l_NumIndexs = mesh.nIndices;

		if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
		{
			if (l_IndexType == 16)
				l_RV = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexs);
			else
				l_RV = new CKGTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexs);
		}
		else if (l_VertexType == MV_POSITION_NORMAL_VERTEX::GetVertexType())
		{
			if (l_IndexType == 16)
				l_RV = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexs);
			else
				l_RV = new CKGTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexs);
		}
		else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::GetVertexType())
		{
			if (l_IndexType == 16)
				l_RV = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexs);
			else
				l_RV = new CKGTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexs);
		}
		else if (l_VertexType == MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX::GetVertexType())
		{
			if (l_IndexType == 16)
				l_RV = new CKGTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexs);
			else
				l_RV = new CKGTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, l_NumIndexs);
		}
		else
		{
			DEBUG_ASSERT( !"Vertex layout not implemented!" );
			return nullptr;
		}

		vtxs.push_back(l_RV);

	}

	m = new CMesh(vtxs, mats);

	m->setMeshFileName(meshFileName);

	return m;
}

std::vector<Vect3f> CStaticMeshLoader::GetVect3fArray( const MeshFile::MeshData& mesh )
{
	unsigned int l_VertexType = mesh.vertexType;
	if ( l_VertexType == MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType() )
	{
		return GetVect3fArrayInternal( reinterpret_cast<MV_POSITION_NORMAL_TEXTURE_VERTEX*>( mesh.vertexes ), mesh.nVertexes );
	}
	else if ( l_VertexType == MV_POSITION_NORMAL_VERTEX::GetVertexType() )
	{
		return GetVect3fArrayInternal( reinterpret_cast<MV_POSITION_NORMAL_VERTEX*>( mesh.vertexes ), mesh.nVertexes );
	}
	else if ( l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::GetVertexType() )
	{
		return GetVect3fArrayInternal( reinterpret_cast<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX*>( mesh.vertexes ), mesh.nVertexes );
	}
	else if ( l_VertexType == MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX::GetVertexType() )
	{
		return GetVect3fArrayInternal( reinterpret_cast<MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX*>( mesh.vertexes ), mesh.nVertexes );
	}
	else
	{
		DEBUG_ASSERT( !"Vertex type not implemented!" );
		return {};
	}
}

template <typename T>
std::vector<Vect3f> CStaticMeshLoader::GetVect3fArrayInternal( const T* vertexes, unsigned int nVtx )
{
	std::vector<Vect3f> ret;
	for ( int i = 0; i < nVtx; ++i )
	{
		ret.push_back( vertexes[i].Position );
	}
	return ret;
}

bool CStaticMeshLoader::FillColliderDescriptor( const std::string& coreName, CPhysxColliderShapeDesc* shapeDesc )
{
	std::string meshFileName = CEngine::GetSingleton().getStaticMeshManager()->get(coreName)->getMeshFileName();
	std::string cookedFileName = meshFileName
		+ ((shapeDesc->shape == CPhysxColliderShapeDesc::Shape::TriangleMesh) ? "_tri" : "_conv");
	std::vector<uint8> * cooked = nullptr;

	bool l_loaded = CEngine::GetSingleton().getCookedMeshManager()->Load(cookedFileName, cooked);

	if (!l_loaded)
	{
		MeshFile *meshFile;

		auto it = m_meshFiles.find(meshFileName);
		if (it == m_meshFiles.end())
		{
			if (!LoadMesh(meshFileName))
			{
				return false;
			}
			meshFile = m_meshFiles[meshFileName];
		}
		else
		{
			meshFile = it->second;
		}

		if (shapeDesc->shape == CPhysxColliderShapeDesc::Shape::TriangleMesh)
		{
			int numVertex = 0 ;
			for (int i = 0; i < meshFile->meshes.size(); ++i)
			{
				numVertex += meshFile->meshes[i].nVertexes;
			}
			if (numVertex >= 1<<16)
			{
				DEBUG_ASSERT(!"MESH WITH MORE THAN 65000 vertexes");
				shapeDesc->shape = CPhysxColliderShapeDesc::Shape::ConvexMesh;
				return FillColliderDescriptor(meshFileName, shapeDesc);
			}
		}

		cooked = new std::vector<uint8>();

		if (shapeDesc->shape == CPhysxColliderShapeDesc::Shape::TriangleMesh)
		{
			std::vector<Vect3f> vertexes;
			std::vector<unsigned short> indexes;

			uint16 offset = 0;
			for (int i = 0; i < meshFile->meshes.size(); ++i)
			{
				std::vector<Vect3f> meshVtxs = GetVect3fArray(meshFile->meshes[i]);
					auto idxs = reinterpret_cast<unsigned short*>(meshFile->meshes[i].indices);
					size_t sz = indexes.size();
					indexes.resize(indexes.size() + meshFile->meshes[i].nIndices);
					std::transform(idxs, idxs + meshFile->meshes[i].nIndices,
						indexes.begin()+sz,
						[offset](unsigned short v) { return v + offset; });
				vertexes.insert(vertexes.end(), meshVtxs.begin(), meshVtxs.end());
					offset = vertexes.size();
			}

			CEngine::GetSingleton().getPhysXManager()->cookTriangleMesh(vertexes, indexes, cooked, meshFile->meshes[0].indexSize);
		}
		else if (shapeDesc->shape == CPhysxColliderShapeDesc::Shape::ConvexMesh)
		{
			std::vector<Vect3f> vertexes;

			for (int i = 0; i < meshFile->meshes.size(); ++i)
		{
				std::vector<Vect3f> meshVtxs = GetVect3fArray(meshFile->meshes[i]);
				vertexes.insert(vertexes.end(), meshVtxs.begin(), meshVtxs.end());
		}
			CEngine::GetSingleton().getPhysXManager()->cookConvexMesh(vertexes, cooked);

		}
		else
		{
			DEBUG_ASSERT(!"Collider type is not Triangle or Convex");
		}

		CEngine::GetSingleton().getCookedMeshManager()->add(cookedFileName, cooked);
	}

	shapeDesc->cookedMeshData = cooked;

	return true;
}

bool CStaticMeshLoader::MeshFile::Load( const std::string &FileName )
{
	FILE *l_meshFile = NULL;
	fopen_s(&l_meshFile, FileName.c_str(), "rb");

	if (l_meshFile == NULL)
	{
		DEBUG_ASSERT(!"Could not open file!");
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
			DEBUG_ASSERT(!"Unrecognized header!");
			fclose(l_meshFile);
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
			DEBUG_ASSERT(!"No materials found!");
			fclose(l_meshFile);
			return false;
		}
		materials.resize(l_numMaterials);


		if (shouldReadMaterials)
		{
			for (int i = 0; i < l_numMaterials; ++i)
			{  //lectura de los materiales
				unsigned short l_NumChars;
				fread(&l_NumChars, sizeof(unsigned short), 1, l_meshFile);

				char *l_MaterialName = new char[l_NumChars + 1];
				fread(&l_MaterialName[0], sizeof(char), l_NumChars + 1, l_meshFile);

				materials[i] = std::string(l_MaterialName);

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
			else if (l_VertexType == MV_POSITION_NORMAL_VERTEX::GetVertexType())
				l_NumBytes = sizeof(MV_POSITION_NORMAL_VERTEX)*l_NumVertexs;
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
				DEBUG_ASSERT( !"unrecognized vertex type" );
				fclose(l_meshFile);
				return false;
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
				delete[] l_VtxsData;
				fclose(l_meshFile);
				return false;
			}

			void *l_IdxData = new char[l_NumBytes];
			fread(l_IdxData, 1, l_NumBytes, l_meshFile);

			MeshData mesh;
			mesh.vertexes = l_VtxsData;
			mesh.vertexType = l_VertexType;
			mesh.nVertexes = l_NumVertexs;

			mesh.indices = l_IdxData;
			mesh.indexSize = l_IndexType;
			mesh.nIndices = l_NumIndexsFile;

			meshes.push_back( std::move(mesh) );

		}

		//Footer---------------------
		unsigned short l_footer;
		fread(&l_footer, sizeof(unsigned short), 1, l_meshFile);
		if (l_footer != 0x55FE)
		{
			DEBUG_ASSERT(!"Unrecognized footer!");
			fclose(l_meshFile);
			return false;
		}
	}

	fclose(l_meshFile);

	return true;
}
