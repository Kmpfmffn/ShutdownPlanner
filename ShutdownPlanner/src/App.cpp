#include "App.h"




bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame();
	int displayX;
	int displayY;
	wxDisplaySize(&displayX, &displayY);
	mainFrame->Move(wxPoint(displayX/3, displayY/3));
	mainFrame->Show(true);
	return true;
}


App::App() {
}
App::~App() {
}