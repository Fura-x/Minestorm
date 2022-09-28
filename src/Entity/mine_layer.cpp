
#include "raylib.h"
#include "math_toolbox.h"

#include "enums.h"
#include "defines.h"

#include "mine_layer.h"
#include "spawn.h"
#include "concave_collider.h"
#include "mine_layer_move.h"
#include "sound_emitter.h"

static convex_polygon* getConvexs(float x, float y)
{
	float width_half = 55.f, height_half = 9.f, sideTriangle = 20.f;

	float2* pts1 = new (float2[4]){ {x - width_half, y + height_half}, {x - width_half, y - height_half}, {x + width_half, y - height_half}, {x + width_half, y + height_half} };
	float2* pts2 = new (float2[3]){ {x - width_half / 3.f, y - height_half}, { x, y - height_half * 2}, {x + width_half / 3.f, y - height_half} };
	float2* pts3 = new (float2[3]){ {x - width_half, y - height_half}, { (x - width_half) + cosf(4.f * M_PI / 3.f) * sideTriangle, (y + height_half) - sinf(4.f * M_PI / 3.f) * sideTriangle}, {x - width_half, y + height_half} };
	float2* pts4 = new (float2[3]){ {x + width_half, y - height_half}, { (x + width_half) + cosf( - M_PI / 3.f) * sideTriangle, (y + height_half) - sinf( - M_PI / 3.f) * sideTriangle}, {x + width_half, y + height_half} };

	convex_polygon* convexs = new (convex_polygon[4]){
		{pts1, 4}, {pts2, 3}, {pts3, 3}, {pts4, 3}
	};

	return convexs;
}

MineLayer::MineLayer(float2 _pos, Texture2D* texture, Sound explosion, float2 goal)
	:Entity(_pos, texture)
{
//scoring
	m_scoring = 1000;

//spawn component
	addComponent(&m_spawn, true);
	m_spawn->setParameter(25.f, std::bind(&MineLayer::beginMove, this));
	m_spawn->startTimer();

//move
	addComponent(&m_move, false);
	m_move->setParameter(goal, &m_shouldCreateSpawn);

//explosion sound
	addComponent(&m_explosionSound, true);
	m_explosionSound->setSound(explosion);

//collider || we allocate memory, and it will be delete in the destructor of concaveCollider

	addComponent(&m_collider, false);
	m_collider->setCollider({ getConvexs(pos().x, pos().y), 4 });
	m_collider->setTagAndMask(u64 ColliderTag::MINE, u64 ColliderTag::PLAYER + u64 ColliderTag::PLAYER_FIREBALL);

	m_colliderToAdd = m_collider;
}

void MineLayer::beginMove()
{
	m_move->m_isActivate = true;
	m_collider->m_isActivate = true;
	m_definitelyCreate = true;
	m_spawn->m_isActivate = false;
}

void MineLayer::collisionHappens(Collider& other)
{
	if (other.getTag() == u64 ColliderTag::PLAYER_FIREBALL)
		entityDead();
}

void MineLayer::entityDead()
{
	m_isAlive = false;
	
	if (m_explosionSound) m_explosionSound->play();
}

void MineLayer::draw()
{
	float size = 0.5f;
	DrawTexturePro(*m_texture, { 2 * ATLAS_SPRITE_SIZE, 0, ATLAS_SPRITE_SIZE, ATLAS_SPRITE_SIZE },
		{ pos().x, pos().y, ATLAS_SPRITE_SIZE * size, ATLAS_SPRITE_SIZE * size},
		{ (ATLAS_SPRITE_SIZE / 2 * size), (ATLAS_SPRITE_SIZE / 2 * size) }, rotation(), WHITE);

	//AABB and referential
	if (m_showAABB)
	{
		box rect = m_collider->m_AABB;
		DrawRectangleLinesEx({ rect.center.x - rect.width_ha, rect.center.y - rect.height_ha, rect.width_ha * 2, rect.height_ha * 2 }, 1, DARKBLUE);
		drawReferential();

		//draw polygons
		auto& concave = m_collider->m_concave;
		for (int pt = 0; pt < 3; ++pt)
		{
			DrawLineEx(concave.polygons[0].pts[pt], concave.polygons[0].pts[pt + 1], 2.f, DARKBLUE);
		}
		DrawLineEx(concave.polygons[0].pts[3], concave.polygons[0].pts[0], 2.f, DARKBLUE);
		for (int convex = 1; convex < 4; ++convex)
		{
			for (int pt = 0; pt < 2; ++pt)
			{
				DrawLineEx(concave.polygons[convex].pts[pt], concave.polygons[convex].pts[pt + 1], 2.f, DARKBLUE);
			}
			DrawLineEx(concave.polygons[convex].pts[2], concave.polygons[convex].pts[0], 2.f, DARKBLUE);
		}
	}

}