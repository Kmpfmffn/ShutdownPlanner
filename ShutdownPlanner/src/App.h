#pragma once
#include <wx/wx.h>
#include <wx/frame.h>


#include "MainFrame.h"


class App : public wxApp
{
public:
	bool OnInit() override;

	App();
	~App();
};