#include "WindowTarget.h"

namespace KYEight 
{
	bool WindowTarget::Initialise() 
	{
		// No initialisation needed for window target, window created by application.
		return true;
	}

	void WindowTarget::Begin() 
	{
		BeginDrawing();
	}	
	void WindowTarget::End() 
	{
		EndDrawing();
	}

	void WindowTarget::Shutdown() 
	{
		// Again application owns window innit brev
	}

	// Getters
	bool WindowTarget::IsViewport() const
	{
		return false;
	}

	float WindowTarget::GetAspectRatio() const
	{
		// For now probably just return 0.0f, as window size can be dynamic and should be queried from the application.
		return 0.0f;
	}
	RenderTexture2D WindowTarget::GetViewportTexture() const
	{
		// Same as above mate
		return {};
	}
}