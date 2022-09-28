
#include <string>

#include "utils.h"
#include "game.h"
#include "input_engine.h"
#include "entity.h"

#include "player_move.h"

PlayerMove::PlayerMove(Entity& en)
	:Move(en)
{
	m_acceleration = m_thrust / m_weight;
}

PlayerMove::PlayerMove(Entity& en, float speed, float rotationSpeed)
	:Move(en, speed), m_rotationSpeed(rotationSpeed)
{
	m_acceleration = m_thrust / m_weight;
}


void PlayerMove::move(float deltaTime)
{
	m_speedVector += m_acceleration * deltaTime;
	m_speedVector = m_speedVector.normalize() * utils::getMin(m_speedVector.length(), 120.f);
}

void PlayerMove::slow(float deltaTime)
{
	m_speedVector = m_speedVector * m_frictions;
}


void PlayerMove::rotateRight(float deltaTime)
{
	float rotationSpeed = m_rotationSpeed * deltaTime;
	m_acceleration = m_acceleration.getRotated(rotationSpeed);
	m_entity.rotate(rotationSpeed);
}

void PlayerMove::rotateLeft(float deltaTime)
{
	float rotationSpeed = m_rotationSpeed * deltaTime;
	m_acceleration = m_acceleration.getRotated(-rotationSpeed);
	m_entity.rotate(-rotationSpeed);
}

void PlayerMove::update(float deltaTime)
{
//drift

	float angle = getAngle(m_speedVector, m_acceleration) * m_drift;
	//get the direction of rotation
	angle *= utils::sign(dotProduct(m_speedVector, m_entity.m_ref.unitX));
	m_speedVector = m_speedVector.getRotated(angle);

	m_entity.pos() += m_speedVector * deltaTime;

	screenTeleport();
}