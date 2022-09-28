
#include <iostream>
#include <string>

#include "raylib.h"
#include "math_toolbox.h"

#include "defines.h"

#include "game.h"
#include "player.h"
#include "timer.h"
#include "spawn.h"
#include "player_move.h"
#include "convex_collider.h"
#include "particle_emitter.h"
#include "sound_emitter.h"

static std::string getStrScore(int iscore)
{
	std::string score;

	if (iscore < 10)
		score = "Score : 000" + std::to_string(iscore);
	else if (iscore < 100)
		score = "Score : 00" + std::to_string(iscore);
	else if (iscore < 1000)
		score = "Score : 0" + std::to_string(iscore);
	else
		score = "Score : " + std::to_string(iscore);

	return score;
}


Player::Player(int number, float2 _pos, Texture2D* texture, Sound* sounds)
	: Entity(_pos, texture), m_number(number)
{

//basic move
	addComponent(&m_move, true);

//firerate
	addComponent(&m_fireRate, true);
	m_fireRate->setMaxTimer(0.25f);

//timer invicibility
	addComponent(&m_invicibility, false);
	m_invicibility->setMaxTimer(2.f);

//respawn component
	addComponent(&m_respawn, false);
	std::function<void(void)> event = std::bind(&Player::returnToLife, this);
	m_respawn->setParameter(3.f, event);

//teleport timer && function
	addComponent(&m_teleportTimer, false);
	event = std::bind(&Player::returnFromTeleport, this);
	m_teleportTimer->setParameter(0.5f, event);

//particle emitter (when move)
	addComponent(&m_fireParticle, true);

	m_fireParticle->setColor(RED, YELLOW);
	if (m_number == 2)
		m_fireParticle->setColor(GREEN, WHITE);
	m_fireParticle->m_translate = { 0.f, 15.f };
	m_fireParticle->setParticleEmissionParams(2, 30, -90, 50);
	m_fireParticle->setLifeTime(1.f);

	addComponent(&m_smokeParticle, true);
	m_smokeParticle->setColor(BLACK, LIGHTGRAY);
	m_smokeParticle->m_translate = { 0.f, 15.f };
	m_smokeParticle->setParticleEmissionParams(1, 90, -90, 100);
	m_smokeParticle->setLifeTime(0.2f);
	m_smokeParticle->setSize(10.f, 10.f);
	m_smokeParticle->setVelocity(70.f);

	addComponent(&m_TeleportParticle, true);
	m_TeleportParticle->m_isProductive = false;
	m_TeleportParticle->setRandomColor();
	m_TeleportParticle->setParticleEmissionParams(8, 180, 0, 10);
	m_TeleportParticle->setLifeTime(1.f);
	m_TeleportParticle->setSize(5.f, 10.f);
	m_TeleportParticle->setVelocity(100.f);

//shoot sound effect
	addComponent(&m_shootSound, true);
	m_shootSound->setSound(sounds[0]);

//teleport sound effect
	addComponent(&m_teleportSound, true);
	m_teleportSound->setSound(sounds[1]);

//hurt sound effect
	addComponent(&m_hurtSound, true);
	m_hurtSound->setSound(sounds[2]);

//collider || allocate memory for points but free in convexCollider 's destructor
	float2* pts = new (float2[5]){
		{pos().x + 0.f, pos().y + 23.f}, {pos().x + (cosf(-M_PI / 6.f) * 13.f), pos().y + (sinf(-M_PI / 6.f) * 10.f)},
		{pos().x + (cosf(-M_PI / 3.f) * 23.f), pos().y + (sinf(-M_PI / 3.f) * 22.f) },
		{pos().x + (cosf(-2.f * M_PI / 3.f) * 20.f), pos().y + (sinf(-2.f * M_PI / 3.f) * 22.f)}, {pos().x + (cosf(-5.f * M_PI / 6.f) * 10.f), pos().y + (sinf(-5.f * M_PI / 6.f) * 10.f)}
	};

	addComponent(&m_collider, true);
	m_collider->setCollider({ pts, 5 });
    m_collider->setTagAndMask(u64 ColliderTag::PLAYER, u64 ColliderTag::MINE + u64 ColliderTag::MINE_FIREBALL);

	m_colliderToAdd = m_collider;
}

