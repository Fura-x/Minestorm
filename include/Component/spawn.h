#pragma once

#include <vector>
#include <functional>

#include "component.h"
#include "timer.h"

class Entity;

class SpawnComponent : public Component
{
private:
	std::function<void(void)>	m_event;
	std::vector<Timer>			m_timer;

public:
	SpawnComponent(Entity& en);
	SpawnComponent(Entity& en, float time, std::function<void(void)> event);
	~SpawnComponent() = default;

	void setParameter(float time, std::function<void(void)> event);
	void startTimer() { m_timer[0].startTimer(); }

	void update(float deltaTime) override;
};