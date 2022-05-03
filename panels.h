#pragma once

#include "wx/panel.h"
#include "wx/overlay.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

//-----------------------------------------------------------------------------
// wxCamPanel
//-----------------------------------------------------------------------------
class wxCamPanel : public wxPanel
{
private:
  cv::VideoCapture cap;
  wxTimer   timer;

  wxStaticBitmapBase* viewer;

public:
  wxCamPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER);

  void SetCap(cv::VideoCapture cap);

private:
  void OnTimer(wxTimerEvent& event);
  wxDECLARE_EVENT_TABLE();
};

//-----------------------------------------------------------------------------
// wxRegistPanel
//-----------------------------------------------------------------------------
class wxRegistPanel : public wxPanel
{
private:
  cv::VideoCapture cap;
  wxTimer   timer;

  wxStaticBitmapBase* viewer;
  wxTextCtrl* name;

public:
  wxRegistPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER);

  void SetCap(cv::VideoCapture cap);

private:
  void OnTimer(wxTimerEvent& event);
  wxDECLARE_EVENT_TABLE();
};


//-----------------------------------------------------------------------------
// wxStatusPanel
//-----------------------------------------------------------------------------
class wxStatusPanel : public wxPanel
{
private:
  wxGridSizer* szl = nullptr;
  wxBoxSizer* szr = nullptr;

public:
  wxStatusPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER);

private:
  void OnSize(wxSizeEvent& event);
  wxDECLARE_EVENT_TABLE();
};
