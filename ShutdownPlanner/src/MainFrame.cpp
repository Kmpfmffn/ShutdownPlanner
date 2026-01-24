#include "MainFrame.h"


#define UNICODE_PLAY wxString::FromUTF8("\xE2\x8F\xB5")
#define UNICODE_PAUSE wxString::FromUTF8("\xE2\x8F\xB8")
#define UNICODE_STOP wxString::FromUTF8("\xE2\x8F\xB9")

#ifdef IS_DEBUG
	#define LOG(str) SetStatusText(str);
#else
	#define LOG(str)
#endif //IS_DEBUG 

enum {
	ID_NULL = 0,
	ID_EDIT_TIMES,
	ID_Hello,
};

// TODO: 
// - Make timer editable from settings
// - Add icon
// - Change about section
// - Push to github [done]
// - Add to website
// - Desktop dimmer implementation
// 
//

// TODO: delete
wxPanel* mainPanel;
wxBoxSizer* topSizer;
//

MainFrame::MainFrame()
	: wxFrame(nullptr, wxID_ANY, "ShutdownPlanner"),
	  m_Clock(new Clock)
{
	// Non GUI initialization
	m_wxTimer.Bind(wxEVT_TIMER, &MainFrame::onTick, this);
	m_wxTimer.Start(16);

	// Build MenuBar
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_EDIT_TIMES, "Edit Timers\tF1");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, "Close\tEsc");
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
	#ifdef IS_DEBUG
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");
	#endif // IS_DEBUG
	// Bind Events to MenuBar
	Bind(wxEVT_MENU, [&](wxCommandEvent&) {
		m_PlayPauseButton->SetBackgroundColour(wxColor(100, 100, 100));
		/*wxTopLevelWindow* topLevelWindow = new wxTopLevelWindow(this, wxID_ANY, "Top Level");
		topLevelWindow->Show(true);
		wxPanel* panel = new wxPanel(topLevelWindow, wxID_ANY);*/
		}, ID_EDIT_TIMES);
	Bind(wxEVT_MENU, [=](wxCommandEvent&) { wxMessageBox("This is the content!", "Title", wxOK | wxICON_INFORMATION); }, wxID_ABOUT);
	Bind(wxEVT_MENU, [=](wxCommandEvent&) { Close(true); }, wxID_EXIT);


	// Content of MainFrame
	wxPanel* mainPanel = new wxPanel(this, wxID_ANY);
	mainPanel->SetFont(mainPanel->GetFont().Scale(1.8f).Bold());
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL); // BoxSizer containing all content of mainPanel
	wxBoxSizer* timerSizer = new wxBoxSizer(wxHORIZONTAL); // BoxSizer containing fixed timer buttons
	wxBoxSizer* customTimerSizer = new wxBoxSizer(wxHORIZONTAL); // BoxSizer containing content for custom timer
	wxBoxSizer* abortPauseSizer = new wxBoxSizer(wxHORIZONTAL); // BoxSizer containing abort and play/pause buttons
	wxBoxSizer* clockSizer = new wxBoxSizer(wxHORIZONTAL); // BoxSizer containing abort and play/pause buttons

	// Content of timerSizer
	// Concstruct buttons for timerSizer
	wxButton* btn_5min	= new wxButton(mainPanel, wxID_ANY, "5 min");
	wxButton* btn_30min = new wxButton(mainPanel, wxID_ANY, "30 min");
	wxButton* btn_45min = new wxButton(mainPanel, wxID_ANY, "45 min");
	wxButton* btn_1h	= new wxButton(mainPanel, wxID_ANY, "1 h");
	wxButton* btn_2h	= new wxButton(mainPanel, wxID_ANY, "2 h");
	wxButton* btn_3h	= new wxButton(mainPanel, wxID_ANY, "3 h");
	wxButton* btn_force	= new wxButton(mainPanel, wxID_ANY, "FORCE");
			  btn_force->SetBackgroundColour(wxColor(200, 0, 0)); // dark red
	// Add buttons to timerSizer
	timerSizer->Add(btn_5min,	wxSizerFlags(1).Expand());
	timerSizer->Add(btn_30min,	wxSizerFlags(1).Expand());
	timerSizer->Add(btn_45min,	wxSizerFlags(1).Expand());
	timerSizer->Add(btn_1h,		wxSizerFlags(1).Expand());
	timerSizer->Add(btn_2h,		wxSizerFlags(1).Expand());
	timerSizer->Add(btn_3h,		wxSizerFlags(1).Expand());
	timerSizer->Add(btn_force,	wxSizerFlags(1).Expand());
	// Bind events to buttons
	btn_5min->Bind(wxEVT_BUTTON,	[=](wxCommandEvent&) { startTimer(300); });
	btn_30min->Bind(wxEVT_BUTTON,	[=](wxCommandEvent&) { startTimer(1800); });
	btn_45min->Bind(wxEVT_BUTTON,	[=](wxCommandEvent&) { startTimer(2700); });
	btn_1h->Bind(wxEVT_BUTTON,		[=](wxCommandEvent&) { startTimer(3600); });
	btn_2h->Bind(wxEVT_BUTTON,		[=](wxCommandEvent&) { startTimer(7200); });
	btn_3h->Bind(wxEVT_BUTTON,		[=](wxCommandEvent&) { startTimer(10800); });
	btn_force->Bind(wxEVT_BUTTON,	[=](wxCommandEvent&) { startTimer(10); });
	// Add timerSizer to topSizer
	timerSizer->SetMinSize(wxSize(-1, 75));
	topSizer->Add(timerSizer, wxSizerFlags(1).Expand());

	// Content of customTimerSizer
	// Construct elements for customTimerSizer
	wxStaticText* customTimerLabel = new wxStaticText(mainPanel, wxID_ANY, "Custom time: ");
	wxTextCtrl* customTimerTextCtrl = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	wxButton* btn_send = new wxButton(mainPanel, wxID_ANY, "SEND");
	// Add elements to customTimerSizer
	customTimerSizer->Add(customTimerLabel, wxSizerFlags(0).Left().Align(wxALIGN_CENTER_VERTICAL));
	customTimerSizer->Add(customTimerTextCtrl, wxSizerFlags(1).Left().Align(wxALIGN_CENTER_VERTICAL));
	customTimerSizer->Add(btn_send, wxSizerFlags(0).Left().Align(wxALIGN_CENTER_VERTICAL));
	// Bind events to elements
	customTimerTextCtrl->Bind(wxEVT_TEXT_ENTER, [=](wxCommandEvent&) { sendCustomTimer(wxString(customTimerTextCtrl->GetValue())); });
	btn_send->			 Bind(wxEVT_BUTTON,		[=](wxCommandEvent&) { sendCustomTimer(wxString(customTimerTextCtrl->GetValue())); });
	// Add customTimerSizer to topSizer
	topSizer->Add(customTimerSizer, wxSizerFlags(1).Expand());

	// Construct abort and play/pause buttons
	wxButton* btn_abort = new wxButton(mainPanel, wxID_ANY, "ABORT");
	m_PlayPauseButton = new wxButton(mainPanel, wxID_ANY, UNICODE_PAUSE);
	m_PlayPauseButton->SetFont(m_PlayPauseButton->GetFont().Scale(2.0f));
	abortPauseSizer->Add(btn_abort, wxSizerFlags(1).Expand());
	abortPauseSizer->Add(m_PlayPauseButton, wxSizerFlags(0).Expand());
	btn_abort->SetBackgroundColour(wxColor(255, 200, 0)); // gold
	btn_abort->			Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { abortTimer(); });
	m_PlayPauseButton-> Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { playPauseTimer(); });
	abortPauseSizer->SetMinSize(wxSize(-1, 75));
	topSizer->Add(abortPauseSizer, wxSizerFlags(1).Expand());

	// Construct clock
	m_ClockStaticText = new wxStaticText(mainPanel, wxID_ANY, UNICODE_PLAY + m_Clock->toString(0));
	clockSizer->Add(m_ClockStaticText, wxSizerFlags(1).Expand());
	m_ClockStaticText->SetFont(m_ClockStaticText->GetFont().Scale(1.5f));
	m_ClockStaticText->SetMinSize(wxSize(200, -1));
	topSizer->Add(clockSizer, wxSizerFlags(1).CenterHorizontal());


	mainPanel->SetSizer(topSizer);
	topSizer->SetSizeHints(this);
}