void Player::teleport()
{
	pos() = m_move->getRandomPos();
	m_teleportTimer->m_isActivate = true;
	m_teleportTimer->startTimer();
	m_TeleportParticle->m_isProductive = true;

	if (m_teleportSound) m_teleportSound->play();
}


void Player::collisionHappens(Collider& other)
{
	bool mineCollision = other.getTag() == u64 ColliderTag::MINE || other.getTag() == u64 ColliderTag::MINE_FIREBALL;

	if (m_teleportTimer->m_isActivate && mineCollision)
		teleport();
	else if (!m_invicibility->m_isActivate && mineCollision)
		getHurt();
}

void Player::getHurt()
{
	--m_life;
	//we assert life in the game::update

	m_respawn->m_isActivate = true;
	m_move->m_isActivate = false;
	m_move->reset();
	m_collider->m_isActivate = false;

	pos() = { 0, 0 };

	if (m_hurtSound) m_hurtSound->play();
}

void Player::returnToLife()
{
	m_respawn->m_isActivate = false;
	m_move->m_isActivate = true;
	m_collider->m_isActivate = true;
	pos() = { (float)SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT / 2.f };
	m_invicibility->startTimer();
}

void Player::returnFromTeleport()
{
	m_teleportTimer->m_isActivate = false;
	m_TeleportParticle->m_isProductive = false;
}


void Player::applyInputPlayerOne(Input& inputs, float deltaTime)
{
	if (!m_move->m_isActivate || m_respawn->m_isActivate)
		return;

	if (inputs.move1)
	{
		m_fireParticle->m_isProductive = true;
		m_smokeParticle->m_isProductive = true;
		m_move->move(deltaTime);
	}
	else
	{
		m_fireParticle->m_isProductive = false;
		m_smokeParticle->m_isProductive = false;
		m_move->slow(deltaTime);
	}

	if (inputs.right1)
		m_move->rotateRight(deltaTime);

	if (inputs.left1)
		m_move->rotateLeft(deltaTime);

	if (inputs.teleport1)
		teleport();

	if (inputs.shoot1 && !m_fireRate->m_isActivate)
	{
		m_fire = true;
		m_fireRate->startTimer();
		m_shootSound->play();
	}
}

void Player::applyInputPlayerTwo(Input& inputs, float deltaTime)
{
	if (!m_move->m_isActivate || m_respawn->m_isActivate)
		return;

	if (inputs.move2)
	{
		m_fireParticle->m_isProductive = true;
		m_smokeParticle->m_isProductive = true;
		m_move->move(deltaTime);
	}
	else
	{
		m_fireParticle->m_isProductive = false;
		m_smokeParticle->m_isProductive = false;
		m_move->slow(deltaTime);
	}

	if (inputs.right2)
		m_move->rotateRight(deltaTime);

	if (inputs.left2)
		m_move->rotateLeft(deltaTime);

	if (inputs.teleport2)
		teleport();

	if (inputs.shoot2 && !m_fireRate->m_isActivate)
	{
		m_fire = true;
		m_fireRate->startTimer();
		m_shootSound->play();
	}
}

