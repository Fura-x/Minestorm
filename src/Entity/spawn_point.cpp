
#include "raylib.h"

#include "defines.h"

#include "spawn_point.h"
#include "spawn.h"
#include "sound_emitter.h"

SpawnPoint::SpawnPoint(float2 _pos, Texture2D* texture, Sound pop, float time, const Mines type)
	:Entity(_pos, texture), m_type(type)//, m_event(spawnEvent)
{
//timer of spawn
	addComponent(&m_spawnTimer, true);
	std::function<void(void)> evt = std::bind(&SpawnPoint::spawnMine, this);
	m_spawnTimer->setParameter(time, evt);

//Mine pop sound
	addComponent(&m_popSound, true);
	m_popSound->setSound(pop);
}

void SpawnPoint::spawnMine()
{
	m_isAlive = false;
	m_shouldSpawnMine = true;

	if (m_popSound) m_popSound->play();
}


void	SpawnPoint::draw()
{
	DrawTexturePro(*m_texture, { ATLAS_SPRITE_SIZE, 0, ATLAS_SPRITE_SIZE, ATLAS_SPRITE_SIZE },
		{ pos().x, pos().y, ATLAS_SPRITE_SIZE * SPRITE_SCALE, ATLAS_SPRITE_SIZE * SPRITE_SCALE },
		{ ATLAS_SPRITE_SIZE * SPRITE_SCALE / 2, ATLAS_SPRITE_SIZE * SPRITE_SCALE / 2 }, 0.f, WHITE);
}