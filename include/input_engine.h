#pragma once

#include "defines.h"

class Game;

struct Input
{
    bool    move1 = false, right1 = false, left1 = false, shoot1 = false;
    bool    teleport1 = false;
    bool    move2 = false, right2 = false, left2 = false, shoot2 = false;
    bool    teleport2 = false;
    bool    changeBoxShow = false;

    bool    pause = false, exit = false;
    bool    volumeDown = false, volumeUp = false;
};

class InputEngine
{
private:
    
public:
    InputEngine() = default;
    ~InputEngine() = default;

    Input input;

    void    update(Game*);
    void    apply(Game*);
};
