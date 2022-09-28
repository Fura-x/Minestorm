
#include "raylib.h"

#include "defines.h"
#include "mine.h"
#include "sound_emitter.h"

Mine::Mine(float2 _pos, Texture2D* texture, Sound explosion, Mines type, int rank)
	:Entity(_pos, texture), m_type(type), m_rank(rank)
{
//sound
	addComponent(&m_explosionSound, true);
	m_explosionSound->setSound(explosion);

	addComponent(&m_explosionFireParticle, false);
	m_explosionFireParticle->setColor(VIOLET, BLACK);
	m_explosionFireParticle->setParticleEmissionParams(16, 180, 0, 2);
	m_explosionFireParticle->setVelocity((float)(rand() % 100) + 50.f);
	m_explosionFireParticle->setSize((float)(rand() % 10) + 10.f, 2.f);
	m_explosionFireParticle->setLifeTime(0.5f);

	addComponent(&m_explosionLightningParticle, false);
	m_explosionLightningParticle->setColor(YELLOW, WHITE);
	m_explosionLightningParticle->setParticleEmissionParams(8, 180, 0, 1);
	m_explosionLightningParticle->setVelocity((float)(rand() % 100) + 100.f);
	m_explosionLightningParticle->setSize(5.f, 5.f);
	m_explosionLightningParticle->setLifeTime(1.f);

	addComponent(&m_explosionTimer, false);
	m_explosionTimer->setParameter(1.f, std::bind(&Mine::entityDead, this));
}

void Mine::setSize()
{
	//define size according to rank (count of division)
	m_size = m_rank == 1 ? 0.75f : 0.5f;
	if (m_rank >= 3) m_size = 0.25f;
}

void Mine::mineExplose()
{
	m_explosionFireParticle->m_isActivate = m_explosionFireParticle->m_isProductive = true;
	m_explosionLightningParticle->m_isActivate = m_explosionLightningParticle->m_isProductive = true;
	m_explosionTimer->m_isActivate = true;

	if (m_rank < 3)
	{
		for (int i = 0; i < m_childPerDivision; ++i)
			childPos.push_back({ pos().x + (float)(rand() % 50), pos().y - (float)(rand() % 50) });
	}

	if (m_explosionSound) m_explosionSound->play();
}

void Mine::entityDead() 
{
	m_isAlive = false;

	if (m_rank >= 3)
		return;

	m_shouldDivide = true;

}

void Mine::draw()
{
	m_explosionFireParticle->draw();
	m_explosionLightningParticle->draw();

	for (auto& pos : childPos)
	{
		DrawTexturePro(*m_texture, { ATLAS_SPRITE_SIZE, 0, ATLAS_SPRITE_SIZE, ATLAS_SPRITE_SIZE },
			{ pos.x, pos.y, ATLAS_SPRITE_SIZE * SPRITE_SCALE, ATLAS_SPRITE_SIZE * SPRITE_SCALE },
			{ ATLAS_SPRITE_SIZE * SPRITE_SCALE / 2, ATLAS_SPRITE_SIZE * SPRITE_SCALE / 2 }, 0.f, WHITE);
	}
}