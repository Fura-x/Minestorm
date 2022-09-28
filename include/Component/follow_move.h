#pragma once

#include "move.h"

class Entity;

class FollowMove : public Move
{
private:
	float2* m_goal = nullptr;

public:
	FollowMove(Entity& en);
	FollowMove(Entity& en, float speed, float2* goal);
	~FollowMove() = default;

	void setGoal(float2* goal) { m_goal = goal; }

	void update(float deltaTime) override;
};