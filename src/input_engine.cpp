
#include <iostream>

#include "raylib.h"

#include "game.h"
#include "player.h"
#include "enums.h"
#include "input_engine.h"
#include "music_engine.h"
#include "utils.h"

void    InputEngine::update(Game* game)
{
//update input
	//Player 1 move
	input.move1 = IsKeyDown(KEY_R);
	input.shoot1 = IsKeyDown(KEY_F);
	input.right1 = IsKeyDown(KEY_G);
	input.left1 = IsKeyDown(KEY_D);
	input.teleport1 = IsKeyPressed(KEY_E) || IsKeyPressed(KEY_T);

	//Player 2 move
	input.move2 = IsKeyDown(KEY_I);
	input.shoot2 = IsKeyDown(KEY_K);
	input.right2 = IsKeyDown(KEY_L);
	input.left2 = IsKeyDown(KEY_J);
	input.teleport2 = IsKeyPressed(KEY_U) || IsKeyPressed(KEY_O);

	//Other comand
	input.pause = IsKeyPressed(KEY_SPACE);
	input.exit = IsKeyPressed(KEY_B);
	if (IsKeyPressed(KEY_C))
		input.changeBoxShow = input.changeBoxShow ? false : true;

	input.volumeDown = IsKeyPressed(KEY_LEFT);
	input.volumeUp = IsKeyPressed(KEY_RIGHT);

//apply input
	apply(game);
}

void    InputEngine::apply(Game* game)
{
	if (game->m_state == State::INGAME)
	{
		Player* player1 = game->m_players.size() > 0 ? game->m_players[0] : nullptr;
		Player* player2 = game->m_players.size() > 1 ? game->m_players[1] : nullptr;

		if (player1)
			player1->applyInputPlayerOne(input, game->getDeltaTime());
		if (player2)
			player2->applyInputPlayerTwo(input, game->getDeltaTime());
	}

	if ((game->m_state == State::MENU || game->m_state == State::PAUSE) && (input.volumeDown || input.volumeUp))
		game->musicEngine->applyInput(input.volumeDown);

//change game state
	if (input.shoot1 && game->m_state == State::MENU)
		game->goToSolo();
	else if (input.shoot2 && game->m_state == State::MENU)
		game->goToDuo();

	if (input.pause && game->m_state == State::INGAME)
		game->m_state = State::PAUSE;
	else if (input.pause && game->m_state == State::PAUSE)
		game->m_state = State::INGAME;

	if (input.exit && (game->m_state == State::PAUSE || game->m_state == State::GAME_OVER))
		game->goToMenu();

	//changeBoxShow is apply in game update during the entities loop
}

