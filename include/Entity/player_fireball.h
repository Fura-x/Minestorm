#pragma once

#include "entity.h"
#include "circle_collider.h"

class BulletMove;
class ParticleEmitter;

class PlayerFireball : public Entity
{
private:
	BulletMove*		m_move = nullptr;
	CircleCollider* m_collider = nullptr;
	ParticleEmitter* m_particleEmitter = nullptr;
	Color			m_color = RED;

	int* m_playerScore = nullptr;

public:
	PlayerFireball(float2 pos, float2 dir, Texture2D* texture, int* playerScore, Color ballColor = RED);
	~PlayerFireball() = default;
	
	void setPlayerScore(int* pSc) { m_playerScore = pSc; }
	void collisionHappens(Collider& other) override;

	void draw() override;
};