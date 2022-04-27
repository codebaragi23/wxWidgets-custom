// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"


#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/mstream.h"
#include "wx/wfstream.h"
#include "wx/quantize.h"
#include "wx/stopwatch.h"

#if wxUSE_CLIPBOARD
#include "wx/dataobj.h"
#include "wx/clipbrd.h"
#endif // wxUSE_CLIPBOARD

#include "campanel.h"

#define TIMER_ID 1000

// #include <opencv2/opencv.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/videoio.hpp>

//-----------------------------------------------------------------------------
// wxCamPanel
//-----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(wxCamPanel, wxPanel)
EVT_PAINT(wxCamPanel::OnPaint)
EVT_TIMER(wxID_ANY, wxCamPanel::OnTimer)

//EVT_CHAR(wxCamPanel::OnChar)
wxEND_EVENT_TABLE()

wxCamPanel::wxCamPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
  : wxPanel(parent, id, pos, size, wxNO_BORDER)
  , timer(this, TIMER_ID)
{
  SetBackgroundColour(*wxLIGHT_GREY);

#if wxUSE_LIBPNG
  wxImage::AddHandler(new wxPNGHandler);
#endif

  wxImage image;
  if (!image.LoadFile("horse.png"))
    wxLogError("Can't load PNG image");
  else
    bmFrame = wxBitmap(image);
}

wxCamPanel::~wxCamPanel()
{
}

bool wxCamPanel::OpenCam(std::string camID)
{
  // cap.open(camID);
  // cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
  // cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  // cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  // cap.set(cv::CAP_PROP_FPS, 30);

  // if (cap.isOpened())
  //   timer.Start(1000 / 30);
}

void wxCamPanel::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC dc(this);
  PrepareDC(dc);
  Render(dc);
}

void wxCamPanel::Render(wxDC& dc)
{
  if (bmFrame.IsOk())
    dc.DrawBitmap(bmFrame, 0, 0, false);
}


void wxCamPanel::OnTimer(wxTimerEvent& event)
{
  // cv::Mat frame;
  // cap.read(frame);
  // cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

  // wxImage tmp = wxImage(frame.cols, frame.rows, frame.data, TRUE);
  // bmFrame = wxBitmap(tmp);

  // wxClientDC dc(this);
  // Render(dc);
}
