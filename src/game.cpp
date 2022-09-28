#include <iostream>
#include <string>
#include <functional>

#include "game.h"
#include "utils.h"

#include "text_entity.h"
#include "mine_fireball.h"
#include "player_fireball.h"
#include "spawn_point.h"
#include "mine_layer.h"
#include "floating_mine.h"
#include "fire_mine.h"
#include "magnetic_mine.h"
#include "fire_magnetic_mine.h"

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

Game::Game() 
: m_state(State::MENU)
{
    collisionEngine = std::make_unique<CollisionEngine>();

    ressources =  std::make_unique<Ressources>(this);

    musicEngine = std::make_unique<MusicEngine>(*ressources.get());

    inputEngine = std::make_unique<InputEngine>();
}


Game::~Game()
{
    clear();
}

void    Game::addEntity(std::unique_ptr<Entity> en)
{
    m_entities.push_back(std::move(en));
}

Entity*     Game::getBackEntity()
{
    return m_entities.back().get();
}

void    Game::addCollider(Entity& en)
{
    if (en.m_colliderToAdd)
    {
        collisionEngine->setCollider(en.m_colliderToAdd);
        en.m_colliderToAdd = nullptr;
    }
}

TextEntity* Game::createTextEntityMoving(float2 pos, std::string text, int size, Color color, float speed, float2 dir, float maxDistance)
{
    addEntity(std::make_unique<TextEntity>(pos, text, size, color));
    TextEntity* en = reinterpret_cast<TextEntity*>(getBackEntity());
    en->setBulletMove(speed, dir, maxDistance);
    return en;
}

PlayerFireball* Game::createPlayerFireball(float2 pos, float2 dir, Texture2D* texture, int* playerScore, Color playerColor)
{
    addEntity(std::make_unique<PlayerFireball>(pos, dir, texture, playerScore, playerColor));
    return reinterpret_cast<PlayerFireball*>(getBackEntity());
}

MineFireball* Game::createMineFireball(float2 pos, Texture2D* texture)
{
    float2 dir = (m_players[0]->pos() - pos).normalize();
    if (m_players.size() > 1) dir = rand() % 2 == 1 ? dir : (m_players[1]->pos() - pos).normalize();
    addEntity(std::make_unique<MineFireball>(pos, dir, texture));
    return reinterpret_cast<MineFireball*>(getBackEntity());
}

SpawnPoint* Game::createSpawnPoint(Mines type, float2 pos, Texture2D* texture, float time)
{
    addEntity(std::make_unique<SpawnPoint>(pos, texture, getSound(Sound_E::MINE_APPEARS), time, type));
    SpawnPoint* s =  reinterpret_cast<SpawnPoint*>(getBackEntity());
    m_spawnPoints.push_back(s);
    return s;
}

MineLayer* Game::createMineLayer()
{
    int side = rand() % 4;
    float2 pos, goal;
    //chose a random arrival
    switch (side)
    {
    case 0 :
        pos = { SCREEN_WIDTH, SCREEN_HEIGHT / 2 };
        goal = { SCREEN_WIDTH / 2 + GAME_WIDTH / 3, SCREEN_HEIGHT / 2 };
        break;
    case 1:
        pos = { 0, SCREEN_HEIGHT / 2 };
        goal = { SCREEN_WIDTH / 2 - GAME_WIDTH / 3, SCREEN_HEIGHT / 2 };
        break;
    case 2:
        pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT };
        goal = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + GAME_HEIGHT / 4};
        break;
    case 3:
        pos = { SCREEN_WIDTH / 2, 0 };
        goal = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - GAME_HEIGHT / 3 };
    default: break;
    }

    addEntity(std::make_unique<MineLayer>(pos, &getTexture(Texture_E::ATLAS_MINESTORM), getSound(Sound_E::MINELAYER_EXPLOSES), goal));
    MineLayer* m = reinterpret_cast<MineLayer*>(getBackEntity());
    m_mineLayer.push_back(m);
    return m;
}

Mine* Game::createMine(Mines type, float2 pos, Texture2D* texture, int rank)
{
    Mine* mine = nullptr;
    if (type == Mines::FLOATING)
        mine = createFloatingMine(pos, texture, rank);
    else if (type == Mines::FIRE)
        mine = createFireMine(pos, texture, rank);
    else if (type == Mines::MAGNETIC)
        mine = createMagneticMine(pos, texture, rank);
    else if (type == Mines::FIRE_MAGNETIC)
        mine = createFireMagneticMine(pos, texture, rank);

    if (mine && rank == 1)
        spawnEmitter->mineCreated();

    return mine;
}

