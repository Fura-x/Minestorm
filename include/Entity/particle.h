#pragma once

#include "entity.h"

class ParticleMove;

struct ParticleInfo
{
	Color beginColor = RED, endColor = BLUE;
	float curColor[3] = { 0.f, 0.f, 0.f };
	float beginSize = 5.f, endSize = 5.f;
	float curSize = 5.f;
	float lifeTime = 1.f, speed = 50.f;
};

class Particle : public Entity
{
private:
	ParticleMove* m_move = nullptr;
	ParticleInfo m_info;

public:
	Particle() = default;
	Particle(float2 pos, float2 dir, ParticleInfo info);
	~Particle() = default;

	void setDirection(const float2 dir);
	void setInfo(const ParticleInfo info);
	void resetMove(const float2 dir);

	void draw() override;
};