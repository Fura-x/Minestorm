#pragma once

#include "collider.h"
#include "primitive.h"

class Entity;

class BoxCollider : public Collider
{
private:

public:
    BoxCollider(Entity&);
    BoxCollider(Entity&, box);
    ~BoxCollider() = default;

    box     m_box = { {0.f, 0.f}, 0.f, 0.f };

    void    update(float deltaTime) override;
    void    setRange(float w, float h);
};