
#include "raylib.h"
#include "math_toolbox.h"

#include "enums.h"
#include "defines.h"

#include "game.h"
#include "fire_magnetic_mine.h"
#include "follow_move.h"
#include "concave_collider.h"

static convex_polygon* getFloatingConvexs(float x, float y, float m_size)
{
	float nearDist = 65.f, farDist = 100.f;

	float rot = 0.f;
	float2* pts1 = new (float2[3]){ {x - (sinf((rot + 1.f) * M_PI / 4.f) * nearDist * m_size), y - (cosf((rot + 1.f) * M_PI / 4.f) * farDist * m_size)}, {x - (sinf((rot + 3.f) * M_PI / 4.f) * nearDist * m_size), y - (cosf((rot + 3.f) * M_PI / 4.f) * nearDist * m_size)}, {x - (sinf((rot + 4.f) * M_PI / 4.f) * nearDist * m_size), y - (cosf((rot + 4.f) * M_PI / 4.f) * nearDist / 1.5f * m_size)} };
	rot += 2.f;
	float2* pts2 = new (float2[3]){ {x - (sinf((rot + 1.f) * M_PI / 4.f) * farDist * m_size), y - (cosf((rot + 1.f) * M_PI / 4.f) * nearDist * m_size)}, {x - (sinf((rot + 3.f) * M_PI / 4.f) * nearDist * m_size), y - (cosf((rot + 3.f) * M_PI / 4.f) * nearDist * m_size)}, {x - (sinf((rot + 4.f) * M_PI / 4.f) * nearDist / 1.5f  * m_size), y - (cosf((rot + 4.f) * M_PI / 4.f) * nearDist * m_size)} };
	rot += 2.f;
	float2* pts3 = new (float2[3]){ {x - (sinf((rot + 1.f) * M_PI / 4.f) * nearDist * m_size), y - (cosf((rot + 1.f) * M_PI / 4.f) * farDist * m_size)}, {x - (sinf((rot + 3.f) * M_PI / 4.f) * nearDist * m_size), y - (cosf((rot + 3.f) * M_PI / 4.f) * nearDist * m_size)}, {x - (sinf((rot + 4.f) * M_PI / 4.f) * nearDist * m_size), y - (cosf((rot + 4.f) * M_PI / 4.f) * nearDist / 1.5f * m_size)} };
	rot += 2.f;
	float2* pts4 = new (float2[3]){ {x - (sinf((rot + 1.f) * M_PI / 4.f) * farDist * m_size), y - (cosf((rot + 1.f) * M_PI / 4.f) * nearDist * m_size)}, {x - (sinf((rot + 3.f) * M_PI / 4.f) * nearDist * m_size), y - (cosf((rot + 3.f) * M_PI / 4.f) * nearDist * m_size)}, {x - (sinf((rot + 4.f) * M_PI / 4.f) * nearDist / 1.5f * m_size), y - (cosf((rot + 4.f) * M_PI / 4.f) * nearDist * m_size)} };

	convex_polygon* convexs = new (convex_polygon[4]){
		{pts1, 3}, {pts2, 3}, {pts3, 3}, {pts4, 3}
	};

	return convexs;
}

FireMagneticMine::FireMagneticMine(float2 _pos, Texture2D* texture, Sound explosion, int rank, float2* playerPos)
	:Mine(_pos, texture, explosion, Mines::FIRE_MAGNETIC, rank)
{
	//move component
	addComponent(&m_move, true);
	m_move->setSpeed((float)m_rank * 15.f);
	m_move->setGoal(playerPos);

	//scoring
	int scoring = rank == 1 ? 750 : 785;
	if (rank >= 3) scoring = 850;
	m_scoring = scoring;

	//collider || we allocate memory, and it will be delete in the destructor of concaveCollider

	setSize();

	addComponent(&m_collider, true);
	m_collider->setCollider({ getFloatingConvexs(pos().x, pos().y, m_size), 4 });
	m_collider->setTagAndMask(u64 ColliderTag::MINE, u64 ColliderTag::PLAYER + u64 ColliderTag::PLAYER_FIREBALL);

	m_colliderToAdd = m_collider;
}

void FireMagneticMine::collisionHappens(Collider& other)
{
	if (other.getTag() == u64 ColliderTag::PLAYER_FIREBALL)
		mineExplose();
}

void FireMagneticMine::mineExplose()
{
	Mine::mineExplose();
	m_collider->m_isActivate = false;
	m_move->m_isActivate = false;
	m_shouldFire = true;
}

void FireMagneticMine::draw()
{
	if (m_explosionTimer->m_isActivate)
	{
		Mine::draw();
		return;
	}

	DrawTexturePro(*m_texture, { 2 * ATLAS_SPRITE_SIZE, ATLAS_SPRITE_SIZE, ATLAS_SPRITE_SIZE, ATLAS_SPRITE_SIZE },
		{ pos().x, pos().y, ATLAS_SPRITE_SIZE * m_size, ATLAS_SPRITE_SIZE * m_size },
		{ (ATLAS_SPRITE_SIZE / 2) * m_size, (ATLAS_SPRITE_SIZE / 2) * m_size }, rotation(), WHITE);

	rotate(0.1f);

	//AABB and referential
	if (m_showAABB)
	{
		box rect = m_collider->m_AABB;
		DrawRectangleLinesEx({ rect.center.x - rect.width_ha, rect.center.y - rect.height_ha, rect.width_ha * 2, rect.height_ha * 2 }, 1, DARKBLUE);
		drawReferential();

		//draw polygons
		auto& concave = m_collider->m_concave;
		for (int convex = 0; convex < 4; ++convex)
		{
			for (int pt = 0; pt < 2; ++pt)
			{
				DrawLineEx(concave.polygons[convex].pts[pt], concave.polygons[convex].pts[pt + 1], 2.f, DARKBLUE);
			}
			DrawLineEx(concave.polygons[convex].pts[2], concave.polygons[convex].pts[0], 2.f, DARKBLUE);
		}
	}

}