#pragma once
#include <string>
#include "raylib.h"
#include <optional>
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
	/// <summary>
	/// Supported procedural mesh types
	/// </summary>
	enum class MeshPrimitive 
	{
		Cube,
		Sphere
	};
	/// <summary>
	/// Describes a procedural mesh independently of its runtime model
	/// </summary>
	struct PrimitiveMeshData 
	{
		MeshPrimitive type_ = MeshPrimitive::Cube;

		// Cube: width, height, depth
		// Sphere: radius in x
		Vector3 dimensions_ = { 1.f, 1.f, 1.f };

		// When generating spheres
		int slices_ = 16;
		int rings_ = 16;
	};

	using ModelHandle = uint32_t;

	struct MeshComponent 
	{
		ModelHandle modelHandle_ = 0;
		Color colour_ = RED;

		std::optional<PrimitiveMeshData> primitive_;
		std::optional<std::string> filepath_;
	};

	/// <summary>
	/// Human readable name displayed in editor
	/// </summary>
	struct NameComponent 
	{
		std::string name_ = "";
	};

}