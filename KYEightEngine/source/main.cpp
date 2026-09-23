#include <iostream>
#include <raylib.h>

int main() 
{
	InitWindow(1280, 720, "KYEightEngine");

	while(!WindowShouldClose()) 
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawText("Engine", 50, 50, 40, BLACK);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}