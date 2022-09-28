#pragma once

class Entity;
struct GPLib;

class Component
{
public:
    Component() = default;
    Component(Entity& en) : m_entity(en) {}
    virtual ~Component() = default;

    Entity&  m_entity;
    bool     m_isActivate = false;

    virtual void    update(float deltaTime){}
};


