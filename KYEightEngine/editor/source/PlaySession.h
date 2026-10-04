#pragma once
#include "core/KYEngine.h"

namespace KYEightEditor
{
	class PlaySession
	{
	public:
		PlaySession() = default;
		~PlaySession() = default;

		// Disable copy/move
		PlaySession(const PlaySession&) = delete;
		PlaySession& operator=(const PlaySession&) = delete;

		PlaySession(PlaySession&&) = delete;
		PlaySession& operator=(PlaySession&&) = delete;

		bool Start(KYEight::KYEngine& engine);
		void Update(KYEight::KYEngine& engine);
		bool Stop(KYEight::KYEngine& engine);

		bool isPlaying() const;

	private:
		bool playing = false;

	};
}