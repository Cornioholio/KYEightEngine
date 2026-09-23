#include "core/KYEngine.h"

#include <iostream>
#include <raylib.h>
#include "rlImGui.h"
#include "imgui.h"

int main() 
{
	InitWindow(1280, 720, "KYEightEngine");

	rlImGuiSetup(true);

	while(!WindowShouldClose()) 
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		rlImGuiBegin();

		ImGui::Begin("KYEightEngine");

		ImGui::Text("Work please");

		if (ImGui::Button("Hello"))
		{
			std::cout << "Works!\n";
		}

		ImGui::End();

		rlImGuiEnd();

		EndDrawing();
	}
	rlImGuiShutdown();

	CloseWindow();

	return 0;
}