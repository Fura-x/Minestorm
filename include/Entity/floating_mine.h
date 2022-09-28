#pragma once

#include "mine.h"
#include "concave_collider.h"

class BulletMove;

class FloatingMine : public Mine
{
private:
	BulletMove*			m_move = nullptr;
	ConcaveCollider*	m_collider = nullptr;

public:
	FloatingMine(float2 pos, Texture2D* texture, Sound explosion, int rank = 1);
	~FloatingMine() = default;

	void collisionHappens(Collider& other) override;
	void mineExplose() override;

	void	draw() override;
};