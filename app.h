#pragma once

// derived classes
class wxCamPanel;

// -- application --
class App : public wxApp
{
public:
  bool OnInit() wxOVERRIDE;
};

wxDECLARE_APP(App);

// -- Frame --
class Frame : public wxFrame
{
private:
  enum Menu
  {
    LIVE,
    REGIST,
    STATUS,
    SETTING,

    MENU_CNT
  };

public:
  Frame(wxWindow* parent, const wxWindowID id, const wxString& title,
    const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);

private:
  void OnChar(wxKeyEvent& event);
  void OnToolSelect(wxCommandEvent& event);
  wxDECLARE_EVENT_TABLE();
};
