
#include "raylib.h"

#include "defines.h"

#include "spawn_emitter.h"
#include "sound_emitter.h"
#include "timer.h"

SpawnEmitter::SpawnEmitter()
{
}

Mines SpawnEmitter::getRandomType()
{
	int total = 20;
	int fireRate = m_wave >= 2 ? 5 : 0;
	int magneticRate = m_wave >= 5 ? 4 : 0;
	int magnfireRate = m_wave >= 7 ? 3 : 0;
	int floatRate = total - fireRate - magneticRate - magneticRate;

	int r = rand() % total;

	if (r < magnfireRate)
		return Mines::FIRE_MAGNETIC;
	else if (r >= magnfireRate && r < magnfireRate + magneticRate)
		return Mines::MAGNETIC;
	else if (r >= magnfireRate + magneticRate && r < magnfireRate + magneticRate + fireRate)
		return Mines::FIRE;
	else
		return Mines::FLOATING;
}

float2 SpawnEmitter::getRandomPos()
{
	return { (float)(rand() % (GAME_WIDTH - 60) + (SCREEN_WIDTH / 2 - GAME_WIDTH / 2) + 30), (float)(rand() % (GAME_HEIGHT - 30) + 30) };
}


void SpawnEmitter::update(float deltaTime)
{
	if (m_mineCount <= 0 && !m_waveBegin)
	{
		++m_wave;
		m_waveBegin = true; //become false when a mine is dead

		//the mineCount var is incremented in each "createMine" of game.cpp

		float t = 0.f;
		for (int i = 0; i < m_wave + 1; ++i)
		{
			t += i % 2 == 0 ? 1 : 0;
			m_spawnPoints.push_back({ getRandomPos(), getRandomType(), t * 5.f });
		}
	}
}

void SpawnEmitter::draw()
{}

