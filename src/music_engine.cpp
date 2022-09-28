
#include "raylib.h"

#include "utils.h"

#include "ressources.h"
#include "music_engine.h"

MusicEngine::MusicEngine(Ressources& ressources)
	:m_ressource(ressources)
{
	for (auto& sound : m_ressource.m_sounds)
		SetSoundVolume(sound, 1.f * m_volume);
}

void MusicEngine::applyInput(bool volumeDown)
{
	m_volume += volumeDown ? -.05f : .05f;
	m_volume = utils::clamp(m_volume, 0.f, 1.f);

	for (auto& sound : m_ressource.m_sounds)
		SetSoundVolume(sound, 1.f * m_volume);
}


void MusicEngine::update(State gameState)
{
	if (currentGameState != gameState)
	{
		if (gameState == State::PAUSE)
			SetSoundVolume(m_ressource.m_sounds[(int)currentSound], 0.3f * m_volume);
		else if ( (gameState == State::INGAME && currentGameState == State::PAUSE))
			SetSoundVolume(m_ressource.m_sounds[(int)currentSound], 1.f * m_volume);
		else
			changeMusic(gameState);

		currentGameState = gameState;

		if (gameState != State::GAME_OVER)
			PlaySound(m_ressource.m_sounds[(int)Sound_E::GAME_STATE]);
		else
			PlaySound(m_ressource.m_sounds[(int)Sound_E::GAME_OVER]);
	}

	if (!IsSoundPlaying(m_ressource.m_sounds[(int)currentSound]))
		PlaySound(m_ressource.m_sounds[(int)currentSound]);
}

void MusicEngine::changeMusic(State gameState)
{
	StopSound(m_ressource.m_sounds[(int)currentSound]);
	if (gameState == State::MENU)
	{
		PlaySound(m_ressource.m_sounds[(int)Sound_E::MENU]);
		currentSound = Sound_E::MENU;
	}
	else if (gameState == State::INGAME)
	{
		PlaySound(m_ressource.m_sounds[(int)Sound_E::INGAME]);
		currentSound = Sound_E::INGAME;
	}
	else if (gameState == State::GAME_OVER)
	{
		PlaySound(m_ressource.m_sounds[(int)Sound_E::GAME_OVER_THEME]);
		currentSound = Sound_E::GAME_OVER_THEME;
	}
}