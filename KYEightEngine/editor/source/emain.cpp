#include "KYEditor.h"

int main() 
{
	KYEightEditor::KYEditor editor;

	if (!editor.Initialise()) 
	{
		return -1;
	}

	editor.Run();

	editor.Shutdown();

	return 0;
}