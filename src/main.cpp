#include <raylib.h>


int main()
{
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(600, 600, "Game");

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);


		EndDrawing();
	}

	CloseWindow();
}
