#pragma once
#include <wx/wx.h>
#include <wx/frame.h>

#include <sstream>
#include <ctime>


struct Clock {
public:
	Clock() {
		m_EndTime = 0;
		m_RemainingTime = 300;
		m_HasActiveTimer = false;
		m_IsTimerPaused = false;
	}
	void startTimerNow(long long seconds) {
		m_EndTime = time(nullptr) + seconds;
		m_HasActiveTimer = true;
		m_IsTimerPaused = false;
	}
	void stopTimer() {
		m_HasActiveTimer = false;
		m_IsTimerPaused = false;
	}
	void pauseTimer() {
		m_RemainingTime = m_EndTime - time(nullptr);
		m_IsTimerPaused = true;
	}
	void continueTimer() {
		m_IsTimerPaused = false;
		m_EndTime = time(nullptr) + m_RemainingTime;
	}
	long long getRemainingTime() {
		if (m_HasActiveTimer)
			if (!m_IsTimerPaused)
				return m_EndTime - time(nullptr);
			else
				return m_RemainingTime;
		else
			return 300;
	}
	bool hasRunningTimer() {
		if (m_HasActiveTimer && !m_IsTimerPaused) {
			return true;
		}
		return false;
	}
	bool hasPausedTimer() {
		if (m_HasActiveTimer && m_IsTimerPaused) {
			return true;
		}
		return false;
	}
	bool hasActiveTimer() { return m_HasActiveTimer; }

	std::string toString(long long tm) {
		std::stringstream str;
		std::string h, m, s;
		(tm/3600 < 10)	? h = "0" + std::to_string(tm/3600)
						: h =		std::to_string(tm/3600);
		(tm/60%60 < 10) ? m = "0" + std::to_string(tm/60 % 60)
						: m =		std::to_string(tm/60 % 60);
		(tm%60 < 10)	? s = "0" + std::to_string(tm % 60)
						: s =		std::to_string(tm % 60);
		str << h << ':' 
			<< m << ':'
			<< s;
		return str.str();
	}
private:
	long long m_EndTime;
	long long m_RemainingTime;
	bool m_HasActiveTimer;
	bool m_IsTimerPaused;
};

class MainFrame : public wxFrame
{
public:
	MainFrame();

private:
	void startTimer(int seconds);
	void abortTimer();
	void playPauseTimer();
	void sendCustomTimer(wxString str);

	void onTick(wxTimerEvent& evt);
private:
	wxTimer m_wxTimer;

	Clock* m_Clock;

	wxStaticText* m_ClockStaticText;
	wxButton* m_PlayPauseButton;
};
