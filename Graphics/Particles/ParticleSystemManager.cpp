#include "ParticleSystemManager.h"

#include <Base/XML/XMLTreeNode.h>
#include <Core/Engine/Engine.h>
#include <Graphics/Material/MaterialManager.h>
#include <Graphics/Material/Material.h>
#include <Windows.h>

namespace
{
	template<typename T>
	T getProp( const CXMLTreeNode& node, std::string prop, T defVal )
	{
		return T();
	}

	template<>
	float getProp( const CXMLTreeNode& node, std::string prop, float defVal )
	{
		return node.GetFloatProperty( prop.c_str(), defVal, false );
	}

	template<>
	Vect3f getProp( const CXMLTreeNode& node, std::string prop, Vect3f defVal )
	{
		return node.GetVect3fProperty( prop.c_str(), defVal, false );
	}

	template<>
	CColorSpace getProp(const CXMLTreeNode& node, std::string prop, CColorSpace defVal)
	{
		return CColorSpace(node.GetVect4fProperty(prop.c_str(), Vect4f(defVal.x, defVal.y, defVal.z, defVal.w), false), node.GetBoolProperty("hsl", false, false));
	}

	template<typename T>
	range<T> getRangeValue( const CXMLTreeNode& node )
	{
		std::string n = node.GetName();
		if ( n == "range" )
		{
			T lower = getProp<T>( node, "lower", T() );
			T upper = getProp<T>( node, "upper", T() );
			return make_range( lower, upper );
		}
		else if ( n == "value" )
		{
			T val = getProp<T>( node, "value", T() );
			return make_range( val );
		}
		else
		{
			DEBUG_ASSERT( !"This should not happen!" );
			return range<T>();
		}
	}
}

CParticleSystemClass::CParticleSystemClass(const CXMLTreeNode& node)
	: CNamed(node)
{
	std::string matName = node.GetPszProperty( "material", "", false );
	DEBUG_ASSERT( matName.size() > 0 );
	material = CEngine::GetSingleton().getMaterialManager()->get(matName);

	emitRate = node.GetFloatProperty("emit_rate", 0, false);
	DEBUG_ASSERT( emitRate > 0 );

	numFrames = node.GetIntProperty("num_frames", 1, false);
	loopFrames = node.GetBoolProperty( "loop_frames", false, false );
	colorInterpolation = node.GetBoolProperty("color_interpolation", false, false);
	
	for ( int i = 0; i < node.GetNumChildren(); ++i )
	{
		CXMLTreeNode prop = node( i );
		std::string name = prop.GetName();
		if (!( name == "range" || name == "value" ))
		{
			continue;
		}
		std::string type = prop.GetPszProperty( "name", "", false );
		if ( type == "size" )
		{
			size = getRangeValue<float>( prop );
		}
		else if ( type == "velocity" )
		{
			startVelocity = getRangeValue<Vect3f>( prop );
		}
		else if ( type == "acceleration" )
		{
			acceleration = getRangeValue<Vect3f>( prop );
		}
		else if ( type == "angle" )
		{
			startAngle = getRangeValue<float>( prop );
		}
		else if ( type == "angle_speed" )
		{
			angleSpeed = getRangeValue<float>( prop );
		}
		else if ( type == "angle_acceleration" )
		{
			angleAcceleration = getRangeValue<float>( prop );
		}
		else if (type == "color")
		{
			color = getRangeValue<CColorSpace>(prop);
		}
		else if ( type == "life" )
		{
			life = getRangeValue<float>( prop );
		}
	}
}

CParticleSystemClass::~CParticleSystemClass()
{

}

void CParticleSystemManager::Load(const std::string &Filename)
{
	m_Filename = Filename;

	CXMLTreeNode l_XML;
	if (!l_XML.LoadFile(Filename.c_str()))
	{
		DEBUG_ASSERT( !"Could not load file" );
		return;
	}

	CXMLTreeNode l_Systems = l_XML["particle_classes"];
	if ( !l_Systems.Exists() )
	{
		DEBUG_ASSERT( l_Systems.Exists() );
		return;
	}

	for (int i = 0; i < l_Systems.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_System = l_Systems(i);

		if (l_System.GetName() == std::string("particle_class"))
		{
			CParticleSystemClass * c = new CParticleSystemClass(l_System);

			add(c->getName(), c);
		}
	}

}

CParticleSystemManager::~CParticleSystemManager()
{
}

void CParticleSystemManager::reload()
{
	destroy();
	Load(m_Filename);
}
