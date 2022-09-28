#pragma once

#include "move.h"

class Entity;

class PlayerMove : public Move
{
private:
	float m_curRotation = 0.f;
	float m_rotationSpeed = 90.f;
	float m_drift = 0.2f;
	float m_frictions = 0.996f;
	float m_weight = 1.f;
	float2 m_thrust = { 0.f, 100.f };
	float2 m_acceleration = { 0.f, 0.f };
	float2 m_speedVector = { 0.f, 0.f };

public:
	PlayerMove(Entity& en);
	PlayerMove(Entity& en, float speed, float rotationSpeed);
	~PlayerMove() = default;

	float getRotation() { return m_curRotation; }
	float2 getSpeedVector() { return m_speedVector; }

	void move(float deltaTime);
	void slow(float deltaTime);
	void rotateRight(float deltaTime);
	void rotateLeft(float deltaTime);

	//set the speed to 0
	void reset() { m_speedVector = { 0.f, 0.f }; }

	void	update(float deltaTime) override;
};