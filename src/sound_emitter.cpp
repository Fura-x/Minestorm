
#include "entity.h"
#include "sound_emitter.h"

SoundEmitter::SoundEmitter(Entity& en)
	:Component(en)
{}

SoundEmitter::SoundEmitter(Entity& en, Sound sound)
	:Component(en), m_sound(sound)
{}

void SoundEmitter::setSound(Sound sound)
{
	m_sound = sound;
}

void SoundEmitter::play()
{
	PlaySound(m_sound);
}