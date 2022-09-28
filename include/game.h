#pragma once

#include "raylib.h"

#include "primitive.h"

#include <vector>
#include <array>
#include <memory>

#include "defines.h"
#include "enums.h"

#include "entity.h"
#include "player.h"
#include "collision_engine.h"
#include "input_engine.h"
#include "music_engine.h"
#include "ressources.h"
#include "spawn_emitter.h"

class TextEntity;
class PlayerFireball;
class MineFireball;
class SpawnPoint;
class MineLayer;
class Mine;
class FloatingMine;
class FireMine;
class MagneticMine;
class FireMagneticMine;

class Game
{
private:

    std::vector<std::unique_ptr<Entity>> m_entities;

    float       m_deltaTime;
    int         m_score = 0;
    bool        shouldEnd = false;

public:
    Game();
    ~Game();

    std::unique_ptr<CollisionEngine>    collisionEngine = nullptr;
    std::unique_ptr<InputEngine>        inputEngine = nullptr;
    std::unique_ptr<MusicEngine>        musicEngine = nullptr;
    std::unique_ptr<Ressources>         ressources = nullptr;
    std::unique_ptr<SpawnEmitter>       spawnEmitter = nullptr;

    std::vector<Player*>        m_players;
    std::vector<SpawnPoint*>    m_spawnPoints;
    std::vector<MineLayer*>     m_mineLayer;

    State       m_state = State::MENU;

    float       getDeltaTime() const {return m_deltaTime;}
    Texture2D&  getTexture(const Texture_E t) const { return ressources->m_textures[(int)t]; }
    Sound&      getSound(const Sound_E s) const { return ressources->m_sounds[(int)s]; }

    void    addEntity(std::unique_ptr<Entity>);
    Entity* getBackEntity();
    void    addCollider(Entity&);

    TextEntity* createTextEntityMoving(float2 pos, std::string text, int size, Color color, float speed, float2 dir, float maxDistance);

    PlayerFireball* createPlayerFireball(float2 pos, float2 dir, Texture2D* texture, int* playerScore, Color ballColor = RED);
    MineFireball* createMineFireball(float2 pos, Texture2D* texture);
    SpawnPoint* createSpawnPoint(Mines type, float2 pos, Texture2D* texture, float time);
    MineLayer* createMineLayer();


    Mine* createMine(Mines type, float2 pos, Texture2D*, int rank = 1);
    FloatingMine* createFloatingMine(float2 pos, Texture2D* texture, int rank = 1);
    FireMine* createFireMine(float2 pos, Texture2D* texture, int rank = 1);
    MagneticMine* createMagneticMine(float2 pos, Texture2D* texture, int rank = 1);
    FireMagneticMine* createFireMagneticMine(float2 pos, Texture2D* texture, int rank = 1);

    //exemple// void            createMainMenu();

    void    update();
    void    updateMenu();
    void    updateInGame();
    void    updatePause();
    void    updateGameOver();

    void    updateCreateEntity();
    void    updateDeadEntity();
            
    void    draw();
    void    drawMenu();
    void    drawInGame();
    void    drawPause();
    void    drawGameOver();

    void    goToMenu();
    void    goToSolo();
    void    goToDuo();

    void    clear();
    void    clearEntities();
    void    endGame() { shouldEnd = true; }
};
