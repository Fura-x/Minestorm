
#include "entity.h"
#include "spawn.h"

SpawnComponent::SpawnComponent(Entity& en)
	:Component(en)
{}


SpawnComponent::SpawnComponent(Entity& en, float time, std::function<void(void)> event)
	:Component(en), m_event(event)
{
//add timer
	m_timer.push_back(Timer(en, time));
	m_timer[0].startTimer();
}

void SpawnComponent::setParameter(float time, std::function<void(void)> event)
{
	m_timer.push_back(Timer(m_entity, time));
	m_timer[0].startTimer();

	m_event = event;
}

void SpawnComponent::update(float deltaTime)
{
	m_timer[0].update(deltaTime);

	if (!m_timer[0].m_isActivate)
	{
		m_event();
		m_timer[0].startTimer();
	}
}

