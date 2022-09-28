
#include "defines.h"
#include "entity.h"

#include "utils.h"
#include "follow_move.h"

static float getTranslateX(float x)
{
	float translate = x;
	if (x < SCREEN_WIDTH / 2)
		translate = x + GAME_WIDTH;

	return translate;
}

static float getTranslateY(float y)
{
	float translate = y;
	if (y < SCREEN_WIDTH / 2)
		translate = y + GAME_WIDTH;

	return translate;
}

FollowMove::FollowMove(Entity& en)
	:Move(en)
{}

FollowMove::FollowMove(Entity& en, float speed, float2* goal)
	:Move(en, speed), m_goal(goal)
{}

void FollowMove::update(float deltaTime)
{
	if (m_goal)
	{
		auto pos1 = m_entity.pos(), goal1 = *m_goal;
		float dist1 = (goal1 - pos1).length();

		float2 pos2 = { getTranslateX(pos1.x), pos1.y }, goal2 = { getTranslateX(goal1.x), goal1.y };
		float dist2 = (goal2 - pos2).length();
		float2 pos3 = { pos1.x, getTranslateY(pos1.y) }, goal3 = { goal1.x, getTranslateY(goal1.y) };
		float dist3 = (goal3 - pos3).length();
		float2 pos4 = { getTranslateX(pos1.x), getTranslateY(pos1.y) }, goal4 = { getTranslateX(goal1.x), getTranslateY(goal1.y) };
		float dist4 = (goal4 - pos4).length();

		float min = utils::getMin(dist1, dist2);
		min = utils::getMin(min, dist3);
		min = utils::getMin(min, dist4);

		if (dist1 == min)
			m_dir = (goal1 - pos1).normalize() * m_speed * deltaTime;
		else if (dist2 == min)
			m_dir = (goal2 - pos2).normalize() * m_speed * deltaTime;
		else if (dist3 == min)
			m_dir = (goal3 - pos3).normalize() * m_speed * deltaTime;
		else
			m_dir = (goal4 - pos4).normalize() * m_speed * deltaTime;

	}

	m_entity.pos() += m_dir;

	screenTeleport();
}