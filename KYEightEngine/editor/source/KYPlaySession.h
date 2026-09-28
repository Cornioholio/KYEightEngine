#pragma once
#include "core/KYEngine.h"

namespace KYEightEditor
{
	class KYPlaySession
	{
	public:
		KYPlaySession() = default;
		~KYPlaySession() = default;

		// Disable copy/move
		KYPlaySession(const KYPlaySession&) = delete;
		KYPlaySession& operator=(const KYPlaySession&) = delete;

		KYPlaySession(KYPlaySession&&) = delete;
		KYPlaySession& operator=(KYPlaySession&&) = delete;

		bool Start(KYEight::KYEngine& engine);
		void Update(KYEight::KYEngine& engine);
		bool Stop(KYEight::KYEngine& engine);

		bool isPlaying() const;

	private:
		bool playing = false;

	};
}