
#include "entity.h"

#include "enums.h"
#include "box_collider.h"

BoxCollider::BoxCollider(Entity& en)
    :Collider(en)
{}


BoxCollider::BoxCollider(Entity& en, box _box) 
 :Collider(en),
 m_box(_box) 
 {}

 void BoxCollider::update(float deltaTime)
 {
     m_box.center= m_entity.pos();
 }

 void   BoxCollider::setRange(float w, float h)
 {
     m_box.width_ha = w;
     m_box.height_ha = h;
 }