void	Player::draw()
{
	if (m_fireParticle->m_isActivate)
		m_fireParticle->draw();
	if (m_smokeParticle->m_isActivate)
		m_smokeParticle->draw();
	if (m_TeleportParticle->m_isActivate)
		m_TeleportParticle->draw();

	float x = pos().x, y = pos().y;
	if (m_respawn->m_isActivate)
	{
		if (((int)GetTime() * 5) % 2 == 1)
			return;
		//draw the player at the center, while his position is at 0, 0
		x = (float)SCREEN_WIDTH / 2;
		y = (float)SCREEN_HEIGHT / 2;
	}


	Color color = m_number == 1 ? RED : GREEN;
	drawPlayerTexture(*m_texture, { x, y, ATLAS_SPRITE_SIZE * SPRITE_SCALE, ATLAS_SPRITE_SIZE * SPRITE_SCALE }, rotation() + 180.f, color);

	if (m_invicibility->m_isActivate)
		DrawCircle((int)pos().x, (int)pos().y, 30.f, { 255, 255, 0, 100 });

	//AABB and referential
	if (m_showAABB && !m_teleportTimer->m_isActivate)
	{
		box rect = m_collider->m_AABB;
		DrawRectangleLinesEx({ rect.center.x - rect.width_ha, rect.center.y - rect.height_ha, rect.width_ha * 2.f, rect.height_ha * 2.f }, 1, DARKBLUE);
		drawReferential();
		DrawLineEx(pos(), posLocalToGlobal(m_move->getSpeedVector(), m_ref), 2.f, YELLOW);

		//draw polygons
		auto& convex = m_collider->m_convex;
		for (int pt = 0; pt < convex.nPts - 1; ++pt)
		{
			DrawLineEx(convex.pts[pt], convex.pts[pt + 1], 2.f, GREEN);
		}
		DrawLineEx(convex.pts[convex.nPts - 1], convex.pts[0], 2.f, GREEN);
	}
}


void	drawPlayerOneInfo(int x, int y, int life, int score, Texture2D texture)
{
	DrawRectangleRounded({ (float)x, (float)y, 150.f, 50.f }, 0.2f, 4, RED);
	DrawText("PLAYER ONE", x + 20, y + 10, 15, BLACK);

	float merge = 140.f / life;
	for (int i = 0; i < life; ++i)
	{
		drawPlayerTexture(texture, { (float)x + 10.f + (merge / 2.f) + (merge*i), (float)y + 35.f, 40.f, 40.f }, 0.f, WHITE);
	}

	std::string scoreStr = getStrScore(score);
	DrawText(scoreStr.c_str(), x + 10, y + 60, 12, GREEN);

	DrawText("Rotate : 'D' and 'G'", x + 10, y + 75, 8, WHITE);
	DrawText("Move : 'R", x + 10, y + 85, 8, WHITE);
	DrawText("Shoot : 'F'", x + 10, y + 95, 8, WHITE);
	DrawText("Teleport : 'E' or 'T'", x + 10, y + 105, 8, WHITE);
}

void	drawPlayerTwoInfo(int x, int y, int life, int score, Texture2D texture)
{
	DrawRectangleRounded({ (float)x, (float)y, 150.f, 50.f }, 0.2f, 4, GREEN);
	DrawText("PLAYER TWO", x + 20, y + 10, 15, BLACK);

	float merge = 140.f / life;
	for (int i = 0; i < life; ++i)
	{
		drawPlayerTexture(texture, { (float)x + 10.f + (merge / 2.f) + (merge * i), (float)y + 35.f, 40.f, 40.f }, 0.f, WHITE);
	}

	std::string scoreStr = getStrScore(score);
	DrawText(scoreStr.c_str(), x + 10, y + 60, 12, GREEN);

	DrawText("Rotate : 'J' and 'L'", x + 10, y + 75, 8, WHITE);
	DrawText("Move : 'I'", x + 10, y + 85, 8, WHITE);
	DrawText("Shoot : 'K'", x + 10, y + 95, 8, WHITE);
	DrawText("Teleport : 'U' or 'O'", x + 10, y + 105, 8, WHITE);
}

void	drawPlayerTexture(Texture2D texture, Rectangle dst, float rotation, Color color)
{
	DrawTexturePro(texture, { 0, 0, ATLAS_SPRITE_SIZE, ATLAS_SPRITE_SIZE }, dst, { dst.width / 2, dst.height / 2 }, rotation, color);
}