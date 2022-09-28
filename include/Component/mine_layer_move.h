#pragma once

#include <vector>

#include "move.h"

class Timer;

class MineLayerMove : public Move
{
private:
	std::vector<Timer> m_stopTimer;
	float2 m_goal;

	bool* m_shouldCreate = nullptr;
	bool m_alreadyCreate = true;

public:
	MineLayerMove(Entity& en);
	MineLayerMove(Entity& en, float2 goal, bool* shouldCreate);
	~MineLayerMove() = default;

	void setParameter(float2 m_goal, bool* m_shouldCreate);

	void update(float deltaTime) override;
};