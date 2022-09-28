
#include "entity.h"
#include "mine_layer.h"

#include "mine_layer_move.h"
#include "timer.h"


static bool isOverGoal(float2 pos, float2 dir, float2 goal)
{
	//if there is a change during move according to the direction, so it passed the goal
	bool befX = pos.x < goal.x, befY = pos.y < goal.y;
	bool aftX = pos.x + dir.x < goal.x, aftY = pos.y + dir.y < goal.y;

	if (fabsf(dir.x) > fabsf(dir.y))
		return befX != aftX;

	return befY != aftY;

}

MineLayerMove::MineLayerMove(Entity& en)
	:Move(en)
{
	m_stopTimer.push_back(Timer(en, 2.f));
	m_stopTimer[0].m_isActivate = false;

	m_speed = 50.f;
}


MineLayerMove::MineLayerMove(Entity& en, float2 goal, bool* shouldCreate)
	:Move(en), m_goal(goal), m_shouldCreate(shouldCreate)
{
	m_stopTimer.push_back(Timer(en, 2.f));
	m_stopTimer[0].m_isActivate = false;

	m_speed = 150.f;
}

void MineLayerMove::setParameter(float2 goal, bool* shouldCreate)
{
	m_goal = goal;
	m_shouldCreate = shouldCreate;
	m_dir = (m_goal - m_dir).normalize();
}


void MineLayerMove::update(float deltaTime)
{
	if (m_stopTimer[0].m_isActivate)
		m_stopTimer[0].update(deltaTime);

//Create a spawnPoint
	if (!m_stopTimer[0].m_isActivate && !m_alreadyCreate)
	{
		m_alreadyCreate = true;
		*m_shouldCreate = rand() % 2 == 0;
	}

	if (isOverGoal(m_entity.pos(), m_dir, m_goal) && !m_stopTimer[0].m_isActivate)
	{
		//Stop move and wait 1 seconde before create spawn point
		MineLayer* mother = dynamic_cast<MineLayer*>(&m_entity);

		m_alreadyCreate = false;

		m_stopTimer[0].startTimer();
		m_goal = getRandomPos();
	}
	else if (!m_stopTimer[0].m_isActivate)
	{
		m_dir = (m_goal - m_entity.pos()).normalize();
		m_entity.pos() += m_dir * m_speed * deltaTime;
	}
}