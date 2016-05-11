#include "StaticMesh.h"

#include "VertexTypes.h"

#include "Texture/Texture.h"
#include "Material/Material.h"
#include "Renderable/RenderableVertexs.h"

#include <Core/Engine/Engine.h>

#include <Renderable/RenderableObjectTechnique.h>

#include <vector>


CStaticMesh::CStaticMesh(const CXMLTreeNode& node)
	: CNamed(node)
	, m_BsCenter(0.0f, 0.0f, 0.0f)
	, m_AabbMin(0.0f, 0.0f, 0.0f)
	, m_AabbMax(0.0f, 0.0f, 0.0f)
	, m_BsRadius(0.0f)
	, m_meshLoaded(false)
{
	m_fileName = node.GetPszProperty("filename", "");
}


CStaticMesh::~CStaticMesh()
{
	Destroy();
}


bool CStaticMesh::MeshFile::Load( const std::string &FileName )
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

			Mesh mesh;
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

bool CStaticMesh::Load()
{
	m_meshLoaded = false;

	MeshFile meshFile;
	if ( !meshFile.Load( m_fileName ) )
	{
		return false;
	}

	for ( auto const &matName : meshFile.materials )
	{
		m_materials.push_back(CEngine::GetSingletonPtr()->getMaterialManager()->get( matName ));
	}

	for ( auto const &mesh : meshFile.meshes )
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
			return false;
		}

		m_renderableVertexs.push_back(l_RV);

	}

	m_meshLoaded = true;
	return true;
}

void CStaticMesh::Render(CContextManager *_context)
{
	for (size_t i = 0; i<m_renderableVertexs.size(); ++i)
	{
		CMaterial * l_Material = m_materials[i].getRef();
		if (l_Material != NULL && l_Material->getRenderableObjectTechique() != NULL)
		{
			l_Material->apply();
			m_renderableVertexs[i]->RenderIndexed(_context, l_Material->getRenderableObjectTechique()->GetEffectTechnique());
		}

	}
}


bool CStaticMesh::Reload()
{
	Destroy();
	return Load();
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


std::vector<Vect3f> CStaticMesh::getVect3fArray( const MeshFile::Mesh& mesh )
{
	unsigned int l_VertexType = mesh.vertexType;
	if ( l_VertexType == MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType() )
	{
		return getVect3fArrayInternal( reinterpret_cast<MV_POSITION_NORMAL_TEXTURE_VERTEX*>( mesh.vertexes ), mesh.nVertexes );
	}
	else if ( l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::GetVertexType() )
	{
		return getVect3fArrayInternal( reinterpret_cast<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX*>( mesh.vertexes ), mesh.nVertexes );
	}
	else if ( l_VertexType == MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX::GetVertexType() )
	{
		return getVect3fArrayInternal( reinterpret_cast<MV_POSITION_NORMAL_TANGENT_BINORMAL_TEXTURE_VERTEX*>( mesh.vertexes ), mesh.nVertexes );
	}
	else
	{
		return {};
	}
}

template <typename T>
std::vector<Vect3f> CStaticMesh::getVect3fArrayInternal( const T* vertexes, unsigned int nVtx )
{
	std::vector<Vect3f> ret;
	for ( int i = 0; i < nVtx; ++i )
	{
		ret.push_back( vertexes[i].Position );
	}
	return ret;
}

bool CStaticMesh::FillColliderDescriptor( CPhysxColliderShapeDesc* shapeDesc )
{
	if ( shapeDesc->shape == CPhysxColliderShapeDesc::Shape::Box )
	{
		// TODO get values for this and implement
	}
	else if ( shapeDesc->shape == CPhysxColliderShapeDesc::Shape::Sphere )
	{
		// TODO get values for this and implement
	}
	else if ( shapeDesc->shape == CPhysxColliderShapeDesc::Shape::Capsule )
	{
		// TODO get values for this and implement
	}
	else if ( shapeDesc->shape == CPhysxColliderShapeDesc::Shape::TriangleMesh )
	{
		// TODO get values for this and implement
	}
	else if ( shapeDesc->shape == CPhysxColliderShapeDesc::Shape::ConvexMesh )
	{
		//std::shared_ptr<std::vector<uint8>> * cooked;
		std::vector<uint8> * cooked = nullptr;

		bool l_loaded = CEngine::GetSingleton().getCookedMeshManager()->Load( getName(), cooked );

		if ( !l_loaded )
		{
			MeshFile meshFile;
			if ( !meshFile.Load( m_fileName ) )
			{
				return false;
			}

			std::vector<Vect3f> vertexes;

			for ( int i = 0; i < meshFile.meshes.size(); ++i )
			{
				std::vector<Vect3f> meshVtxs = getVect3fArray( meshFile.meshes[i] );
				vertexes.insert( vertexes.end(), meshVtxs.begin(), meshVtxs.end() );
			}

			cooked = new std::vector<uint8>();

			CEngine::GetSingleton().getPhysXManager()->cookConvexMesh(vertexes, cooked);

			CEngine::GetSingleton().getCookedMeshManager()->add( getName(), cooked );
		}

		shapeDesc->cookedMeshData = cooked;
	}
	else
	{
		DEBUG_ASSERT( !"Unrecognized collider type" );
	}



	return true;
}
