#pragma once
#include <string>
#include "raylib.h"

namespace KYEight 
{
	/// <summary>
	/// Given to scene objects by default, holds geometry position, rotation and scale.
	/// </summary>
	struct Transform3D
	{
		Vector3 position_ = { 0.f, 0.f, 0.f };
		Vector3 rotation_ = { 0.f, 0.f, 0.f };
		Vector3 scale_ = { 1.f, 1.f, 1.f };
	};
	

	struct MeshComponent 
	{
		Model model_;
		Color colour_ = RED;
	};

	/// <summary>
	/// Human readable name displayed in editor
	/// </summary>
	struct NameComponent 
	{
		std::string name_ = "";
	};

}