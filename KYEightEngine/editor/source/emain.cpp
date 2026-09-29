#include "KYEditor.h"

// Game editor entry point
int main() 
{
	KYEightEditor::KYEditor editor(KYEight::KYRenderer::RenderTarget::Viewport);

	if (!editor.Initialise()) 
	{
		return -1;
	}

	// Render editor ui
	editor.Run();

	editor.Shutdown();

	return 0;
}