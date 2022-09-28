
#include "game.h"
#include "entity.h"

#include "timer.h"

Timer::Timer(Entity& en)
    :Component(en)
{}

Timer::Timer(Entity& en, float maxTimer)
: Component(en), m_maxTimer(maxTimer), m_timer(maxTimer) {}

Timer::operator bool()
{
    return m_isActivate;
}

void    Timer::startTimer()
{
    m_timer = m_maxTimer;
    m_isActivate = true;
}

void    Timer::setMaxTimer(float maxTimer)
{
    if (maxTimer < 0.001f)
        maxTimer = 0.001f;

    m_maxTimer = maxTimer;
    m_timer = maxTimer;
}

void    Timer::update(float deltaTime)
{
    m_timer -= deltaTime;

    m_isActivate = m_timer >= 0;
}