#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "component.h"

class Entity;

class Collider : public Component
{
private:
    uint64_t m_tag = 0;
    uint64_t m_mask = 0;

public:
    Collider() = default;
    Collider(Entity& en) : Component(en) {}
    ~Collider() = default;

    //std::unique_ptr<Primitive2>  m_collider;

    void     setTag(uint64_t tag) { m_tag = tag; }
    void     setMask(uint64_t mask) { m_mask = mask; }
    void     setTagAndMask(uint64_t tag, uint64_t mask) { m_tag = tag; m_mask = mask; }
    uint64_t getTag() const { return m_tag; }
    uint64_t getMask() const { return m_mask; }

    void update(float deltaTime) override {}
};
