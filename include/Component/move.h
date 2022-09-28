#pragma once

#include "defines.h"

#include "component.h"
#include "float2.h"

class Entity;

class Move : public Component
{
protected:
	float2	m_dir = { 1.f, 0.f };
	float	m_speed = 100.f;
	float	m_distance = 0.f;
	float	m_maxDistance = 100.f;
	bool	m_nonStop = false;

public:
	Move(Entity& en) :Component(en) {};
	Move(Entity& en, float speed) :Component(en), m_speed(speed) {};
	~Move() = default;

	void setSpeed(const float sp) { m_speed = sp; }
	void setDirection(const float2 dir) { m_dir = dir; }
	void setMaxDistance(const float dist) { m_maxDistance = dist; }


	float2 getRandomPos();


	virtual void update(float deltaTime) = 0;
	void		 screenTeleport();
};

inline float2 Move::getRandomPos()
{
	return { (float)(rand() % (GAME_WIDTH - 150) + GAME_X_MIN + 150), (float)(rand() % (GAME_HEIGHT - 200) + 200) };
}

inline void		 Move::screenTeleport()
{
	//teleport entity to the opposite side of the game screen if needed 

	auto& pos = m_entity.pos();

	if (pos.x < SCREEN_WIDTH / 2 - GAME_WIDTH / 2 + 30)
		pos.x = SCREEN_WIDTH / 2 + GAME_WIDTH / 2 - 30;
	else if (pos.x > SCREEN_WIDTH / 2 + GAME_WIDTH / 2 - 30)
		pos.x = SCREEN_WIDTH / 2 - GAME_WIDTH / 2 + 30;

	if (pos.y < 30)
		pos.y = GAME_HEIGHT - 50;
	else if (pos.y > GAME_HEIGHT - 50)
		pos.y = 30;
}
