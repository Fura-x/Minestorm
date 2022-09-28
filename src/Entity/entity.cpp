
#include "defines.h"

#include "entity.h"
#include "game.h"
#include "box_collider.h"
#include "circle_collider.h"

Entity::Entity(float2 pos, Texture2D* texture)
    :m_texture(texture)
{
    m_ref.origin = pos;
}


void    Entity::rotate(const float angle)
{
    m_ref.angle += angle;
    m_ref.unitX = m_ref.unitX.getRotated(angle);
    m_ref.unitY = m_ref.unitY.getRotated(angle);
}

void    Entity::drawReferential()
{
    DrawLineEx(pos(), posLocalToGlobal(m_ref.unitY * 50.f, m_ref), 1.f, RED);
    DrawLineEx(pos(), posLocalToGlobal(m_ref.unitX * 50.f, m_ref), 1.f, BLUE);
}

void    Entity::update(float deltaTime)
{
    for (auto& component : components)
    {
        if (component->m_isActivate)
            component->update(deltaTime);
    }
}
