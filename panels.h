#pragma once

#include "wx/panel.h"
#include "wx/overlay.h"
#include "wx/notebook.h"

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
  enum
  {
    CAPTURE = 0x100,
    REGIST,
  } wxIDS;

  cv::VideoCapture cap;
  wxTimer   timer;

  wxStaticBitmapBase* viewer;
  wxBookCtrlBase* faces;
  wxTextCtrl* name;

public:
  wxRegistPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER);

  void SetCap(cv::VideoCapture cap);

private:
  void OnTimer(wxTimerEvent& event);
  void OnButton(wxCommandEvent& event);
  wxDECLARE_EVENT_TABLE();
};


//-----------------------------------------------------------------------------
// wxStatusPanel
//-----------------------------------------------------------------------------
class wxStatusPanel : public wxPanel
{
private:
  enum
  {
    EDIT = 0x100,
    DELETE,
    NAME = 0x150,
    REGIST_DATE,
    APPROV_DATE,
    STATUS_DETAIL = 0x200,
  } wxIDS;

  wxGridSizer* szl = nullptr;
  wxBoxSizer* szr = nullptr;
  wxBookCtrlBase* faces;
  int curSelID = 0;

public:
  wxStatusPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER);

private:
  void OnSize(wxSizeEvent& event);
  void OnDetail(wxCommandEvent& event);
  void OnButton(wxCommandEvent& event);
  wxDECLARE_EVENT_TABLE();
};
