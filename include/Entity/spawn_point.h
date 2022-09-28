#pragma once

#include <functional>

#include "enums.h"
#include "entity.h"

class SpawnComponent;
class Mine;
class SoundEmitter;

class SpawnPoint : public Entity
{
private:
	SpawnComponent* m_spawnTimer = nullptr;
	const Mines		m_type = Mines::FLOATING;

	SoundEmitter* m_popSound = nullptr;

public:
	bool m_shouldSpawnMine = false;

	SpawnPoint(float2 pos, Texture2D*, Sound pop, float timer, const Mines type);
	~SpawnPoint() = default;

	const Mines getType() const { return m_type; }
	void spawnMine();

	void draw() override;
};