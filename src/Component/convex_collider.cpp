
#include "utils.h"
#include "game.h"
#include "entity.h"

#include "convex_collider.h"

ConvexCollider::ConvexCollider(Entity& en)
	:Collider(en)
{}

ConvexCollider::ConvexCollider(Entity& en, convex_polygon convex)
	:Collider(en), m_convex(convex)
{
	m_AABB = getPolygonAABB(m_convex);
}

ConvexCollider::~ConvexCollider()
{
	delete[] m_convex.pts;
}


void ConvexCollider::setCollider(convex_polygon convex)
{
	m_convex = convex;
	m_curRotation = m_entity.rotation();
	m_curEntityPos = m_entity.pos();

	setAABB();
}

void ConvexCollider::setAABB()
{
	m_AABB = getPolygonAABB(m_convex);
}

void ConvexCollider::update(float deltaTime)
{
	float2 lastMove = m_curEntityPos != m_entity.pos() ? m_entity.pos() - m_curEntityPos : m_curEntityPos;

	for (int i = 0; i < m_convex.nPts; ++i)
	{
		auto& point = m_convex.pts[i];
		point += lastMove;

		if (m_curRotation != m_entity.m_ref.angle)
			point = pointRotation(point, m_entity.pos(), m_entity.m_ref.angle - m_curRotation);

	}

	if (m_curRotation != m_entity.m_ref.angle || m_curEntityPos != m_entity.pos())
		setAABB();

	m_curRotation = m_entity.rotation();
	m_curEntityPos = m_entity.pos();
}