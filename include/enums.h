#pragma once

enum class ColliderTag
{
	NONE = 0,
	PLAYER = 1,
	MINE = 2,
	PLAYER_FIREBALL = 4,
	MINE_FIREBALL = 8,

	//TODO change count for each tag added
	COUNT = 5
};

enum class State
{
	MENU,
	INGAME,
	PAUSE,
	GAME_OVER,

	COUNT
};

enum class Mines
{
	FLOATING,
	FIRE,
	MAGNETIC,
	FIRE_MAGNETIC,

	COUNT
};

enum class Font_E
{
	COUNT
};

enum class Texture_E
{
	ATLAS_MINESTORM,
	ATLAS_MINESTORM_SPECIAL,

	BACKGROUND,
	FOREGROUND,

	VOLUME_LOGO,

	COUNT
};

enum class Music_E
{
	MENU,
	INGAME,

	COUNT,
	NONE
};

enum class Sound_E
{
//Music
	MENU,
	INGAME,
	GAME_OVER_THEME,

//Sound
	GAME_STATE,
	PLAYER_SHOT,
	PLAYER_HURT,
	NEW_WAVE,
	MINE_APPEARS,
	MINE_EXPLOSES,
	MINELAYER_EXPLOSES,
	TELEPORT,
	GAME_OVER,

	COUNT
};