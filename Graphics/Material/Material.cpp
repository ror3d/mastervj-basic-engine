#include "Material.h"
#include "EffectTechnique.h"
#include "Texture/Texture.h"
#include "Context/ContextManager.h"

void CMaterial::destroy()
{
	for ( auto it = m_textures.begin(); it != m_textures.end(); ++it )
	{
		delete *it;
	}
	delete m_effectTechnique;
}

CMaterial::CMaterial( const std::string &Filename )
	: CNamed( "" )
{
	CXMLTreeNode TreeNode;
	TreeNode.LoadFile( Filename.c_str() );

	CXMLTreeNode l_materials = TreeNode["materials"];
	if ( l_materials.Exists() )
	{
		for ( int i = 0; i < l_materials.GetNumChildren(); ++i )
		{
			CXMLTreeNode l_material = l_materials( i );

			if ( l_material.GetName() == "material" )
			{
				setName(l_material.GetPszProperty( "name" ));
				m_effectTechnique->setName( l_material.GetPszProperty( "effect_technique" ) );

				if ( l_material( 0 ).GetName() == "texture" )
				{
					CXMLTreeNode l_text = l_material( 0 );
					CTexture* text = new CTexture();

					text->load( l_text.GetPszProperty( "filename" ) );

					m_textures.push_back( text );
				}
			}
		}
	}
}

CMaterial::~CMaterial()
{
	destroy();
}

void CMaterial::apply()
{
	// TODO
}


