#pragma once

#include <vector>

#include "HelperTypes.h"

#include "3DElement.h"
#include "Material/Material.h"

class CRenderManager;
class CRenderableVertexs;

class CRenderableObject : public CNamed, public C3DElement
{
public:

	struct SSubmesh
	{
		const CRenderableVertexs* vertices;
		std::string material;
	};

	CRenderableObject() : CNamed("") {}

	void AddSubmesh(const CRenderableVertexs* _Vertices, const std::string& _Material, float _BoundingRadius, const Vect3f& _BoundingBoxMin, const Vect3f& _BoundingBoxMax);

	void SetTransform(const SPositionOrientation& _transform) { m_Transform = _transform; }
	void SetPosition(const Vect3f& _position) { m_Transform.Position = _position; }
	void SetYaw(float _yaw) { m_Transform.Yaw = _yaw; }
	void SetPitch(float _pitch) { m_Transform.Pitch = _pitch; }
	void SetRoll(float _roll) { m_Transform.Roll = _roll; }

	int GetNumSubmeshes() const { return m_Submeshes.size(); }
	const SSubmesh& GetSubmesh(size_t i) const { return m_Submeshes[i]; }
	const SPositionOrientation& GetTransform() const { return m_Transform; }
	SPositionOrientation* GetPtrTransform() { return &m_Transform; }

	float GetBoundingRadius() const { return m_BoundingRadius; }
	Vect3f GetBoundingMin() const { return m_BoundingBoxMin; }
	Vect3f GetBoundingMax() const { return m_BoundingBoxMax; }

	virtual void Update(float ElapsedTime) {}
	virtual void Render(CRenderManager *rm) {}

private:

	std::vector<SSubmesh> m_Submeshes;
	SPositionOrientation m_Transform;

	float m_BoundingRadius;
	Vect3f m_BoundingBoxMin, m_BoundingBoxMax;
};

