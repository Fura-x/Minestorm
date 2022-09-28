
#include "math_toolbox.h"
#include "entity.h"

#include "particle_emitter.h"
#include "timer.h"

static Color getRandomColor()
{
	int r = rand() % 20;
	Color colors[24] = { LIGHTGRAY, GRAY, DARKGRAY,YELLOW,GOLD,ORANGE,PINK,RED,MAROON,GREEN,LIME,DARKGREEN,SKYBLUE,BLUE,DARKBLUE,PURPLE,VIOLET,DARKPURPLE, BEIGE, BROWN, DARKBROWN, WHITE, BLACK };
	return colors[r];
}

ParticleEmitter::ParticleEmitter(Entity& en, int scope, unsigned int particlePerSecond)
	:Component(en), m_scope(scope), m_particleCreateCount(10)
{
	m_createTimer.push_back(Timer(m_entity, 1.f / (float)particlePerSecond));
	m_createTimer[0].m_isActivate = false;
}

void ParticleEmitter::createParticles()
{
	float2 pos = m_entity.pos() + m_translate.getRotated((float)(m_entityOrientaion + m_emitterOrientation));
	for (int i = 0; i < m_particleCreateCount; ++i)
	{
		//get random values
		float angle = (float)(rand() % (m_scope * 2) + m_entityOrientaion - m_scope);
		float2 dir = float2(1.f, 0.f).getRotated(angle);
		
		if (m_randomColor)
		{
			m_info.beginColor = getRandomColor();
			m_info.curColor[0] = (float)m_info.beginColor.r;
			m_info.curColor[1] = (float)m_info.beginColor.g;
			m_info.curColor[2] = (float)m_info.beginColor.b;
			m_info.endColor = getRandomColor();
		}

		//if a particle is dead, we replace it by another
		bool replaced = false;
		for (auto& particle : m_particles)
		{
			if (!particle->m_isAlive)
			{
				particle->pos() = pos;
				particle->m_isAlive = true;
				particle->resetMove(dir);
				particle->setInfo(m_info); //reset info
				replaced = true;
				break;
			}
		}
		if (!replaced)
			m_particles.push_back(std::make_unique<Particle>(pos, dir, m_info));
	}
	m_createTimer[0].startTimer();
}

void ParticleEmitter::setColor(Color begin, Color end)
{
	m_info.beginColor = begin;
	m_info.curColor[0] = (float)begin.r;
	m_info.curColor[1] = (float)begin.g;
	m_info.curColor[2] = (float)begin.b;
	m_info.endColor = end;
}

void ParticleEmitter::setLifeTime(float lifeSeconde)
{
	m_info.lifeTime = lifeSeconde;
}

void ParticleEmitter::setVelocity(float velocity)
{
	m_info.speed = velocity;
}

void ParticleEmitter::setSize(float beginSize, float endSize)
{
	m_info.beginSize = beginSize;
	m_info.endSize = endSize;
	m_info.curSize = beginSize;
}


void ParticleEmitter::setParticleEmissionParams(const int count, const int scope, const int orientation, const unsigned int particlePerSecond)
{
	m_particleCreateCount = count;
	m_scope = scope;
	m_emitterOrientation = orientation;

	if (m_createTimer.size() > 0) m_createTimer[0].setMaxTimer(1.f / (float)particlePerSecond);
	else m_createTimer.push_back(Timer(m_entity, 1.f / (float)particlePerSecond));

	m_createTimer[0].m_isActivate = false;
}


void ParticleEmitter::update(float deltaTime)
{
	int angle = (int)m_entity.rotation() - m_entityOrientaion + m_emitterOrientation;
	m_entityOrientaion += angle;

	if (m_createTimer[0].m_isActivate)
		m_createTimer[0].update(deltaTime);

	if (m_isProductive && !m_createTimer[0].m_isActivate)
		createParticles();

	for (auto& particle : m_particles)
	{
		if (particle->m_isAlive)
			particle->update(deltaTime);
	}
}

void ParticleEmitter::draw()
{
	for (auto& particle : m_particles)
	{
		if (particle->m_isAlive)
			particle->draw();
	}
}