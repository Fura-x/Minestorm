
#include <iostream>

#include "entity.h"
#include "enums.h"

#include "collision_engine.h"

#include "math_toolbox.h"

#include "collider.h"
#include "box_collider.h"
#include "circle_collider.h"
#include "convex_collider.h"
#include "concave_collider.h"

CollisionEngine::~CollisionEngine()
{
    m_colliders.clear();
}

void    CollisionEngine::setCollider(Collider* m_collider)
{
    m_colliders.push_back(m_collider);
}

void    CollisionEngine::setWaitingCollider(Collider* m_collider)
{
    m_waitingList.push_back(m_collider);
}

void    CollisionEngine::collisionCheck(Collider& colliderA, Collider& colliderB)
{
// cast collider to test
    bool isCollision = false;

    BoxCollider* boxA = dynamic_cast<BoxCollider*>(&colliderA);
    BoxCollider* boxB = dynamic_cast<BoxCollider*>(&colliderB);
    CircleCollider* circleA = dynamic_cast<CircleCollider*>(&colliderA);
    CircleCollider* circleB = dynamic_cast<CircleCollider*>(&colliderB);
    ConvexCollider* convexA = dynamic_cast<ConvexCollider*>(&colliderA);
    ConvexCollider* convexB = dynamic_cast<ConvexCollider*>(&colliderB);
    ConcaveCollider* concaveA = dynamic_cast<ConcaveCollider*>(&colliderA);
    ConcaveCollider* concaveB = dynamic_cast<ConcaveCollider*>(&colliderB);

//test collision depending on each collider type
    {
        //box && circle
        if (boxA && boxB)
            isCollision = boxsCollide(boxA->m_box, boxB->m_box);
        else if (boxA && circleB)
            isCollision = circleBoxCollide(circleB->m_circle, boxA->m_box);
        else if (boxB && circleA)
            isCollision = circleBoxCollide(circleA->m_circle, boxB->m_box);
        else if (circleA && circleB)
            isCollision = circlesCollide(circleA->m_circle, circleB->m_circle);
        //convex
        else if (boxA && convexB)
            isCollision = boxsCollide(boxA->m_box, convexB->m_AABB) ? PolygonsCollide(boxPolygon(boxA->m_box), convexB->m_convex) : false;
        else if (boxB && convexA)
            isCollision = boxsCollide(boxB->m_box, convexA->m_AABB) ? PolygonsCollide(boxPolygon(boxB->m_box), convexA->m_convex) : false;
        else if (circleA && convexB)
            isCollision = circleBoxCollide(circleA->m_circle, convexB->m_AABB) ? circlePolygonCollide(circleA->m_circle, convexB->m_convex) : false;
        else if (circleB && convexA)
            isCollision = circleBoxCollide(circleB->m_circle, convexA->m_AABB) ? circlePolygonCollide(circleB->m_circle, convexA->m_convex) : false;
        else if (convexA && convexB)
            isCollision = boxsCollide(convexA->m_AABB, convexB->m_AABB) ? PolygonsCollide(convexA->m_convex, convexB->m_convex) : false;
        //concave
        else if (boxA && concaveB)
            isCollision = boxsCollide(boxA->m_box, concaveB->m_AABB) ? PolygonsCollide(boxPolygon(boxA->m_box), concaveB->m_concave) : false;
        else if (boxB && concaveA)
            isCollision = boxsCollide(boxB->m_box, concaveA->m_AABB) ? PolygonsCollide(boxPolygon(boxB->m_box), concaveA->m_concave) : false;
        else if (circleA && concaveB)
            isCollision = circleBoxCollide(circleA->m_circle, concaveB->m_AABB) ? circlePolygonCollide(circleA->m_circle, concaveB->m_concave) : false;
        else if (circleB && concaveA)
            isCollision = circleBoxCollide(circleB->m_circle, concaveA->m_AABB) ? circlePolygonCollide(circleB->m_circle, concaveA->m_concave) : false;
        else if (convexA && concaveB)
            isCollision = boxsCollide(convexA->m_AABB, concaveB->m_AABB) ? PolygonsCollide(convexA->m_convex, concaveB->m_concave) : false;
        else if (convexB && concaveA)
            isCollision = boxsCollide(convexB->m_AABB, concaveA->m_AABB) ? PolygonsCollide(convexB->m_convex, concaveA->m_concave) : false;
        else if (concaveA && concaveB)
            isCollision = boxsCollide(concaveA->m_AABB, concaveB->m_AABB) ? PolygonsCollide(concaveA->m_concave, concaveB->m_concave) : false;
        else if (concaveB && concaveA)
            isCollision = boxsCollide(concaveB->m_AABB, concaveA->m_AABB) ? PolygonsCollide(concaveB->m_concave, concaveA->m_concave) : false;
    }

    if (isCollision)
    {
        colliderA.m_entity.collisionHappens(colliderB);
        colliderB.m_entity.collisionHappens(colliderA);
    }
}

void    CollisionEngine::update()
{
    int stop = 0;
    int size = (int)m_colliders.size()-1;
    if (size == 2)
        float test = 1;
    //verify collision between each collider
    for (int i = 0; i <= size; ++i)
    {
        auto& colliderA = m_colliders[i];

        for (int j = size; j >= stop+1; --j)
        {
            auto& colliderB = m_colliders[j]; 

            if ( colliderA->m_isActivate && colliderB->m_isActivate 
                && (colliderA->getTag() & colliderB->getMask()))
            {
                collisionCheck(*colliderA, *colliderB);
            }
        }

        if (!colliderA->m_entity.m_isAlive)
            m_eraseList.push_back(i);
        ++stop;
    }

    //add new collider created during the update
    while ( m_waitingList.size() > 0)
    {
        auto& waiting = m_waitingList[m_waitingList.size()-1];
        m_colliders.emplace_back(std::move(waiting));
        m_waitingList.pop_back();
    }
    //remove collider destroyed during the update
    while ( m_eraseList.size() > 0)
    {
        m_colliders.erase(m_colliders.begin() + m_eraseList[m_eraseList.size()-1]);
        m_eraseList.pop_back();
    }
}