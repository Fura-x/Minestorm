
#include "entity.h"

#include "enums.h"
#include "circle_collider.h"

CircleCollider::CircleCollider(Entity& en)
    :Collider(en)
{}

CircleCollider::CircleCollider(Entity& en, circle c)
: Collider(en), m_circle(c) {}

 void CircleCollider::update(float deltaTime)
 {
     m_circle.center = m_entity.pos();
 }

 void   CircleCollider::setRange(float radius)
 {
     m_circle.radius = radius;
 }