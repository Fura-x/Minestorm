#pragma once

#include "raylib.h"

#include "enums.h"
#include "component.h"

class Entity;

class SoundEmitter : public Component
{
private:
	Sound m_sound;

public:
	SoundEmitter(Entity& en);
	SoundEmitter(Entity& en, Sound sound);
	~SoundEmitter() = default;

	void setSound(Sound sound);

	void play();
};