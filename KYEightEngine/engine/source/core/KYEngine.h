#pragma once
#include "renderer/KYRenderer.h"

#include <functional>
#include <iostream>
namespace KYEight
{
	class KYEngine
	{
	public:
		KYEngine();
		~KYEngine();

		// Disable move and copy semantics
		KYEngine(const KYEngine&) = delete;
		KYEngine& operator=(const KYEngine&) = delete;

		KYEngine(const KYEngine&&) = delete;
		KYEngine& operator=(KYEngine&&) = delete;

		bool Initialise();
		void Run();
		void Shutdown();

		void SetEditorCallback(std::function<void()> callback);
	private:
		void Update();

		std::function<void()> editorCallback;
		KYRenderer* renderer;

	};
}