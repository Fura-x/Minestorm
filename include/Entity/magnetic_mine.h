#pragma once

#include "mine.h"

class ConcaveCollider;
class FollowMove;

class MagneticMine : public Mine
{
private:
	FollowMove*			m_move = nullptr;
	ConcaveCollider*	m_collider = nullptr;

public:
	MagneticMine(float2 pos, Texture2D* texture, Sound explosion, int rank, float2* playerPos);
	~MagneticMine() = default;

	void collisionHappens(Collider& other) override;
	void mineExplose() override;

	void draw() override;
};