FloatingMine* Game::createFloatingMine(float2 pos, Texture2D* texture, int rank)
{
    addEntity(std::make_unique<FloatingMine>(pos, texture, getSound(Sound_E::MINE_EXPLOSES), rank));
    return reinterpret_cast<FloatingMine*>(getBackEntity());
}

FireMine* Game::createFireMine(float2 pos, Texture2D* texture, int rank)
{
    addEntity(std::make_unique<FireMine>(pos, texture, getSound(Sound_E::MINE_EXPLOSES), rank));
    return reinterpret_cast<FireMine*>(getBackEntity());
}

MagneticMine* Game::createMagneticMine(float2 pos, Texture2D* texture, int rank)
{
    //define random goal
    float2* goal = &m_players[0]->pos();
    if (m_players.size() > 1) goal = rand() % 2 == 1 ? &m_players[0]->pos() : &m_players[1]->pos();

    addEntity(std::make_unique<MagneticMine>(pos, texture, getSound(Sound_E::MINE_EXPLOSES), rank, goal));
    return reinterpret_cast<MagneticMine*>(getBackEntity());
}

FireMagneticMine* Game::createFireMagneticMine(float2 pos, Texture2D* texture, int rank)
{
    //define random goal
    float2* goal = &m_players[0]->pos();
    if (m_players.size() > 1) goal = rand() % 2 == 1 ? &m_players[0]->pos() : &m_players[1]->pos();

    addEntity(std::make_unique<FireMagneticMine>(pos, texture, getSound(Sound_E::MINE_EXPLOSES), rank, goal));
    return reinterpret_cast<FireMagneticMine*>(getBackEntity());
}


void    Game::update()
{
    m_deltaTime = GetFrameTime();

    inputEngine->update(this);
    musicEngine->update(m_state);

    if (m_state == State::MENU)
        updateMenu();
    else if (m_state == State::INGAME)
        updateInGame();
    else if (m_state == State::PAUSE)
        updatePause();
    else if (m_state == State::GAME_OVER)
        updateGameOver();
}

void   Game::updateMenu()
{}

void    Game::updateInGame()
{
//update wave system
    spawnEmitter->update(m_deltaTime);

//update entities
    for (auto& entity : m_entities)
    {
        entity->update(m_deltaTime);

        addCollider(*entity);

        //display AABB and repere or not, according to the current state
        entity->m_showAABB = inputEngine->input.changeBoxShow;
    }

//update game according to player state
    //set score to null and add players' score
    m_score = 0;
    for (auto& p : m_players)
    {
        if (p->getLife() <= 0)
            m_state = State::GAME_OVER;

        m_score += p->m_score;
    }

//update according to mine layer state (destroy it if created but not "activate")
    if (m_spawnPoints.size() > 0 && m_mineLayer.size() > 0 && !m_mineLayer[0]->m_definitelyCreate)
        m_mineLayer[0]->m_isAlive = false;

//update collisions
    collisionEngine->update();

//update live and death
    updateCreateEntity();

    updateDeadEntity();
}

void    Game::updatePause()
{}

void    Game::updateGameOver()
{}

void    Game::updateCreateEntity()
{
    //Entities cannot access to game, so we have to create entity in this function

//Create spawn points each new waves
    for (int i = (int)spawnEmitter->m_spawnPoints.size() - 1; i >= 0; --i)
    {
        auto& s = spawnEmitter->m_spawnPoints[i];
        createSpawnPoint(s.type, s.pos, &getTexture(Texture_E::ATLAS_MINESTORM), s.time);
        spawnEmitter->m_spawnPoints.pop_back();
    }
    
    for (int i = 0; i < m_entities.size(); ++i)
    {
        Entity* en = m_entities[i].get();

//display score on the screen
        if (!en->m_isAlive && en->getScoring() != 0)
            createTextEntityMoving(en->pos(), std::to_string(en->getScoring()), 10, GREEN, 20.f, { 0.f, -1.f }, 5.f);

//create mines child/fireballs
        Mine* mine = dynamic_cast<Mine*>(en);
        if (mine)
        {
            if (mine->m_shouldFire)
            {
                createMineFireball(mine->pos(), &getTexture(Texture_E::ATLAS_MINESTORM));
                mine->m_shouldFire = false;
            }
            for (int i = (int)mine->childPos.size() - 1; i >= 0 && !mine->m_isAlive; --i)
            {
                createMine(mine->getType(), mine->childPos[i], &getTexture(Texture_E::ATLAS_MINESTORM), mine->getRank() + 1);
                mine->childPos.pop_back();
            }
        }

//create Big mine
        SpawnPoint* spawn = dynamic_cast<SpawnPoint*>(en);
        if (spawn && spawn->m_shouldSpawnMine)
        {
            createMine(spawn->getType(), spawn->pos(), &getTexture(Texture_E::ATLAS_MINESTORM), 1);
        }
    }

//Player fireball
    for (auto& p : m_players)
    {
        if (p->m_fire)
        {
            p->m_fire = false;
            Color color = p->getNumber() == 1 ? RED : GREEN;
            createPlayerFireball(p->pos(), p->m_ref.unitY, &getTexture(Texture_E::ATLAS_MINESTORM), &p->m_score, color);
        }
    }

//Mine Layer management

    //if there is not spawn, a mine layer is created
    if (m_spawnPoints.size() == 0 && m_mineLayer.size() == 0)
        createMineLayer();

    if (m_mineLayer.size() > 0 && m_mineLayer[0]->m_shouldCreateSpawn)
    {
        m_mineLayer[0]->m_shouldCreateSpawn = false;
        createSpawnPoint(spawnEmitter->getRandomType(), m_mineLayer[0]->pos(), &getTexture(Texture_E::ATLAS_MINESTORM), 3.f);
    }
}


