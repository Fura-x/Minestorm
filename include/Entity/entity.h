#pragma once

#include <vector>
#include <memory>

#include "float2.h"
#include "primitive.h"
#include "component.h"
#include "collider.h"


class Game;
class Scorable;

class Entity
{
protected:
    std::vector<std::unique_ptr<Component>> components;
    Texture2D* m_texture;

    int m_scoring = 0;

public:
    Entity() = default;
    Entity(float2 pos, Texture2D* texture);
    virtual ~Entity() = default;

    referential2 m_ref;

    Collider*   m_colliderToAdd = nullptr;
    bool        m_isAlive = true;
    bool        m_showAABB = false;

    float2& pos() { return m_ref.origin; }
    float&  rotation() { return m_ref.angle; }

    void rotate(const float angle);

    int getScoring() const { return m_scoring; }

    template<typename T>
    void        addComponent(T** containers, const bool isActivate);
    Component*  getBackComponent() const {return components.back().get();}

    virtual void entityDead() {m_isAlive = false;};
    virtual void collisionHappens(Collider&) {};

    void         update(float deltaTime);
    virtual void draw() {};
    void         drawReferential();

};

template<typename T>
void    Entity::addComponent(T** containers, const bool isActivate)
{
    components.emplace_back(std::make_unique<T>(*this));
    *containers = reinterpret_cast<T*>(getBackComponent());
    Component* comp = dynamic_cast<Component*>(*containers);
    if (comp) comp->m_isActivate = isActivate;
}
