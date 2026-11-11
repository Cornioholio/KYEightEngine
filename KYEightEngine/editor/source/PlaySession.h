#pragma once
#include "core/KYEngine.h"

namespace KYEightEditor
{
	/// <summary>
	/// Handles events regarding engine playtime, basically handles whether the engine should update inside the editor.
	/// </summary>
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

		/// <summary>
		/// Begins a play session where the engine is allowed to update.
		/// </summary>
		/// <param name="engine"></param>
		/// <returns>True if not already playing</returns>
		bool Start(KYEight::KYEngine& engine);

		/// <summary>
		/// Calls engine update every frame if play session is started
		/// </summary>
		/// <param name="engine"></param>
		void Update(KYEight::KYEngine& engine);

		/// <summary>
		/// Stops a play session and ceases engine updates.
		/// </summary>
		/// <param name="engine"></param>
		/// <returns></returns>
		bool Stop(KYEight::KYEngine& engine);

		bool isPlaying() const;

	private:
		bool playing = false;

	};
}