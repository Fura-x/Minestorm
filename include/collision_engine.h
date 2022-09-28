#pragma once

#include <vector>
#include <memory>

#include "collider.h"

class CollisionEngine
{
private:
    std::vector<Collider*> m_waitingList;
    std::vector<int> m_eraseList;

public:
    CollisionEngine() = default;
    ~CollisionEngine();

    std::vector<Collider*> m_colliders;

    void    setCollider(Collider*);
    void    setWaitingCollider(Collider*);
    void    test() {};
    void    collisionCheck(Collider&, Collider&);
    void    update();
};
