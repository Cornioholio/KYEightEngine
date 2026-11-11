#include "KYEditor.h"

// Game editor entry point
int main() 
{
	KYEightEditor::KYEditor editor;

	if (!editor.Initialise()) 
	{
		return -1;
	}

	// Render editor UI
	editor.Run();

	editor.Shutdown();

	return 0;
}