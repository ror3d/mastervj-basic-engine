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
	CColor getProp( const CXMLTreeNode& node, std::string prop, CColor defVal )
	{
		return CColor(node.GetVect4fProperty(prop.c_str(), Vect4f(defVal.x, defVal.y, defVal.z, defVal.w), false), node.GetBoolProperty("hsl", false, false));
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
	timePerFrame = node.GetFloatProperty("time_per_frame", 0, false);
	loopFrames = node.GetBoolProperty( "loop_frames", false, false );
	
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
			color = getRangeValue<CColor>(prop);
		}
		else if ( type == "life" )
		{
			life = getRangeValue<float>( prop );
		}
	}
}


CParticleSystemClass::CParticleSystemClass(const CParticleSystemClass& original)
	: CNamed(original.getName())
	, material(original.material)
	, numFrames(original.numFrames)
	, timePerFrame(original.timePerFrame)
	, loopFrames(original.loopFrames)
	, emitRate(original.emitRate)
	, size(original.size)
	, life(original.life)
	, startVelocity(original.startVelocity)
	, acceleration(original.acceleration)
	, startAngle(original.startAngle)
	, angleSpeed(original.angleSpeed)
	, angleAcceleration(original.angleAcceleration)
	, color(original.color)
{

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

void AddFloatRangeValue(CXMLTreeNode &xml, std::string name, range<float> value)
{
	if (value.first == value.second)
	{
		xml.StartElement("value");
		xml.WritePszProperty("name", name.c_str());
		xml.WriteFloatProperty("value", value.first);
	}
	else
	{
		xml.StartElement("range");
		xml.WritePszProperty("name", name.c_str());
		xml.WriteFloatProperty("lower", value.first);
		xml.WriteFloatProperty("upper", value.second);
	}

	xml.EndElement();
}

void AddVect3fRangeValue(CXMLTreeNode &xml, std::string name, range<Vect3f> value)
{
	if (value.first == value.second)
	{
		xml.StartElement("value");
		xml.WritePszProperty("name", name.c_str());
		xml.WriteVect3fProperty("value", value.first);
	}
	else
	{
		xml.StartElement("range");
		xml.WritePszProperty("name", name.c_str());
		xml.WriteVect3fProperty("lower", value.first);
		xml.WriteVect3fProperty("upper", value.second);
	}

	xml.EndElement();
}

void AddColorRangeValue(CXMLTreeNode &xml, std::string name, CParticleSystemClass* particle)
{
	if (particle->color.first == particle->color.second)
	{
		xml.StartElement("value");
		xml.WritePszProperty("name", name.c_str());
		xml.WriteVect4fProperty("value", particle->color.first);
	}
	else
	{
		xml.StartElement("range");
		xml.WritePszProperty("name", name.c_str());
		xml.WriteVect4fProperty("lower", particle->color.first);
		xml.WriteVect4fProperty("upper", particle->color.second);
	}

	xml.WriteBoolProperty("hsl", particle->color.first.HSL);

	xml.EndElement();
}

std::string boolToString(bool value)
{
	return value ? "true" : "false";
}

void CParticleSystemManager::writeFile()
{
	CXMLTreeNode NewXML;

	if (NewXML.StartNewFile("Data\\particle_classes2.xml"))
	{
		NewXML.StartElement("particle_classes");

		for (auto part : m_resources)
		{
			CParticleSystemClass * particle = part.second;
			
			NewXML.StartElement("particle_class");

			NewXML.WritePszProperty("name", particle->getName().c_str());
			NewXML.WritePszProperty("material", particle->material->getName().c_str());
			NewXML.WriteFloatProperty("emit_rate", particle->emitRate);
			NewXML.WriteIntProperty("num_frames", particle->numFrames);
			NewXML.WriteFloatProperty("time_per_frame", particle->timePerFrame);
			NewXML.WriteBoolProperty("loop_frames", particle->loopFrames);

			AddFloatRangeValue(NewXML, "size", particle->size);
			AddVect3fRangeValue(NewXML, "velocity", particle->startVelocity);
			AddVect3fRangeValue(NewXML, "acceleration", particle->acceleration);
			AddFloatRangeValue(NewXML, "angle", particle->startAngle);
			AddFloatRangeValue(NewXML, "angle_speed", particle->angleSpeed);
			AddFloatRangeValue(NewXML, "angle_acceleration", particle->angleAcceleration);
			AddColorRangeValue(NewXML, "color", particle);

			AddFloatRangeValue(NewXML, "life", particle->life);

			NewXML.EndElement();
		}

		NewXML.EndElement();
		NewXML.EndNewFile();
	}

	DeleteFile("Data\\particle_classes.xml.bkp");
	rename("Data\\particle_classes.xml", "Data\\particle_classes.xml.bkp");
	rename("Data\\particle_classes2.xml", "Data\\particle_classes.xml");
}