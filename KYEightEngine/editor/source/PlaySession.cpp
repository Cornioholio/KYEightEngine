#include "PlaySession.h"

namespace KYEightEditor
{

	bool PlaySession::Start(KYEight::KYEngine& engine)
	{
		if (playing) 
		{
			return false;
		}

		playing = true;

		return true;
	}
	void PlaySession::Update(KYEight::KYEngine& engine)
	{
		if (!playing) 
		{
			return;
		}

		engine.Update();
	}
	bool PlaySession::Stop(KYEight::KYEngine& engine)
	{
		if (!playing) 
		{
			return false;
		}

		playing = false;

	}

	bool PlaySession::isPlaying() const
	{
		return playing;
	}
}