#pragma once

// derived classes
class Frame;
class wxCamPanel;

// -- application --

class App : public wxApp
{
public:
  bool OnInit() wxOVERRIDE;
};

wxDECLARE_APP(App);

// Frame

class Frame : public wxFrame
{
public:
  Frame();

protected:
  void OnChar(wxKeyEvent& event);

private:
  wxAuiToolBar* toolbar;
  wxCamPanel* campanel;

private:
  wxDECLARE_DYNAMIC_CLASS(Frame);
  wxDECLARE_EVENT_TABLE();
};
