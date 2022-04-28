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

#define wxUSE_GENERIC_DRAGIMAGE 0

#include "app.h"
#include "campanel.h"

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "sample.xpm"
#endif

// main program
wxIMPLEMENT_APP(App);

bool App::OnInit()
{
  if (!wxApp::OnInit())
    return false;

#if wxUSE_LIBPNG
  wxImage::AddHandler(new wxPNGHandler);
#endif

  (new Frame())->Show();
  return true;
}

// Frame
wxIMPLEMENT_DYNAMIC_CLASS(Frame, wxFrame);

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
EVT_CHAR_HOOK(Frame::OnChar)
EVT_MENU(wxID_ANY, Frame::OnToolLeftClick)
wxEND_EVENT_TABLE()

Frame::Frame()
  : wxFrame((wxFrame*)NULL, wxID_ANY, "App")
{
  // set frame icon
  SetIcon(wxICON(sample));

  SetToolBar(NULL);
  // #if wxUSE_STATUSBAR
  //   CreateStatusBar(2);
  //   int widths[] = { -1, 100 };
  //   SetStatusWidths(2, widths);
  // #endif // wxUSE_STATUSBAR

  wxToolBar* toolBar = CreateToolBar(wxTB_FLAT | wxTB_DOCKABLE | wxTB_VERTICAL, wxID_ANY);
  toolBar->SetToolBitmapSize(wxSize(32, 32));
  toolBar->SetBackgroundColour(wxColour(50, 50, 50));
  toolBar->SetMargins(16, 16);
  toolBar->AddRadioTool(LIVE, "Live", wxImage("bitmaps/play.png"));
  toolBar->AddRadioTool(REGIST, "Regist", wxImage("bitmaps/regist.png"));
  toolBar->AddRadioTool(STATUS, "Status", wxImage("bitmaps/status.png"));
  toolBar->AddRadioTool(SETTING, "Setting", wxImage("bitmaps/setting.png"));
  {
    wxToolBarToolBase* const tool = toolBar->CreateSeparator();
    tool->MakeStretchable();
    toolBar->InsertTool(toolBar->GetToolsCount() - 1, tool);
  }
  toolBar->Realize();

  //toolBar->ToggleTool(SETTING, true);

  //toolBar->ToggleWindowStyle();
  //toolBar->AddStretchableSpace();

  camPanel = new wxCamPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(640, 480));
  camPanel->OpenCam("/dev/video0");
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
}

void Frame::OnToolLeftClick(wxCommandEvent& event)
{
  printf("Clicked on tool %d\n", event.GetId());
  switch (event.GetId())
  {
  case LIVE:
    /* code */
    break;

  default:
    break;
  }
}