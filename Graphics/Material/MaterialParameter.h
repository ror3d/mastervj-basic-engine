#ifndef MATERIAL_PARAMETER_H
#define MATERIAL_PARAMETER_H

#include "Utils/Named.h"

class CMaterialParameter : public CNamed
{
public:
	enum TMaterialType
	{
		FLOAT = 0,
		VECT2F,
		VECT3F,
		VECT4F
	};
protected:
	TMaterialType m_MaterialType;
	std::string m_paramValues;
public:
	CMaterialParameter(CXMLTreeNode &TreeNode,CMaterialParameter::TMaterialType MaterialType);
	virtual ~CMaterialParameter();
	virtual void Apply() = 0;
	TMaterialType getMaterialType(){ return m_MaterialType; }
	std::string getParamValues(){ return m_paramValues;  }
};

template<typename T>
class CTemplatedMaterialParameter : public CMaterialParameter
{
private:
	T m_Value;
	void *m_EffectAddress;
public:
	CTemplatedMaterialParameter(CXMLTreeNode &TreeNode, void *dir, const T &Value, CMaterialParameter::TMaterialType MaterialType)
		: CMaterialParameter(TreeNode, MaterialType)
		, m_Value(Value)
		, m_EffectAddress(dir)
	{
	}

	virtual ~CTemplatedMaterialParameter(){
	}

	inline T * getValue()
	{
		return &m_Value;
	}

	inline void setValue(T val)
	{
		return &m_Value;
	}

	void Apply()
	{
		memcpy(m_EffectAddress, &m_Value, sizeof(T));
	}
};

#endif