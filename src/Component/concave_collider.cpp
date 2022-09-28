
#include "utils.h"
#include "game.h"
#include "entity.h"

#include "concave_collider.h"

ConcaveCollider::ConcaveCollider(Entity& en)
	:Collider(en)
{}

ConcaveCollider::ConcaveCollider(Entity& en, concave_polygon concave)
	:Collider(en), m_concave(concave)
{
	setAABB();
}

ConcaveCollider::~ConcaveCollider()
{
	for (int convex = 0; convex < m_concave.polygonCount; ++convex)
	{
		delete[](m_concave.polygons[convex].pts);
	}
	delete[](m_concave.polygons);
}

void ConcaveCollider::setCollider(concave_polygon collider)
{
	m_concave = collider;
	m_curRotation = m_entity.rotation();
	m_curEntityPos = m_entity.pos();
	setAABB();
}

void ConcaveCollider::setAABB()
{
	float minX = 1000.f, maxX = -1000.f, minY = 1000.f, maxY = -1000.f;
	for (int i = 0; i < m_concave.polygonCount; ++i)
	{
		box box = getPolygonAABB(m_concave.polygons[i]);
		minX = utils::getMin(minX, box.center.x - box.width_ha);
		maxX = utils::getMax(maxX, box.center.x + box.width_ha);
		minY = utils::getMin(minY, box.center.y - box.height_ha);
		maxY = utils::getMax(maxY, box.center.y + box.height_ha);
	}
	float2 center = { (minX + maxX) / 2, (minY + maxY) / 2 };
	m_AABB = { center , center.x - minX, center.y - minY };
}

void ConcaveCollider::update(float deltaTime)
{
	float2 lastMove = m_curEntityPos != m_entity.pos() ? m_entity.pos() - m_curEntityPos : m_curEntityPos;

	for (int convex = 0; convex < m_concave.polygonCount; ++convex)
	{
		for (int pt = 0; pt < m_concave.polygons[convex].nPts; ++pt)
		{
			auto& point = m_concave.polygons[convex].pts[pt];
			point += lastMove;

			if (m_curRotation != m_entity.m_ref.angle)
				point = pointRotation(point, m_entity.pos(), m_entity.m_ref.angle - m_curRotation);
		}
	}
	if (m_curRotation != m_entity.m_ref.angle || m_curEntityPos != m_entity.pos())
		setAABB();

	m_curRotation = m_entity.rotation();
	m_curEntityPos = m_entity.pos();
}