void     Game::updateDeadEntity()
{

    for (int i = (int)m_spawnPoints.size() - 1; i >= 0; --i)
        if (!m_spawnPoints[i]->m_isAlive) m_spawnPoints.erase(m_spawnPoints.begin() + i);

    if (m_mineLayer.size() > 0 && !m_mineLayer[0]->m_isAlive) m_mineLayer.clear();

    for (int i = (int)m_entities.size()-1; i >= 0; --i)
    {
        if (!m_entities[i]->m_isAlive)
        {
            Mine* mine = dynamic_cast<Mine*>(m_entities[i].get());
            if (mine && mine->getRank() == 3)
                spawnEmitter->mineDestroyed();

            m_entities.erase(m_entities.begin() + i);
        }
    }

}


void    Game::draw()
{
    DrawTexture(ressources->m_textures[(int)Texture_E::BACKGROUND], SCREEN_WIDTH / 2 - 320, 0, WHITE);

    if (m_state == State::MENU)
        drawMenu();
    else if (m_state == State::INGAME)
        drawInGame();
    else if (m_state == State::PAUSE)
        drawPause();
    else if (m_state == State::GAME_OVER)
        drawGameOver();


    DrawTexture(ressources->m_textures[(int)Texture_E::FOREGROUND], SCREEN_WIDTH / 2 - 320, 0, WHITE);

    int life1 = m_players.size() > 0 ? m_players[0]->getLife() : 3;
    int score1 = m_players.size() > 0 ? m_players[0]->m_score : 0;
    drawPlayerOneInfo( SCREEN_WIDTH / 2 - GAME_WIDTH / 2 + 75, 75, life1, score1, getTexture(Texture_E::ATLAS_MINESTORM));

    if (m_players.size() > 1)
        drawPlayerTwoInfo(GAME_X_MAX - 225, 75, m_players[1]->getLife(), m_players[1]->m_score, getTexture(Texture_E::ATLAS_MINESTORM));

    DrawRectangle(0, 0, GAME_X_MIN, SCREEN_HEIGHT, BLACK);
    DrawRectangle(GAME_X_MAX, 0, GAME_X_MIN, SCREEN_HEIGHT, BLACK);
}

void    Game::drawMenu()
{
    DrawText("Do you want to play ?", SCREEN_WIDTH / 3 + (int)(sin(GetTime()) * 10), SCREEN_HEIGHT / 3,  25 + (int)fabs((sin(GetTime())) * 10), WHITE);
    DrawText("Press 'F':         Solo mode", 2 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3 + 100, 15, BLUE);
    DrawText("Press 'K':         Two players mode", 2 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3 + 125, 15, GREEN);
    DrawText("Press 'Esc':       Exit", 2 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3 + 150, 15, RED);
    DrawText("Press RIGHT/LEFT : Set Volume", 2 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3 + 200, 15, YELLOW);

//draw volume option
    float volume = musicEngine->getVolume();
    float spriteArea = volume < 0.05f ? 1.f : 0.f;
    DrawTexturePro(getTexture(Texture_E::VOLUME_LOGO), { spriteArea * 100.f, 100.f, 100.f, 100.f },
        { GAME_X_MAX - 150.f, GAME_Y_MIN + 125.f, 75.f, 75.f }, { 50.f, 50.f }, 90.f, WHITE);
    DrawText(TextFormat("%i", (int)(volume * 100.f)), GAME_X_MAX - 100, GAME_Y_MIN + 100, 30, WHITE);
}

