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
public:
  Frame();

protected:
  void OnChar(wxKeyEvent& event);
  void OnToolLeftClick(wxCommandEvent& event);

private:
  enum Menu
  {
    LIVE,
    REGIST,
    STATUS,
    SETTING,
  };

  wxCamPanel* camPanel;

private:
  wxDECLARE_DYNAMIC_CLASS(Frame);
  wxDECLARE_EVENT_TABLE();
};
