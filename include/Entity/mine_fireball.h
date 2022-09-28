#pragma once

#include "entity.h"

class BulletMove;
class CircleCollider;

class MineFireball : public Entity
{
private:
	BulletMove* m_move = nullptr;
	CircleCollider* m_collider = nullptr;

public:
	MineFireball(float2 pos, float2 dir, Texture2D* texture);
	~MineFireball() = default;

	void collisionHappens(Collider& other) override;
	void entityDead() override;

	void draw() override;
};