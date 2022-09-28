
#include "game.h"
#include "enums.h"

#include "ressources.h"

Ressources::Ressources(Game* game)
: m_game(game)
{
    m_fonts.resize((int)Font_E::COUNT);

    m_textures.resize((int)Texture_E::COUNT);

    m_textures[(int)Texture_E::ATLAS_MINESTORM] = LoadTexture("media/minestorm_sprite_atlas_mine_storm.png");
    m_textures[(int)Texture_E::ATLAS_MINESTORM_SPECIAL] = LoadTexture("media/minestorm_sprite_atlas_mine_storm-special.png");
    m_textures[(int)Texture_E::BACKGROUND] = LoadTexture("media/minestorm_background.png");
    m_textures[(int)Texture_E::FOREGROUND] = LoadTexture("media/minestorm_forground.png");
    m_textures[(int)Texture_E::VOLUME_LOGO] = LoadTexture("media/volume.png");

    //m_musics.resize((int)Music_E::COUNT);
    //
    //m_musics[(int)Music_E::MENU] = LoadMusicStream("media/sounds/MiniMelodies-Minestorm_OST-01_Minestorm_OST-Title_Theme.ogg");
    //m_musics[(int)Music_E::INGAME] = LoadMusicStream("meida/sounds/Mini Melodies - Minestorm OST - 01 Minestorm OST - Ingame.ogg");

    m_sounds.resize((int)Sound_E::COUNT);

    m_sounds[(int)Sound_E::MENU] = LoadSound("media/sounds/MiniMelodies-Minestorm_OST-01_Minestorm_OST-Title_Theme.ogg");
    m_sounds[(int)Sound_E::INGAME] = LoadSound("media/sounds/MiniMelodies-Minestorm_OST-01_Minestorm_OST-Ingame.ogg");
    m_sounds[(int)Sound_E::GAME_OVER_THEME] = LoadSound("media/sounds/MiniMelodies-Minestorm_OST-01_Minestorm_OST-Highscore.ogg");

    m_sounds[(int)Sound_E::GAME_STATE] = LoadSound("media/sounds/game_state.wav");
    m_sounds[(int)Sound_E::PLAYER_SHOT] = LoadSound("media/sounds/player_shot.wav");
    m_sounds[(int)Sound_E::PLAYER_HURT] = LoadSound("media/sounds/player_hurts.wav");
    m_sounds[(int)Sound_E::TELEPORT] = LoadSound("media/sounds/teleport(2).wav");
    m_sounds[(int)Sound_E::NEW_WAVE] = LoadSound("media/sounds/new_wave.wav");
    m_sounds[(int)Sound_E::MINE_APPEARS] = LoadSound("media/sounds/mine_appears.wav");
    m_sounds[(int)Sound_E::MINE_EXPLOSES] = LoadSound("media/sounds/mine_explose(2).wav");
    m_sounds[(int)Sound_E::MINELAYER_EXPLOSES] = LoadSound("media/sounds/minelayer_explose.wav");
    m_sounds[(int)Sound_E::GAME_OVER] = LoadSound("media/sounds/game_over.wav");

}

Ressources::~Ressources()
{
    for (auto& font : m_fonts)
        UnloadFont(font);

    for (auto& texture : m_textures)
        UnloadTexture(texture);

    for (auto& music : m_musics)
        UnloadMusicStream(music);

    for (auto& sound : m_sounds)
        UnloadSound(sound);
}