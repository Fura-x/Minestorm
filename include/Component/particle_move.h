#pragma once

#include "move.h"

class ParticleMove : public Move
{
private:
	float m_alpha = 255.f;
	float m_alphaReduce = 1.f;
	float m_lifeTime = 1.f, m_maxLifeTime = 1.f;

public:
	ParticleMove(Entity& en);
	~ParticleMove() = default;

	float getAlpha() const { return m_alpha; }
	float getReduce() const { return m_alphaReduce * 0.0016f; }

	void resetParameter(float2 dir);
	void setParameter(float2 dir, float lifeTime, float speed);
	void setParameter(float lifeTime, float speed);
	void update(float deltaTime) override;
};