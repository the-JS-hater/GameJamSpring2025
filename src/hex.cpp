#include "../inc/hex.hpp"

#include <stdio.h>

ElectricHex::ElectricHex(
	float scale, float offset,
	float x, float y,
	float w, float h
)
{
	outerBox = Rectangle { 
		scale * x, 
		scale * y, 
		scale * w, 
		scale * h
	};

	Triangle temp_corners[4] =
	{
		Triangle {
			Vector2 { scale * x, scale * y },
			Vector2 { scale * (x + offset), scale * y},
			Vector2 { scale * x, scale * (y + h/2) }
		},
		Triangle {
			Vector2 { scale * (x + w), scale * y },
			Vector2 { scale * (x + w - offset), scale * y },
			Vector2 { scale * (x + w), scale * (y + h/2) }
		},
		Triangle {
			Vector2 { scale * (x + w), scale * (y + h/2) },
			Vector2 { scale * (x + w), scale * (y + h) },
			Vector2 { scale * (x + w - offset), scale * (y + h) }
		},
		Triangle {
			Vector2 { scale * x, scale * (y + h/2) },
			Vector2 { scale * (x + offset), scale * (y + h) },
			Vector2 { scale * x, scale * (y + h) }
		}
	};
	std::copy(temp_corners, &temp_corners[4], corners);
}

void ElectricHex::draw()
{
	DrawRectangleLinesEx(
		outerBox,
		3,
		RED
	);

	for (auto && [v1, v2, v3]: corners)
	{
		DrawTriangleLines(v1, v2, v3, BLUE); 
	}
}

bool ElectricHex::inBounds(Rectangle aabb)
{
	auto &&[x,y,w,h] = aabb;
	Vector2 points[4] {
		Vector2 {x, y},
		Vector2 {x + w, y},
		Vector2 {x + w, y + h},
		Vector2 {x, y + h}
	};

	for (auto p: points)
	{
		if (!CheckCollisionPointRec(p, outerBox)) 
		{
			return false;
		}


		for (auto &&[p1,p2,p3]: corners) 
		{
			if (CheckCollisionPointTriangle(p,p1,p2,p3)) 
			{
				return false;			
			}
		}
	}
	return true;
}


void ElectricHex::resolveCollision(ECS& ecs, Entity id)
{
  Position* pos = ecs.positions.getComponent(id);
  Collider* col = ecs.colliders.getComponent(id);
  Velocity* vel = ecs.velocities.getComponent(id);
	Acceleration* acc = ecs.accelerations.getComponent(id);
  Mass* mass = ecs.masses.getComponent(id);
		
	if (!col || !vel || !mass || !acc || !pos) return;

	vel->vx *= -1;
	vel->vy *= -1;
  pos->x += vel->vx; 
  pos->y += vel->vy; 
  acc->accX *= 0.0f;
  acc->accY *= 0.0f;
	return;
}

void ElectricHex::checkHexBounds(ECS& ecs, Player& p_1, Player& p_2)
{
  for (Entity id = 0; id < ecs.nextEntity; ++id) 
	{
		Collider* collider = ecs.colliders.getComponent(id);
		if (!collider) continue;
		if (inBounds(collider->rect)) continue;

    if (id == p_1.body) p_2.score++;
    if (id == p_2.body) p_1.score++;

    if (id == p_2.body || id == p_1.body) {
      p_1.respawn(ecs);
      p_2.respawn(ecs);
    }

		resolveCollision(ecs, id);
	}
}
