#include "../inc/hex.hpp"


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
		//auto [v1, v2, v3]
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
		if (!CheckCollisionPointRec(p, outerBox)) return false;

		for (auto &&[p1,p2,p3]: corners) 
		{
			if (CheckCollisionPointTriangle(p,p1,p2,p3)) return false;			
		}
	}
	return true;
}
