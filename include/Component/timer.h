#pragma once

#include "component.h"

class Entity;

class Timer : public Component
{
private:
    float m_maxTimer = 1.f;
    float m_timer = 1.f;

public:
    Timer(Entity&);
    Timer(Entity&, float maxTimer);
    ~Timer() = default;

    operator bool();
    
    float   getTimer() {return m_timer;}
    float   getMaxTimer() {return m_maxTimer;}

    void    startTimer();
    void    setMaxTimer(float maxTimer);
    void    update(float deltaTime) override;
};
