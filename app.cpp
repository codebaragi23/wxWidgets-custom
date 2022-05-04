/////////////////////////////////////////////////////////////////////////////
// Name:        dragimag.cpp
// Purpose:     wxDragImage sample
// Author:      Julian Smart
// Modified by:
// Created:     28/2/2000
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"


#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

//#include "wx/image.h"

// Under Windows, change this to 1
// to use wxGenericDragImage

#include "app.h"
#include "panels.h"

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "sample.xpm"
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

// main program
wxIMPLEMENT_APP(App);

bool App::OnInit()
{
  if (!wxApp::OnInit())
    return false;

  wxImage::AddHandler(new wxPNGHandler);
  wxImage::AddHandler(new wxJPEGHandler);

  auto frame = new Frame(NULL, wxID_ANY, "wxWidgets-custom", wxDefaultPosition, wxSize(1920 / 2, 2160 / 2));
  frame->Show();
  return true;
}

// Frame
wxBEGIN_EVENT_TABLE(Frame, wxFrame)
EVT_CHAR_HOOK(Frame::OnChar)
EVT_MENU(wxID_ANY, Frame::OnToolSelect)
wxEND_EVENT_TABLE()

Frame::Frame(wxWindow* parent, const wxWindowID id, const wxString& title,
  const wxPoint& pos, const wxSize& size, const long style)
  : wxFrame(parent, id, title, pos, size, style)
{
  // set frame icon
  SetIcon(wxICON(sample));

  // #if wxUSE_STATUSBAR
  //   CreateStatusBar(2);
  //   int widths[] = { -1, 100 };
  //   SetStatusWidths(2, widths);
  // #endif // wxUSE_STATUSBAR

  auto toolBar = CreateToolBar(wxTB_FLAT | wxTB_NODIVIDER | wxTB_VERTICAL/* | wxTB_TEXT*/, wxID_ANY);
  toolBar->SetBackgroundColour(wxColour(50, 50, 50));
  toolBar->AddRadioTool(LIVE, "Live", wxImage("bitmaps/live_24.png"));
  toolBar->AddRadioTool(REGIST, "Regist", wxImage("bitmaps/regist_24.png"));
  toolBar->AddRadioTool(STATUS, "Status", wxImage("bitmaps/status_24.png"));
  toolBar->AddRadioTool(SETTING, "Setting", wxImage("bitmaps/setting_24.png"));
  {
    auto tool = toolBar->CreateSeparator();  tool->MakeStretchable();
    toolBar->InsertTool(toolBar->GetToolsCount() - 1, tool);
  }
  toolBar->Realize();

  auto sz = new wxBoxSizer(wxVERTICAL); SetSizer(sz);

  cv::VideoCapture cap("/dev/video0");
  cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  cap.set(cv::CAP_PROP_FPS, 30);
  {
    // auto test = new wxPanel(this, wxID_ANY);
    // test->SetBackgroundColour(wxColour(50, 50, 50));
    //new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    auto panel = new wxCamPanel(this, wxID_ANY);
    panel->SetCap(cap);
    sz->Add(panel, wxSizerFlags(1).Expand());
  }

  {
    auto panel = new wxRegistPanel(this, wxID_ANY);
    panel->SetCap(cap);
    sz->Add(panel, wxSizerFlags(1).Expand());
  }
  {
    auto panel = new wxStatusPanel(this, wxID_ANY);
    sz->Add(panel, wxSizerFlags(1).Expand());
  }
  {
    auto panel = new wxPanel(this, wxID_ANY);
    sz->Add(panel, wxSizerFlags(1).Expand());
  }
  toolBar->OnLeftClick(1, true);
}


void Frame::OnChar(wxKeyEvent& event)
{
  switch (event.GetKeyCode())
  {
  case WXK_TAB:
    GetToolBar()->Show(!GetToolBar()->IsShown());
    break;

  case WXK_ESCAPE:
    Close(true);
    break;

  default:
    break;
  }

  event.Skip();
}

void Frame::OnToolSelect(wxCommandEvent& event)
{
  wxPrintf("Clicked on tool %d\n", event.GetId());

  int toolid = event.GetId();
  GetToolBar()->ToggleTool(toolid, true);

  auto sz = GetSizer();
  sz->ShowItems(false);
  sz->Show(toolid, true);
  sz->Layout();

  //GetToolBar()->SetToolNormalBitmap(event.GetId(), wxImage(""));
}