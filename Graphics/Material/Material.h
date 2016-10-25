#ifndef MATERIAL_H
#define MATERIAL_H

#include <Base/Utils/Named.h>
#include <Base/Utils/Utils.h>
#include <Base/Math/Color.h>
#include <vector>

#include <Graphics/Material/MaterialParameter.h>

class CXMLTreeNode;
class CEffectTechnique;
class CTexture;
class CRenderableObjectTechnique;

class CMaterial : public CNamed
{
private:
	static int GetTextureStage(const std::string& type)
	{
		const std::string stages[] = {
			"albedo",
			"normal",
			"lightmap",
			"lightmap2",
			"lightmap3",
			"environment",
			"shadowmap"
		};
		int i = 0;
		for (auto const &t : stages)
		{
			if (t == type) return i;
			++i;
		}
		return -1;
	}

	std::vector<std::pair<uint8, CTexture *>> m_textures;
	std::vector<CMaterialParameter *> m_Parameters;
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
public:
	CMaterial(CXMLTreeNode &TreeNode);
	virtual ~CMaterial();
	void destroy();

	virtual void apply(CRenderableObjectTechnique *RenderableObjectTechnique = NULL);

	CRenderableObjectTechnique * getRenderableObjectTechique(){ return m_RenderableObjectTechnique; }

	std::vector<CMaterialParameter *> * getParameters(){ return &m_Parameters;  }

	CTexture *GetTexture( uint32 id );
	CTexture *GetTextureAtStage( uint32 stage );

	template<typename T>
	inline void SetParameterValue( const std::string& paramName, T value )
	{
		for (auto &p : m_Parameters)
		{
			if ( p->getName() == paramName )
			{
				CTemplatedMaterialParameter<T>* pd = dynamic_cast<CTemplatedMaterialParameter<T>*>( p );
				if ( pd )
				{
					pd->setValue( value );
					break;
				}
			}
		}
	}

	template<typename T>
	inline T GetParameterValue( const std::string& paramName ) const
	{
		for (const auto &p : m_Parameters)
		{
			if ( p->getName() == paramName )
			{
				CTemplatedMaterialParameter<T>* pd = dynamic_cast<CTemplatedMaterialParameter<T>*>( p );
				if ( pd )
				{
					return pd->getValue();
					return;
				}
			}
		}
		return T();
	}

	void SetTextureAtStage(CTexture* tex, uint32 stage);
};

#endif
