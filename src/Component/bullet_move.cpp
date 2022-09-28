
#include "entity.h"
#include "game.h"
#include "utils.h"
#include "bullet_move.h"


BulletMove::BulletMove(Entity& en)
    :Move(en)
{
    m_speed = 300.f;
    m_maxDistance = GAME_HEIGHT / 2.f;
}

BulletMove::BulletMove(Entity& en, float2 dir)
: Move(en)
{
    m_dir = dir;
}

void    BulletMove::update(float deltaTime)
{
    auto& pos = m_entity.pos();
    float2 moveVect = m_dir * m_speed * deltaTime;

    pos += moveVect;

    m_distance += moveVect.length();
    if (m_maxDistance != 0.f && m_distance > m_maxDistance)
    {
        m_entity.m_isAlive = false;
    }

    screenTeleport();
}


