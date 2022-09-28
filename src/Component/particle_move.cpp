
#include "entity.h"

#include "particle_move.h"

ParticleMove::ParticleMove(Entity& en)
	:Move(en)
{
	//prepare random dir
	m_maxDistance = 0.f;
}

void ParticleMove::resetParameter(float2 dir)
{
	m_dir = dir;
	m_alpha = 255;
	m_distance = 0;
	m_lifeTime = m_maxLifeTime;
}


void ParticleMove::setParameter(float2 dir, float lifeTime, float speed)
{
	m_dir = dir;
	m_speed = speed;
	m_lifeTime = lifeTime;
	m_maxLifeTime = lifeTime;
}

void ParticleMove::setParameter(float lifeTime, float speed)
{
	m_speed = speed;
	m_lifeTime = lifeTime;
	m_maxLifeTime = lifeTime;
}


void ParticleMove::update(float deltaTime)
{
	float move = m_speed * deltaTime;
	m_entity.pos() += m_dir * move;

	m_distance += move;
	m_lifeTime -= deltaTime;

	m_alphaReduce = m_alpha;
	m_alpha = m_lifeTime * 255.f / m_maxLifeTime;
	m_alphaReduce -= m_alpha;

	if (m_alpha <= 0.f || (m_maxDistance != 0 && m_distance > m_maxDistance))
		m_entity.entityDead();
}