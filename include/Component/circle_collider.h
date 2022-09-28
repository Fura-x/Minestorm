#pragma once

#include "collider.h"
#include "primitive.h"

class Entity;

class CircleCollider : public Collider
{
private:

public:
    CircleCollider(Entity&);
    CircleCollider(Entity&, circle);
    ~CircleCollider() = default;

    circle   m_circle = { {0.f, 0.f}, 0.f };

    void    update(float deltaTime) override;
    void    setRange(float radius);
};