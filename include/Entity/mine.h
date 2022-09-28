#pragma once

#include <vector>

#include "enums.h"

#include "entity.h"
#include "particle_emitter.h"
#include "spawn.h"

class SoundEmitter;

class Mine : public Entity
{
protected:
	ParticleEmitter* m_explosionFireParticle = nullptr;
	ParticleEmitter* m_explosionLightningParticle = nullptr;
	SpawnComponent* m_explosionTimer = nullptr;

	SoundEmitter* m_explosionSound = nullptr;

	const Mines m_type = Mines::FLOATING;
	int m_rank = 1;
	int m_childPerDivision = 2;
	float m_size = 1.f;

public:
	bool m_shouldFire = false;
	std::vector<float2> childPos;
	bool m_shouldDivide = false;

	Mine(float2 _pos, Texture2D* texture, Sound explosion, Mines type, int rank = 1);
	~Mine() = default;

	const Mines getType() const { return m_type; }
	int			getRank() const { return m_rank; }

	void setSize();

	void			entityDead() override;
	virtual void mineExplose();

	void draw() override;
};