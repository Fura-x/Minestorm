#pragma once

#include "entity.h"

class Game;
class ConvexCollider;
class PlayerMove;
class Timer;
class SpawnComponent;
class ParticleEmitter;
class SoundEmitter;
struct Input;

class Player : public Entity
{
private:
	//Inventory&		inventory;
	PlayerMove*			m_move = nullptr;
	ConvexCollider*		m_collider = nullptr;

	Timer*				m_fireRate = nullptr;
	Timer*				m_invicibility = nullptr;
	SpawnComponent*		m_respawn = nullptr;
	SpawnComponent*		m_teleportTimer = nullptr;

	ParticleEmitter*	m_fireParticle = nullptr;
	ParticleEmitter*	m_smokeParticle = nullptr;
	ParticleEmitter*	m_TeleportParticle = nullptr;

	SoundEmitter*		m_shootSound = nullptr;
	SoundEmitter*		m_hurtSound = nullptr;
	SoundEmitter*		m_teleportSound = nullptr;

	const int	m_number = 1;
	int			m_life = 3;

public:
	int		m_score = 0;
	bool	m_fire = false;

	Player() = default;
	Player(int number, float2 pos, Texture2D* texture, Sound* Sounds);
	~Player() = default;

	int getNumber() const { return m_number; }
	int getLife() const { return m_life; }

	void teleport();
	void collisionHappens(Collider& other) override;
	void getHurt();
	void returnToLife();
	void returnFromTeleport();

	void applyInputPlayerOne(Input&, float deltaTime);
	void applyInputPlayerTwo(Input&, float deltaTime);

	void draw() override;
};

void	drawPlayerOneInfo(int x, int y, int life, int score, Texture2D texture);
void	drawPlayerTwoInfo(int x, int y, int life, int score, Texture2D texture);

void	drawPlayerTexture(Texture2D texture, Rectangle src, float rotation, Color color);