#pragma once

#include "enums.h"

class Ressources;

class MusicEngine
{
private:
	Ressources& m_ressource;
	State currentGameState = State::GAME_OVER;
	Sound_E currentSound = Sound_E::MENU;

	float m_volume = 0.8f;

public:
	MusicEngine(Ressources& ressource);
	~MusicEngine() = default;

	float getVolume() const { return m_volume; }

	void applyInput(bool volumeDown);

	void update(State gameState);
	void changeMusic(State gameState);
};