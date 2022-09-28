#pragma once

#include <vector>
#include <memory>

#include "component.h"
#include "particle.h"

class Entity;
class Timer;

class ParticleEmitter : public Component
{
private:
	std::vector<std::unique_ptr<Particle>> m_particles;
	ParticleInfo m_info;

	std::vector<Timer> m_createTimer;
	int m_particleCreateCount = 1;
	int m_entityOrientaion = 0, m_emitterOrientation = 0, m_scope = 30;

	bool m_randomColor = false;

public:
	float2 m_translate;
	bool m_isProductive = false;

	ParticleEmitter(Entity& en, int m_scope = 360.f, unsigned int particlePerSecond = 2);
	~ParticleEmitter() = default;

	void setColor(Color begin, Color end);
	void setRandomColor() { m_randomColor = true; }
	void setLifeTime(float lifeSeconde);
	void setVelocity(float velocity);
	void setSize(float beginSize, float endSize);
	void setParticleEmissionParams(const int count, const int scope, const int orientation, const unsigned int particlePerSecond);
	void setParticleCreateCount(const int count) { m_particleCreateCount = count; }

	void update(float deltaTime) override;
	void createParticles();
	void draw();
};