#include "globals.h"
#include <fmt/format.h>
#include <wx/listctrl.h>

void launch_process(const std::string& command, int flags) {
#if defined __APPLE__ || defined __linux__
    //the '&' runs the command nonblocking, and >/dev/null 2>&1 destroys output
    auto fullcmd = fmt::format("{}{} &", command,null_device);
    FILE* stream = popen(fullcmd.c_str(), "r");
    pclose(stream);

#elif _WIN32
    //call wxExecute with the Async flag
    wxExecute(wxString(command),flags);

#endif
}

void reveal_in_explorer(const std::string& path) {
#if defined __APPLE__
	std::string command = "open \"" + path + "\"";

#elif defined __linux__
	std::string command = "xdg-open \"" + path + "\"";

#elif defined _WIN32
	//do not surround the paths in quotes, it will not work
	std::string command = "\\Windows\\explorer.exe \"" + path + "\"";
#endif
	launch_process(command);
}

long wxListCtrl_get_selected(wxListCtrl* listCtrl) {
	long itemIndex = -1;
	while ((itemIndex = listCtrl->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
		break;
	}
	return itemIndex;
}

void fitWindowMinSize(wxWindow* window) {
	//fit size to children
	window->Fit();

	//constrain minimum size to the minimum fitting size
	wxSize size = window->GetSize();
	
	window->SetSizeHints(size);
}

/**
@returns the calculated display scale factor using GDI+
*/
float get_WIN_dpi_multiple() {
	return 1;
	/*
	FLOAT dpiX;
	HDC screen = GetDC(0);
	dpiX = static_cast<FLOAT>(GetDeviceCaps(screen, LOGPIXELSX));
	ReleaseDC(0, screen);
	return dpiX / 96;*/
}

void dpi_scale(wxWindow* window){
	//fit size to children
	window->Fit();

	//calculate the scaled min size
	float fac = get_WIN_dpi_multiple();
	float minh = window->GetMinHeight() * fac;
	float minw = window->GetMinWidth() * fac;
	//set the minimum size
	window->SetSizeHints(wxSize(minw, minh));
}