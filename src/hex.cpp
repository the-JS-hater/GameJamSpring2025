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
  Collider* col = ecs.colliders.getComponent(id);
  Velocity* vel = ecs.velocities.getComponent(id);
	Mass* mass = ecs.masses.getComponent(id);
		
	if (!col || !vel || !mass) return;

	Vector2 pos = { col->rect.x, col->rect.y };

	vel->vx *= -1;
	vel->vy *= -1;
	return;
}

void ElectricHex::checkHexBounds(ECS& ecs)
{
  for (Entity id = 0; id < ecs.nextEntity; ++id) 
	{
		Collider* collider = ecs.colliders.getComponent(id);
		if (!collider) continue;
		if (inBounds(collider->rect)) continue;

		resolveCollision(ecs, id);
	}
}