void    Game::drawInGame()
{
    spawnEmitter->draw();

    for (int i = 0; i < m_entities.size(); ++i)
        m_entities[i]->draw();

    //players are draw in front of the mines and fireballs
    for (auto& p : m_players)
        p->draw(); 
}

void    Game::drawPause()
{
    DrawText("PAUSE", 3 * SCREEN_WIDTH / 7, SCREEN_HEIGHT / 3, 30 , WHITE);
    std::string score = getStrScore(m_score);
    DrawText(score.c_str(), 3 * SCREEN_WIDTH / 7, SCREEN_HEIGHT / 3 + 50, 15, GREEN);
    DrawText("Press 'SPACE':     Resume", 2 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3 + 150, 15, BLUE);
    DrawText("Press 'B':         Back to menu", 2 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3 + 175, 15, RED);
    DrawText("Press RIGHT/LEFT : Set Volume", 2 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3 + 225, 15, YELLOW);

//draw volume option
    float volume = musicEngine->getVolume();
    float spriteArea = volume < 0.05f ? 1.f : 0.f;
    DrawTexturePro(getTexture(Texture_E::VOLUME_LOGO), { spriteArea * 100.f, 100.f, 100.f, 100.f },
        { GAME_X_MAX - 150.f, GAME_Y_MIN + 125.f, 75.f, 75.f }, { 50.f, 50.f }, 90.f, WHITE);
    DrawText(TextFormat("%i", (int)(volume * 100.f)), GAME_X_MAX - 100, GAME_Y_MIN + 100, 30, WHITE);
}

void    Game::drawGameOver()
{
    DrawText("GAME OVER", 2 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3, 40, WHITE);

    std::string score = getStrScore(m_score);
    DrawText(score.c_str(), 3 * SCREEN_WIDTH / 7, SCREEN_HEIGHT / 3 + 50, 15, GREEN);
    std::string wave = "Wave : " + std::to_string(spawnEmitter->getWave());
    DrawText(wave.c_str(), 3 * SCREEN_WIDTH / 7, SCREEN_HEIGHT / 3 + 70, 15, YELLOW);

    DrawText("Press 'B':        Back to menu", 2 * SCREEN_WIDTH / 5, SCREEN_HEIGHT / 3 + 150, 15, BLUE);
}


void    Game::goToMenu()
{
    clearEntities();
    m_score = 0;
    m_state = State::MENU;
}

void    Game::goToSolo()
{
    Sound playerSounds[3] = { getSound(Sound_E::PLAYER_SHOT) , getSound(Sound_E::TELEPORT), getSound(Sound_E::PLAYER_HURT) };

    float2 pos = { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f };
    addEntity(std::make_unique<Player>(1, pos, &getTexture(Texture_E::ATLAS_MINESTORM), playerSounds));
    m_players.push_back(reinterpret_cast<Player*>(getBackEntity()));

    spawnEmitter = std::make_unique<SpawnEmitter>();

    m_state = State::INGAME;
}

void    Game::goToDuo()
{
    Sound playerSounds[3] = { getSound(Sound_E::PLAYER_SHOT) , getSound(Sound_E::TELEPORT), getSound(Sound_E::PLAYER_HURT) };

    float2 pos = { SCREEN_WIDTH / 2.f - GAME_WIDTH / 4.f, SCREEN_HEIGHT / 2.f };
    addEntity(std::make_unique<Player>(1, pos, &getTexture(Texture_E::ATLAS_MINESTORM), playerSounds));
    m_players.push_back(reinterpret_cast<Player*>(getBackEntity()));


    pos = { SCREEN_WIDTH / 2.f + GAME_WIDTH / 4.f, SCREEN_HEIGHT / 2.f };
    addEntity(std::make_unique<Player>(2, pos, &getTexture(Texture_E::ATLAS_MINESTORM), playerSounds));
    m_players.push_back(reinterpret_cast<Player*>(getBackEntity()));

    spawnEmitter = std::make_unique<SpawnEmitter>();

    m_state = State::INGAME;
}

void    Game::clear()
{
    clearEntities();
}

void    Game::clearEntities()
{
    m_entities.clear();
    m_players.clear();
    m_spawnPoints.clear();
    m_mineLayer.clear();
    collisionEngine->m_colliders.clear();
    spawnEmitter.reset();
}

