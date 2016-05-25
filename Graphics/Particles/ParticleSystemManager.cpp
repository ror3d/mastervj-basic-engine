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
		return CColor(node.GetVect4fProperty( prop.c_str(), Vect4f(defVal.x, defVal.y, defVal.z, defVal.w), false ));
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
		else if ( type == "color" )
		{
			color = getRangeValue<CColor>( prop );
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


void CParticleSystemManager::reload()
{
	destroy();
	Load(m_Filename);
}

void AddFloatValue(std::string name, range<float> value, FILE * file)
{
	std::string valStr = "";

	if (value.first == value.second)
	{
		valStr = "\t\t<value name=\"" + name + "\" value=\"" + std::to_string(value.first) + "\"/>\n";
	}
	else
	{
		valStr = "\t\t<range name=\"" + name + "\" lower=\"" + std::to_string(value.first) + "\" upper=\"" + std::to_string(value.second) + "\"/>\n";
	}

	fputs(valStr.c_str(), file);
}

void AddVect3fValue(std::string name, range<Vect3f> value, FILE * file)
{
	std::string valStr = "";

	if (value.first == value.second)
	{
		valStr = "\t\t<value name=\"" + name + "\" ";
		valStr += "value=\"" + std::to_string(value.first.x) + " ";
		valStr += std::to_string(value.first.y) + " ";
		valStr += std::to_string(value.first.z) + "\"/>\n";
	}
	else
	{
		valStr = "\t\t<range name=\"" + name + "\" ";
		valStr += "lower=\"" + std::to_string(value.first.x) + " ";
		valStr += std::to_string(value.first.y) + " ";
		valStr += std::to_string(value.first.z) + "\" ";
		valStr += "upper=\"" + std::to_string(value.second.x) + " ";
		valStr += std::to_string(value.second.y) + " ";
		valStr += std::to_string(value.second.z) + "\"/>\n";
	}

	fputs(valStr.c_str(), file);
}

void AddColorValue(std::string name, range<CColor> value, FILE * file)
{
	std::string valStr = "";

	if (value.first == value.second)
	{
		valStr = "\t\t<value name=\"" + name + "\" ";
		valStr += "value=\"" + std::to_string(value.first.x) + " ";
		valStr += std::to_string(value.first.y) + " ";
		valStr += std::to_string(value.first.z) + " ";
		valStr += std::to_string(value.first.w) + "\"/>\n";
	}
	else
	{
		valStr = "\t\t<range name=\"" + name + "\" ";
		valStr += "lower=\"" + std::to_string(value.first.x) + " ";
		valStr += std::to_string(value.first.y) + " ";
		valStr += std::to_string(value.first.z) + " ";
		valStr += std::to_string(value.first.w) + "\" ";
		valStr += "upper=\"" + std::to_string(value.second.x) + " ";
		valStr += std::to_string(value.second.y) + " ";
		valStr += std::to_string(value.second.z) + " ";
		valStr += std::to_string(value.second.w) + "\"/>\n";
	}

	fputs(valStr.c_str(), file);
}

std::string boolToString(bool value)
{
	return value ? "true" : "false";
}


void CParticleSystemManager::writeFile()
{
	FILE * file = fopen("Data\\particle_classes2.xml", "ab+");

	fputs("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n", file);

	fputs("<particle_classes>\n", file);

	for (auto part : m_resources)
	{
		CParticleSystemClass * particle = part.second;

		std::string particleStr = "\t<particle_class ";
		particleStr += "name=\"" + particle->getName() + "\" ";
		particleStr += "material=\"" + particle->material->getName() + "\" ";
		particleStr += "emit_rate=\"" + std::to_string(particle->emitRate) + "\" ";
		particleStr += "num_frames=\"" + std::to_string(particle->numFrames) + "\" ";
		particleStr += "time_per_frame=\"" + std::to_string(particle->timePerFrame) + "\" ";
		particleStr += "loop_frames=\"" + boolToString(particle->loopFrames) + "\">\n";

		fputs(particleStr.c_str(), file);

		AddFloatValue("size", particle->size, file);
		AddVect3fValue("velocity", particle->startVelocity, file);
		AddVect3fValue("acceleration", particle->acceleration, file);
		AddFloatValue("angle", particle->startAngle, file);
		AddFloatValue("angle_speed", particle->angleSpeed, file);
		AddFloatValue("angle_acceleration", particle->angleAcceleration, file);
		AddColorValue("color", particle->color, file);
		AddFloatValue("life", particle->life, file);

		fputs("\t</particle_class>\n", file);
	}

	fputs("</particle_classes>\n", file);
	
	fclose(file);

	DeleteFile("Data\\particle_classes.xml.bkp");
	rename("Data\\particle_classes.xml", "Data\\particle_classes.xml.bkp");
	rename("Data\\particle_classes2.xml", "Data\\particle_classes.xml");
}