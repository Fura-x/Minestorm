
#include "raylib.h"
#include "utils.h"

#include "particle.h"
#include "particle_move.h"

Particle::Particle(float2 pos, float2 dir, ParticleInfo info)
	:Entity(pos, nullptr)
{
//global info
	m_info = info;

//move
	addComponent(&m_move, true);
	m_move->setParameter(dir, m_info.lifeTime, m_info.speed);
}

void Particle::setDirection(const float2 dir)
{
	m_move->setDirection(dir);
}

void Particle::setInfo(const ParticleInfo info)
{
	m_info = info;
}

void Particle::resetMove(const float2 dir)
{
	m_move->resetParameter(dir);
}

void Particle::draw()
{
	auto& in = m_info;
	float l = m_move->getReduce();

	float endf[3] = { (float)in.endColor.r, (float)in.endColor.g, (float)in.endColor.b };

	for (int i = 0; i < 3; ++i) 
		in.curColor[i] = utils::lerp(in.curColor[i], endf[i], l);
	in.curSize = utils::lerp(in.curSize, in.endSize, l);

	Color color = { (unsigned char)in.curColor[0], (unsigned char)in.curColor[1], (unsigned char)in.curColor[2], (unsigned char)m_move->getAlpha() };

	DrawRectangle((int)pos().x, (int)pos().y, (int)m_info.curSize, (int)m_info.curSize, color);
}