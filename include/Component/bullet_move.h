#pragma once

#include "move.h"
#include "float2.h"

class Entity;

class BulletMove : public Move
{
private:
    
public:
    BulletMove(Entity&);
    BulletMove(Entity&, float2);
    ~BulletMove() = default;

    void    update(float deltaTime) override;
};
