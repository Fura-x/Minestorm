
#include "raylib.h"

#include "enums.h"
#include "defines.h"

#include "game.h"
#include "player_fireball.h"
#include "bullet_move.h"
#include "particle_emitter.h"

PlayerFireball::PlayerFireball(float2 _pos, float2 dir, Texture2D* texture, int* playerScore, Color ballColor)
	:Entity(_pos, texture), m_color(ballColor), m_playerScore(playerScore)
{
//particle component
	//particle emitter (when move)
	addComponent(&m_particleEmitter, true);

	m_particleEmitter->setColor(YELLOW, WHITE);
	m_particleEmitter->setParticleEmissionParams(3, 100, 90, 5);
	m_particleEmitter->setLifeTime(0.4f);
	m_particleEmitter->setVelocity(100.f);
	m_particleEmitter->m_isProductive = true;

//mmove component
	addComponent(&m_move, true);
	m_move->setSpeed(300.f);
	m_move->setDirection(dir);
	m_move->setMaxDistance(GAME_HEIGHT / 2.f);

//collider component
	circle cir = { pos(), 8.f };
	addComponent(&m_collider, true);
	m_collider->m_circle = cir;
	m_collider->setTagAndMask (u64 ColliderTag::PLAYER_FIREBALL, u64 ColliderTag::MINE + u64 ColliderTag::MINE_FIREBALL );
	
	m_colliderToAdd = m_collider;
}

void PlayerFireball::collisionHappens(Collider& other)
{
	if (other.getTag() == u64 ColliderTag::MINE || other.getTag() == u64 ColliderTag::MINE_FIREBALL)
	{
		*m_playerScore += other.m_entity.getScoring();
		entityDead();
	}
}


void	PlayerFireball::draw()
{
	m_particleEmitter->draw();

	DrawTexturePro(*m_texture, { ATLAS_SPRITE_SIZE * 3, 0, ATLAS_SPRITE_SIZE, ATLAS_SPRITE_SIZE },
								{ m_collider->m_circle.center.x,m_collider->m_circle.center.y, ATLAS_SPRITE_SIZE * SPRITE_SCALE, ATLAS_SPRITE_SIZE * SPRITE_SCALE },
								{ (int)(130.f * SPRITE_SCALE), (int)(130.f * SPRITE_SCALE) },
								rotation() + 180.f, m_color);

	//AABB and referential
	if (m_showAABB)
	{
		circle cir = m_collider->m_circle;
		DrawRectangleLinesEx({ cir.center.x - cir.radius, cir.center.y - cir.radius, cir.radius * 2.f, cir.radius * 2.f }, 1, DARKBLUE);
		drawReferential();
	}
}