// Timer controlling methods
void MainFrame::startTimer(int seconds) {
	if (m_Clock->hasActiveTimer()) {
		abortTimer();
	}
	std::string command = "shutdown -s -t " + std::to_string(seconds);
	wxShell(command);
	LOG("Start timer: " + command);
	m_Clock->startTimerNow(seconds);
}
void MainFrame::abortTimer() {
	std::string command = "shutdown -a";
	wxShell(command);
	LOG("Abort timer: " + command);
	m_Clock->stopTimer();
	m_PlayPauseButton->SetLabel(UNICODE_PLAY);
}
void MainFrame::playPauseTimer() {
	if (m_Clock->hasRunningTimer()) {
		std::string command = "shutdown -a";
		wxShell(command);
		LOG("Abort timer: " + command);
		m_Clock->pauseTimer();
		m_PlayPauseButton->SetLabel(UNICODE_PLAY);
	}
	else if (m_Clock->hasPausedTimer()) {
		std::string command = "shutdown -s -t " + std::to_string(m_Clock->getRemainingTime());
		m_Clock->continueTimer();
		wxShell(command);
		LOG("Continue timer: " + command);
		m_PlayPauseButton->SetLabel(UNICODE_PAUSE);
	}
	else {
		LOG("Invalid timer status!");
	}
}
void MainFrame::sendCustomTimer(wxString str) {
	// TODO: make better

	int seconds = 300;
	if (str == "abort") {
		LOG("RUN: shutdown -a");
		//abortTimer();
		m_Clock->stopTimer();
	}
	else if (str.IsNumber()) {
		str.ToInt(&seconds);
		LOG("RUN: shutdown -s -t " + seconds);
		startTimer(seconds);
		m_Clock->startTimerNow(seconds);
	}
	else {
		SetStatusText("WARNING: Invalid custom time!");
	}
}


// Execute onTick every 16ms (~60fps)
void MainFrame::onTick(wxTimerEvent& evt) {
	if (m_Clock->hasActiveTimer()) {
		if (m_Clock->hasRunningTimer() == true) {
			m_ClockStaticText->SetLabel(UNICODE_PLAY + m_Clock->toString(m_Clock->getRemainingTime()));
		}
		else if (m_Clock->hasPausedTimer() == true) {
			m_ClockStaticText->SetLabel(UNICODE_PAUSE + m_Clock->toString(m_Clock->getRemainingTime()));
		}
		else {
			m_ClockStaticText->SetLabel("Irjendwat is schief jelofen...");
		}
	}
	else { //m_Clock->hasActiveTimer() == false
		m_ClockStaticText->SetLabel(UNICODE_STOP + m_Clock->toString(0));
	}
}