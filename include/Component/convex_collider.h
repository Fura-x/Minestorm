#pragma once

#include "primitive.h"
#include "collider.h"

class Entity;

class ConvexCollider : public Collider
{
private:
	float	m_curRotation = 0.f;
	float2	m_curEntityPos;

public:
	convex_polygon	m_convex;
	box				m_AABB;

	ConvexCollider(Entity&);
	ConvexCollider(Entity&, convex_polygon convex);
	~ConvexCollider();

	void setCollider(convex_polygon convex);
	void setAABB();

	void update(float deltaTime) override;
};