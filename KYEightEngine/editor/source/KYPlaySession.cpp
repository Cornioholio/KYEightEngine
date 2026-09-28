#include "KYPlaySession.h"

namespace KYEightEditor
{

	bool KYPlaySession::Start(KYEight::KYEngine& engine) 
	{
		if (playing) 
		{
			return false;
		}

		playing = true;

		return true;
	}
	void KYPlaySession::Update(KYEight::KYEngine& engine) 
	{
		if (!playing) 
		{
			return;
		}

		engine.Update();
	}
	bool KYPlaySession::Stop(KYEight::KYEngine& engine) 
	{
		if (!playing) 
		{
			return false;
		}

		playing = false;

	}

	bool KYPlaySession::isPlaying() const 
	{
		return playing;
	}
}