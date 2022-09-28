#pragma once

#include <functional>
#include <string>

#include "enums.h"
#include "entity.h"

#include "spawn.h"
#include "bullet_move.h"

class SpawnComponent;
class BulletMove;

class TextEntity : public Entity
{
private:
	SpawnComponent* m_showTime = nullptr;
	BulletMove* m_move = nullptr;

	Font*		m_font = nullptr;
	std::string	m_text = "";

	int m_size = 10;
	Color m_color = GREEN;

public:
	TextEntity(float2 pos, std::string text, int size = 10.f, Color = GREEN) : Entity(pos, nullptr), m_text(text) {}
	~TextEntity() = default;

	void setDeadTimer(float time)
	{
		addComponent(&m_showTime, true);
		m_showTime->setParameter(time, std::bind(&TextEntity::entityDead, this));
	}
	void setBulletMove(float speed, float2 dir, float maxDistance)
	{
		addComponent(&m_move, true);
		m_move->setDirection(dir);
		m_move->setSpeed(speed);
		m_move->setMaxDistance(maxDistance);
	}

	void draw() override {
		if (!m_font)
			DrawText(m_text.c_str(), (int)pos().x - (int)m_text.size() * m_size / 2, (int)pos().y, m_size, m_color);
	}
};

