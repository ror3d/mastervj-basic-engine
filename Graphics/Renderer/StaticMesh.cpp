#include "StaticMesh.h"
#include <vector>

#include "Texture/Texture.h"
#include "Material/Material.h"
#include "Renderer/RenderableVertexs.h"


CStaticMesh::CStaticMesh()
	: CNamed("")
	, m_BsCenter(0.0f,0.0f,0.0f)
	, m_AabbMin(0.0f,0.0f,0.0f)
	, m_AabbMax(0.0f,0.0f,0.0f)
	, m_BsRadius(0.0f)
{
}
CStaticMesh::~CStaticMesh()
{
	Destroy();
}
//Codigo basado en https://code.google.com/p/uab-engine/source/browse/trunk/+uab-engine/?r=3
bool CStaticMesh::Load(const std::string &FileName){
	/*
        m_fileName=FileName;
        FILE *l_meshFile = NULL;
        fopen_s(&l_meshFile, FileName.c_str(), "rb");

        if(l_meshFile==NULL){
                return false;
        }
        else{
                unsigned short l_header, l_numMaterials;
                fread(&l_header, sizeof(unsigned short), 1, l_meshFile);   //lectura del header
                if(l_header!=0x55FF){
					return false;
                }
                
                fread(&l_numMaterials, sizeof(unsigned short), 1, l_meshFile); //lectura del numero de materiales
                if(l_numMaterials==0){
                        return false;
                }
                m_materials.resize(l_numMaterials);

                for(int i=0; i<l_numMaterials;++i){  //lectura de los materiales
                        unsigned short l_materialType;
                        unsigned short l_NumChars;

						fread(&l_NumChars, sizeof(unsigned short), 1, l_meshFile);
						char *l_FileName = new char[l_NumChars+1];
						fread(&l_FileName[0], sizeof(char), l_NumChars+1, l_meshFile);
						CTexture *l_Texture=CEngine::GetSingletonPtr()->GetTextureManager()->LoadFile(l_FileName);
						m_Materials[i]->m_Textures[j].push_back(l_Texture);
						CHECKED_DELETE(l_FileName);
				

                } //lectura de los materiales
                
                for(int i=0; i<l_numMaterials;++i){  //lectura de los vertices  
                        unsigned short l_NumVertexs;
                        unsigned short l_NumIndexs;

                        unsigned short l_VertexSize=0;	
						l_VertexSize = sizeof(MV_VERTEX_TYPE_TEXTURE1);
                        void * l_Vtxs=NULL;
                        void * l_Idxs=NULL;
                        CRenderableVertexs *l_RV = NULL;

                        fread(&l_NumVertexs, sizeof(unsigned short), 1, l_meshFile);
                                                
                        l_Vtxs = malloc(l_VertexSize*l_NumVertexs);
                        fread(l_Vtxs, l_VertexSize, l_NumVertexs, l_meshFile);

                        fread(&l_NumIndexs, sizeof(unsigned short), 1, l_meshFile);
                        
                        l_Idxs = malloc(sizeof(unsigned short)*l_NumIndexs);
                        fread(l_Idxs, sizeof(unsigned short), l_NumIndexs, l_meshFile);
						//TODO VertexType
						//De momento siempre llega el tipo 19
                
						//TODO no entiendo, imagino que se debe utilizar alguna de las clases de TemplatedRenderableVertexs que extienden RenderableVertex
						l_RV =  new CRenderableVertexs<TNORMALTEXTURE1_VERTEX>(CEngine::GetSingletonPtr()->GetRenderManager(), l_Vtxs,l_Idxs,l_NumVertexs, l_NumIndexs);
                        m_RVs.push_back(l_RV);
                        free(l_Vtxs);
                        free(l_Idxs);
                        
                } //lectura de los vertices

                unsigned short l_footer;
                fread(&l_footer, sizeof(unsigned short), 1, l_meshFile);   //lectura del footer
                if(l_footer!=0xFF55){                     
                        return false;
                }
        }
		*/
        return true;
}

void CStaticMesh::Render(CRenderManager *RM) const{
//        for(size_t i=0; i<m_renderableVertexs.size(); ++i){
//               for(size_t j=0; j<m_materials[i]->size(); ++j)
//					m_materials[i]->m_Textures[j]->Activate(j);
//				m_renderableVertexs[i]->Render(RM, NULL,NULL);
//        }
}


bool CStaticMesh::Reload(){
        Destroy();
        if(Load(m_fileName))
                return true;
        else
                return false;
};

void CStaticMesh::Destroy(){
	for(int i=0;i<m_renderableVertexs.size();i++){
		delete m_renderableVertexs[i];
	}
	m_renderableVertexs.clear();
	for(int i=0;i<m_materials.size();i++){
		delete m_materials[i];
	}
	m_materials.clear();
}