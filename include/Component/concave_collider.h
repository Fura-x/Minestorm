#pragma once

#include <vector>

#include "primitive.h"
#include "convex_collider.h"

class Entity;

class ConcaveCollider : public Collider
{
private:
	float m_curRotation = 0.f;
	float2	m_curEntityPos;

public:
	concave_polygon	m_concave;
	box				m_AABB;

	ConcaveCollider(Entity&);
	ConcaveCollider(Entity&, concave_polygon);
	~ConcaveCollider();

	void setCollider(concave_polygon);
	void setAABB();

	void update(float deltaTime) override;
};
