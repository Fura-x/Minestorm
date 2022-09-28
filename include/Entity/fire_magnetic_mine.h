#pragma once

#include "mine.h"

class ConcaveCollider;
class FollowMove;

class FireMagneticMine : public Mine
{
private:
	FollowMove* m_move = nullptr;
	ConcaveCollider* m_collider = nullptr;

public:
	FireMagneticMine(float2 pos, Texture2D* texture, Sound explosion, int rank, float2* playerPos);
	~FireMagneticMine() = default;

	void collisionHappens(Collider& other) override;
	void mineExplose() override;

	void draw() override;
};