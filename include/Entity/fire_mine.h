#pragma once

#include "mine.h"
#include "concave_collider.h"

class BulletMove;

class FireMine : public Mine
{
private:
	BulletMove* m_move = nullptr;
	ConcaveCollider* m_collider = nullptr;

public:
	FireMine(float2 pos, Texture2D* texture, Sound explosion, int rank = 1);
	~FireMine() = default;

	void collisionHappens(Collider& other) override;
	void mineExplose() override;

	void	draw() override;
};