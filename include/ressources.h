#pragma once

#include <vector>
#include "raylib.h"

class Game;

class Ressources
{
private:
    Game*   m_game;
    
public:
    Ressources(Game*);
    ~Ressources();

    std::vector<Font>       m_fonts;
    std::vector<Texture2D>  m_textures;
    std::vector<Music>      m_musics;
    std::vector<Sound>      m_sounds;
};
