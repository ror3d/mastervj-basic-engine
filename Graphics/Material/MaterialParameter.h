#ifndef MATERIAL_PARAMETER_H
#define MATERIAL_PARAMETER_H

#include "Utils\Named.h"
#include "Material\Material.h"
#include "Utils\Utils.h"

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
public:
	CMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode,
		CMaterialParameter::TMaterialType MaterialType);
	virtual ~CMaterialParameter();
	virtual void Apply() = 0;
	virtual void * GetValueAddress() const = 0;
	UAB_GET_PROPERTY(TMaterialType, MaterialType);
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
	CTemplatedMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode,
		const T &Value, CMaterialParameter::TMaterialType MaterialType);
	virtual ~CTemplatedMaterialParameter();
	void Apply();
	void * GetValueAddress() const;
};

#endif