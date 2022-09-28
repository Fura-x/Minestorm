#pragma once

#include <vector>

#include "enums.h"

#include "entity.h"
#include "spawn_point.h"

class Timer;

struct SpawnPointIntermediate
{
	float2 pos;
	Mines type = Mines::FLOATING;
	float time = 3.f;
};

struct SpawnTimer
{
	float m_timer = 3.f, m_maxTimer = 3.f;
	bool m_run = false;

	inline void start()
	{
		m_timer = m_maxTimer;
		m_run = true;
	}

	inline void update(float deltaTime)
	{
		if (m_run)
			m_timer -= deltaTime;

		m_run = m_timer <= 0 ? false : true;
	}
};

class SpawnEmitter
{
private:
	SpawnTimer m_spreadTimer = { 6.f, 6.f, false };

	int m_wave = 0;
	int m_mineCount = 0;
	bool m_waveBegin = false;

public:
	std::vector<SpawnPointIntermediate> m_spawnPoints;

	SpawnEmitter();
	~SpawnEmitter() = default;

	void mineCreated() { m_mineCount += 4; } //one giant mine = 4 mini mine
	void mineDestroyed() 
	{
		m_waveBegin = false;
		--m_mineCount;
	} //only mini mine are noticed

	int getWave() const { return m_wave; }
	Mines getRandomType();
	float2 getRandomPos();

	void update(float deltaTime);
	void draw();
};