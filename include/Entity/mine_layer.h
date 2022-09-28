#pragma once

#include "entity.h"

class ConcaveCollider;
class SpawnComponent;
class MineLayerMove;
class SoundEmitter;

class MineLayer : public Entity
{
private:
	ConcaveCollider* m_collider = nullptr;
	SpawnComponent* m_spawn = nullptr;
	MineLayerMove* m_move = nullptr;

	SoundEmitter* m_explosionSound = nullptr;

public:
	bool m_shouldCreateSpawn = false;
	bool m_definitelyCreate = false;

	MineLayer(float2 pos, Texture2D* texture, Sound explosion, float2 goal);
	~MineLayer() = default;

	void beginMove();
	void collisionHappens(Collider& other) override;
	void entityDead() override;

	void draw() override;
};