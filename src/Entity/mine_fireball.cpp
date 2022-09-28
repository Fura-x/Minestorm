
#include "raylib.h"

#include "enums.h"
#include "defines.h"

#include "game.h"
#include "mine_fireball.h"
#include "circle_collider.h"
#include "bullet_move.h"

MineFireball::MineFireball(float2 _pos, float2 dir, Texture2D* texture)
	:Entity(_pos, texture)
{
	//move component
	addComponent(&m_move, true);
	m_move->setDirection(dir);
	m_move->setMaxDistance(GAME_HEIGHT);
	m_move->setSpeed(75.f);

//scoring
	m_scoring = 110;

	//collider component
	circle cir = { pos(), 8.f };
	addComponent(&m_collider, true);
	m_collider->m_circle = cir;
	m_collider->setTagAndMask( u64 ColliderTag::MINE_FIREBALL, u64 ColliderTag::PLAYER + u64 ColliderTag::PLAYER_FIREBALL);

	m_colliderToAdd = m_collider;
}

void MineFireball::collisionHappens(Collider& other)
{
	if (other.getTag() == u64 ColliderTag::PLAYER_FIREBALL)
		m_isAlive = false;
}

void MineFireball::entityDead()
{
	m_isAlive = false;
}


void	MineFireball::draw()
{
	DrawTexturePro(*m_texture, { ATLAS_SPRITE_SIZE * 3, 0, ATLAS_SPRITE_SIZE, ATLAS_SPRITE_SIZE },
		{ m_collider->m_circle.center.x,m_collider->m_circle.center.y, ATLAS_SPRITE_SIZE * 0.5f, ATLAS_SPRITE_SIZE * 0.5f },
		{ (int)(ATLAS_SPRITE_SIZE / 2 * 0.5f), (int)(ATLAS_SPRITE_SIZE / 2 * 0.5f) }, rotation() + 180.f, WHITE);

	//AABB and referential
	if (m_showAABB)
	{
		circle cir = m_collider->m_circle;
		DrawRectangleLinesEx({ cir.center.x - cir.radius, cir.center.y - cir.radius, cir.radius * 2.f, cir.radius * 2.f }, 1, DARKBLUE);
		drawReferential();
	}
}