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
	CMaterialParameter(const CMaterialParameter& param);
	virtual ~CMaterialParameter();
	virtual void Apply() = 0;
	TMaterialType getMaterialType(){ return m_MaterialType; }
	std::string getParamValues(){ return m_paramValues;  }

	//TODO: LUA
	/*virtual CEmptyPointerClass * GetValueLuaAddress() const {
		return
			(CEmptyPointerClass *)GetValueAddress();
	}*/
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

	CTemplatedMaterialParameter(const CTemplatedMaterialParameter<T>& param)
		: CMaterialParameter(param)
		, m_Value(param.m_Value)
		, m_EffectAddress(param.m_EffectAddress)
	{
	}

	virtual ~CTemplatedMaterialParameter()
	{
	}

	T * getValue(){
		return &m_Value;
	}

	void Apply()
	{
		memcpy(m_EffectAddress, &m_Value, sizeof(T));
	}
};

#endif