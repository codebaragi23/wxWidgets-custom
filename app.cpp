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

#include "wx/aui/aui.h"
#include "wx/artprov.h"
#include "wx/splitter.h"

// Under Windows, change this to 1
// to use wxGenericDragImage

#define wxUSE_GENERIC_DRAGIMAGE 0

#include "app.h"
#include "campanel.h"

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
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

  (new Frame())->Show();
  return true;
}

#define ID_SampleItem 100

// Frame
wxIMPLEMENT_DYNAMIC_CLASS(Frame, wxFrame);

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
EVT_CHAR_HOOK(Frame::OnChar)
wxEND_EVENT_TABLE()

Frame::Frame()
  : wxFrame((wxFrame*)NULL, wxID_ANY, "wxDragImage sample")
{
  SetIcon(wxICON(sample));

#if wxUSE_STATUSBAR
  CreateStatusBar(2);
  int widths[] = { -1, 100 };
  SetStatusWidths(2, widths);
#endif // wxUSE_STATUSBAR

  auto splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);

  toolbar = new wxAuiToolBar(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL | wxSUNKEN_BORDER);
  toolbar->AddTool(ID_SampleItem + 30, "Test", wxArtProvider::GetBitmapBundle(wxART_ERROR));
  toolbar->AddSeparator();
  toolbar->AddTool(ID_SampleItem + 31, "Test", wxArtProvider::GetBitmapBundle(wxART_QUESTION));
  toolbar->AddTool(ID_SampleItem + 32, "Test", wxArtProvider::GetBitmapBundle(wxART_INFORMATION));
  toolbar->AddTool(ID_SampleItem + 33, "Test", wxArtProvider::GetBitmapBundle(wxART_WARNING));
  toolbar->AddTool(ID_SampleItem + 34, "Test", wxArtProvider::GetBitmapBundle(wxART_MISSING_IMAGE));

  campanel = new wxCamPanel(splitter, wxID_ANY, wxPoint(0, 0), wxSize(640, 480));
  campanel->OpenCam("/dev/video0");

  splitter->SplitVertically(toolbar, campanel);
  splitter->SetSashInvisible();
  toolbar->Realize();
}


void Frame::OnChar(wxKeyEvent& event)
{
  switch (event.GetKeyCode())
  {
  case WXK_TAB:
    toolbar->Show(!toolbar->IsShownOnScreen());
    break;

  case WXK_ESCAPE:
    Close(true);
    break;

  default:
    break;
  }
}