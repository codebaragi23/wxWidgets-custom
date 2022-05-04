// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/defs.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"
#include "wx/notebook.h"
#include "wx/generic/statbmpg.h"
#include "wx/statline.h"

#if wxUSE_GRAPHICS_CONTEXT
#include "wx/graphics.h"
#endif

#include "panels.h"

#define TIMER_ID1 1000
#define TIMER_ID2 1001

#include "wx/gtk/private.h"

#include "wx/aui/aui.h"

//-----------------------------------------------------------------------------
// wxStaticTextEx
//-----------------------------------------------------------------------------
class wxStaticTextEx : public wxStaticText
{
public:
  wxStaticTextEx(wxWindow* parent,
    wxWindowID id,
    const wxString& label,
    const wxPoint& pos = wxDefaultPosition,
    const wxSize& size = wxDefaultSize,
    long style = 0,
    const wxString& name = wxASCII_STR(wxStaticTextNameStr))
  {
    Create(parent, id, label, pos, size, style, name);

    float x_alignment = 0, y_alignment = 0;
    if (style & wxALIGN_CENTER_HORIZONTAL)
    {
      x_alignment = 0.5;
    }
    else if (style & wxALIGN_RIGHT)
    {
      x_alignment = 1.0;
    }

    if (style & wxALIGN_CENTER_VERTICAL)
    {
      y_alignment = 0.5;
    }

#ifdef __WXGTK4__
    g_object_set(m_widgetLabel, "xalign", x_alignment, "yalign", y_alignment, NULL);
#else
    gtk_misc_set_alignment((GtkMisc*)GTK_MISC(m_widget), (gfloat)x_alignment, (gfloat)y_alignment);
#endif
  }
};

//-----------------------------------------------------------------------------
// wxLiveViewer
//-----------------------------------------------------------------------------
class wxLiveViewer : public wxGenericStaticBitmap
{
private:
  ScaleMode m_scaleMode;

  wxRect rt;
  bool drawing_live;

public:
  wxLiveViewer()
    : wxGenericStaticBitmap()
    , drawing_live(true)
  {}
  wxLiveViewer(wxWindow* parent,
    wxWindowID id,
    const wxBitmapBundle& bitmap,
    const wxPoint& pos = wxDefaultPosition,
    const wxSize& size = wxDefaultSize,
    long style = 0,
    const wxString& name = wxASCII_STR(wxStaticBitmapNameStr))
    : wxGenericStaticBitmap(parent, id, bitmap, pos, size, style, name)
    , drawing_live(true)
  {
    Bind(wxEVT_PAINT, &wxLiveViewer::OnPaint, this);
  }

  virtual void SetBitmap(const wxBitmapBundle& bitmap) wxOVERRIDE
  {
    m_bitmapBundle = bitmap;
    Refresh();
  }
  virtual void SetScaleMode(ScaleMode scaleMode) wxOVERRIDE
  {
    m_scaleMode = scaleMode;
    Refresh();
  }

  virtual wxRect GetDrawningRect() { return rt; }

private:
  void OnPaint(wxPaintEvent& event);
  wxDECLARE_DYNAMIC_CLASS(wxLiveViewer);
};

wxIMPLEMENT_DYNAMIC_CLASS(wxLiveViewer, wxGenericStaticBitmap);
void wxLiveViewer::OnPaint(wxPaintEvent& event)
{
  if (!m_bitmapBundle.IsOk())
    return;

  wxPaintDC dc(this);
  const wxSize drawSize = GetClientSize();
  if (!drawSize.x || !drawSize.y)
    return;

  wxBitmap bitmap = GetBitmap();
  const wxSize bmpSize = bitmap.GetSize();
  wxDouble w = 0;
  wxDouble h = 0;
  switch (m_scaleMode)
  {
  case Scale_None:
    dc.DrawBitmap(bitmap, 0, 0, true);
    return;
  case Scale_Fill:
    w = drawSize.x;
    h = drawSize.y;
    break;
  case Scale_AspectFill:
  case Scale_AspectFit:
  {
    wxDouble scaleFactor;
    wxDouble scaleX = (wxDouble)drawSize.x / (wxDouble)bmpSize.x;
    wxDouble scaleY = (wxDouble)drawSize.y / (wxDouble)bmpSize.y;
    if ((m_scaleMode == Scale_AspectFit && scaleY < scaleX) ||
      (m_scaleMode == Scale_AspectFill && scaleY > scaleX))
      scaleFactor = scaleY;
    else
      scaleFactor = scaleX;

    w = bmpSize.x * scaleFactor;
    h = bmpSize.y * scaleFactor;

    break;
  }
  default:
    wxFAIL_MSG("Unknown scale mode");
  }

  wxDouble x = (drawSize.x - w) / 2;
  wxDouble y = (drawSize.y - h) / 2;
  rt = wxRect(x, y, w, h);

#if wxUSE_GRAPHICS_CONTEXT
  wxScopedPtr<wxGraphicsContext> const
    gc(wxGraphicsRenderer::GetDefaultRenderer()->CreateContext(dc));
  gc->DrawBitmap(bitmap, x, y, w, h);
#else
  wxBitmap::Rescale(bitmap, wxSize(wxRound(w), wxRound(h)));
  dc.DrawBitmap(bitmap, wxRound(x), wxRound(y), true);
#endif

  if (drawing_live)
  {
    int sz = FromDIP(24);
    dc.SetFont(wxFont(sz, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
    dc.SetTextForeground(wxColour(255, 0, 0));
    auto tsz = dc.GetTextExtent("LIVE");
    dc.DrawText("LIVE", x + w - tsz.GetWidth() - sz, y + sz);
    dc.SetPen(wxPen(wxColour(255, 0, 0)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(wxRect(x + w - tsz.GetWidth() - sz, y + sz, tsz.GetWidth(), tsz.GetHeight()));
  }
}


//-----------------------------------------------------------------------------
// wxCamPanel
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(wxCamPanel, wxPanel)
EVT_TIMER(TIMER_ID1, wxCamPanel::OnTimer)
wxEND_EVENT_TABLE()

wxCamPanel::wxCamPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
  : wxPanel(parent, id, pos, size, style)
  , timer(this, TIMER_ID1)
{
  auto sz = new wxBoxSizer(wxVERTICAL); SetSizer(sz);
  viewer = new wxLiveViewer(this, wxID_ANY, wxImage());
  viewer->SetBackgroundColour(wxColour(31, 31, 31));
  viewer->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
  sz->Add(viewer, wxSizerFlags(1).Expand());

  // auto st = new wxStaticText(this, wxID_ANY, "LIVE");
  // st->SetFont(wxFont(FromDIP(32), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
  // //st->SetBackgroundColour(wxColour(255, 0, 0));
  // st->SetForegroundColour(wxColour(255, 0, 0));
  // st->SetTransparent(255);
  // sz->Add(st, wxSizerFlags(0).Top().Right());
}

void wxCamPanel::SetCap(cv::VideoCapture cap)
{
  this->cap = cap;
  if (cap.isOpened())
    timer.Start(1000 / 30);
}

void wxCamPanel::OnTimer(wxTimerEvent& event)
{
  if (!IsShown()) return;
  cv::Mat frame;
  cap.read(frame);
  cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

  wxImage tmp = wxImage(frame.cols, frame.rows, frame.data, TRUE);
  viewer->SetBitmap(tmp);
}


//-----------------------------------------------------------------------------
// wxRegistPanel
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(wxRegistPanel, wxPanel)
EVT_BUTTON(CAPTURE, wxRegistPanel::OnButton)
EVT_BUTTON(REGIST, wxRegistPanel::OnButton)

EVT_TIMER(TIMER_ID2, wxRegistPanel::OnTimer)
wxEND_EVENT_TABLE()

wxRegistPanel::wxRegistPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
  : wxPanel(parent, id, pos, size, style)
  , timer(this, TIMER_ID2)
{
  SetBackgroundColour(wxColour(38, 38, 38));

  auto sz = new wxBoxSizer(wxVERTICAL);
  {
    int h;
    {
      wxClientDC dc(this);
      dc.GetTextExtent(wxString("A"), nullptr, &h);
    }
    auto st = new wxStaticTextEx(this, wxID_ANY, "  Face Register", wxDefaultPosition, wxSize(-1, h + FromDIP(20)), wxALIGN_CENTER_VERTICAL);
    st->SetBackgroundColour(wxColour(31, 31, 31));
    st->SetForegroundColour(wxColour(255, 255, 255));
    sz->Add(st, wxSizerFlags(0).Top().Align(wxALIGN_LEFT).Expand());
  }

  auto szm = new wxBoxSizer(wxHORIZONTAL);
  auto szl = new wxBoxSizer(wxVERTICAL);
  {
    auto camsel = new wxChoice(this, wxID_ANY);
    camsel->AppendString("Cam1");
    camsel->Select(0);
    //camsel->AppendString("Cam2");
    szl->Add(camsel, wxSizerFlags(0).Top().Left());

    viewer = new wxLiveViewer(this, wxID_ANY, wxImage());
    viewer->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
    szl->Add(viewer, wxSizerFlags(1).Expand());
    szl->Add(new wxButton(this, CAPTURE, "Capture"), wxSizerFlags(0).Left());
  }

  auto szr = new wxBoxSizer(wxVERTICAL);
  {
    auto nb = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(300, 340)), wxAUI_NB_CLOSE_ON_ACTIVE_TAB);
    nb->SetBackgroundColour(wxColour(38, 38, 38));
    nb->SetForegroundColour(wxColour(255, 255, 255));
    nb->SetArtProvider(new wxAuiSimpleTabArt);
    faces = nb;

    {
      auto sb = new wxGenericStaticBitmap(faces, wxID_ANY, wxImage("0.jpg"));
      sb->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
      faces->AddPage(sb, wxString::Format("Face%d", (int)faces->GetPageCount() + 1), true);
    }
    {
      auto sb = new wxGenericStaticBitmap(faces, wxID_ANY, wxImage("0.jpg"));
      sb->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
      faces->AddPage(sb, wxString::Format("Face%d", (int)faces->GetPageCount() + 1), true);
    }
    szr->Add(faces, wxSizerFlags(0).Center());

    auto fgs = new wxFlexGridSizer(2);
    fgs->AddGrowableCol(1);
    int h = -1;
    {
      auto tc = new wxTextCtrl(this, wxID_ANY);
      tc->GetSize(nullptr, &h);
      tc->Destroy();
    }
    fgs->Add(new wxStaticTextEx(this, wxID_STATIC, "Name: ", wxDefaultPosition, wxSize(-1, h), wxALIGN_CENTER_VERTICAL), wxSizerFlags(0).Left());
    fgs->Add(name = new wxTextCtrl(this, wxID_ANY), wxSizerFlags(1).Expand());
    szr->Add(fgs, wxSizerFlags(1).Expand().Border(wxTOP, FromDIP(20)));

    szr->Add(new wxButton(this, REGIST, "Register"), wxSizerFlags(0));
  }

  szm->Add(szl, wxSizerFlags(1).Expand());
  szm->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(3, 3), wxLI_VERTICAL), wxSizerFlags(0).Align(wxGROW | wxTOP | wxBOTTOM).Border(wxLEFT | wxRIGHT, FromDIP(20)));
  szm->Add(szr, wxSizerFlags(0).Align(wxGROW | wxTop | wxBOTTOM));
  sz->Add(szm, wxSizerFlags(1).Expand().Border(wxALL, FromDIP(20)));
  SetSizer(sz);

  // wxWindow* win = nullptr;
  // while(win = FindWindow(wxID_STATIC))
  //   win->SetForegroundColour(wxColour(255, 255, 255));
}

void wxRegistPanel::SetCap(cv::VideoCapture cap)
{
  this->cap = cap;
  if (cap.isOpened())
    timer.Start(1000 / 30);
}

void wxRegistPanel::OnTimer(wxTimerEvent& event)
{
  if (!IsShown()) return;
  cv::Mat frame;
  cap.read(frame);
  cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

  wxImage tmp = wxImage(frame.cols, frame.rows, frame.data, TRUE);
  viewer->SetBitmap(tmp);
}

void wxRegistPanel::OnButton(wxCommandEvent& event)
{
  switch (event.GetId())
  {
  case CAPTURE:
  {
    wxImage bmp = viewer->GetBitmap().ConvertToImage();
    auto sb = new wxGenericStaticBitmap(faces, wxID_ANY, bmp);
    sb->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
    faces->AddPage(sb, wxString::Format("Face%d", (int)faces->GetPageCount() + 1), true);
  }
  break;
  case REGIST:
  {
    wxImage bmp = viewer->GetBitmap().ConvertToImage();
    auto sb = new wxGenericStaticBitmap(faces, wxID_ANY, bmp);
    sb->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
    faces->AddPage(sb, wxString::Format("Face%d", (int)faces->GetPageCount() + 1), true);
  }
  break;

  default:
    break;
  }
}



//-----------------------------------------------------------------------------
// wxStatusPanel
//-----------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(wxStatusPanel, wxPanel)
EVT_SIZE(wxStatusPanel::OnSize)

EVT_BUTTON(EDIT, wxStatusPanel::OnButton)
EVT_BUTTON(DELETE, wxStatusPanel::OnButton)
wxEND_EVENT_TABLE()

wxStatusPanel::wxStatusPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
  : wxPanel(parent, id, pos, size, style)
{
  SetBackgroundColour(wxColour(38, 38, 38));

  auto sz = new wxBoxSizer(wxVERTICAL);
  {
    int h;
    {
      wxClientDC dc(this);
      dc.GetTextExtent(wxString("A"), nullptr, &h);
    }
    auto st = new wxStaticTextEx(this, wxID_ANY, "  Registration Status", wxDefaultPosition, wxSize(-1, h + FromDIP(20)), wxALIGN_CENTER_VERTICAL);
    st->SetBackgroundColour(wxColour(31, 31, 31));
    st->SetForegroundColour(wxColour(255, 255, 255));
    sz->Add(st, wxSizerFlags(0).Top().Left().Expand());
  }

  auto szm = new wxBoxSizer(wxHORIZONTAL);
  szl = new wxGridSizer(4);
  {
    for (int c = 0; c < 6; c++)
    {
      auto cell = new wxBoxSizer(wxVERTICAL);
      auto sb = new wxGenericStaticBitmap(this, wxID_ANY, wxImage("0.jpg"));
      sb->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
      sb->SetBackgroundColour(wxColour(38, 38, 38));
      cell->Add(sb, wxSizerFlags(1).Expand());
      cell->Add(new wxStaticText(this, wxID_ANY, "Name: xx"), wxSizerFlags(0).Border(wxTOP | wxBOTTOM, FromDIP(4)));
      auto btn = new wxButton(this, STATUS_DETAIL + c, "Detail", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
      btn->Bind(wxEVT_BUTTON, &wxStatusPanel::OnDetail, this);
      cell->Add(btn);
      szl->Add(cell, wxSizerFlags(0).Top().Border(wxBOTTOM, FromDIP(20)));
    }
  }
  szr = new wxBoxSizer(wxVERTICAL);
  {
    auto nb = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(300, 340)), 0);
    nb->SetBackgroundColour(wxColour(38, 38, 38));
    nb->SetForegroundColour(wxColour(255, 255, 255));
    nb->SetArtProvider(new wxAuiSimpleTabArt);
    faces = nb;

    {
      auto sb = new wxGenericStaticBitmap(faces, wxID_ANY, wxImage("0.jpg"));
      sb->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
      faces->AddPage(sb, wxString::Format("Face%d", (int)faces->GetPageCount() + 1), true);
    }
    {
      auto sb = new wxGenericStaticBitmap(faces, wxID_ANY, wxImage("0.jpg"));
      sb->SetScaleMode(wxStaticBitmapBase::Scale_AspectFit);
      faces->AddPage(sb, wxString::Format("Face%d", (int)faces->GetPageCount() + 1), true);
    }
    szr->Add(faces, wxSizerFlags(0).Expand());

    auto fgs = new wxFlexGridSizer(2);
    fgs->AddGrowableCol(1);

    int h = -1;
    {
      auto tc = new wxTextCtrl(this, wxID_ANY);
      tc->GetSize(nullptr, &h);
      tc->Destroy();
    }
    fgs->Add(new wxStaticTextEx(this, wxID_STATIC, "Name: ", wxDefaultPosition, wxSize(-1, h), wxALIGN_CENTER_VERTICAL), wxSizerFlags(0).Left());
    fgs->Add(new wxTextCtrl(this, NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY), wxSizerFlags(1).Expand());

    fgs->Add(new wxStaticTextEx(this, wxID_STATIC, "Regist date: ", wxDefaultPosition, wxSize(-1, h), wxALIGN_CENTER_VERTICAL), wxSizerFlags(0).Left());
    fgs->Add(new wxStaticTextEx(this, REGIST_DATE, "2022.06.01", wxDefaultPosition, wxSize(-1, h), wxALIGN_CENTER_VERTICAL), wxSizerFlags(1).CenterVertical().Expand());

    fgs->Add(new wxStaticTextEx(this, wxID_STATIC, "Last Approved date: ", wxDefaultPosition, wxSize(-1, h), wxALIGN_CENTER_VERTICAL), wxSizerFlags(0).Left());
    fgs->Add(new wxStaticTextEx(this, APPROV_DATE, "-1", wxDefaultPosition, wxSize(-1, h), wxALIGN_CENTER_VERTICAL), wxSizerFlags(1).CenterVertical().Expand());
    szr->Add(fgs, wxSizerFlags(1).Expand().Border(wxTOP, FromDIP(20)));

    auto szh = new wxBoxSizer(wxHORIZONTAL);
    szh->Add(new wxButton(this, EDIT, "Edit"), wxSizerFlags(0).Border(wxRIGHT, FromDIP(10)));
    szh->Add(new wxButton(this, DELETE, "Delete"), wxSizerFlags(0).Border(wxRIGHT, FromDIP(10)));
    szr->Add(szh);
  }

  szm->Add(szl, wxSizerFlags(1));
  szm->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(3, 3), wxLI_VERTICAL), wxSizerFlags(0).Align(wxGROW | wxTOP | wxBOTTOM).Border(wxLEFT | wxRIGHT, FromDIP(20)));
  szm->Add(szr, wxSizerFlags(0).Align(wxGROW | wxTop | wxBOTTOM));
  sz->Add(szm, wxSizerFlags(1).Expand().Border(wxALL, FromDIP(20)));
  SetSizer(sz);
}

void wxStatusPanel::OnSize(wxSizeEvent& event)
{
  event.Skip();
  if (!szl || !szr) return;
  int w = GetSize().GetWidth();
  int r = szr->GetMinSize().GetWidth();
  int border = FromDIP(20) * 2 + 3;
  int icon = 112;
  int spacing = FromDIP(20);
  int cols = (w - r - border) / (icon + spacing);
  if (cols > 0) szl->SetCols(cols);
}

void wxStatusPanel::OnDetail(wxCommandEvent& event)
{
  wxPrintf("Clicked on tool %d\n", event.GetId());
}

void wxStatusPanel::OnButton(wxCommandEvent& event)
{
  switch (event.GetId())
  {
  case EDIT:
  {
    auto tc = ((wxTextCtrl*)FindWindow(NAME));
    tc->SetEditable(!tc->IsEditable());
  }
  break;
  case DELETE:
  {

  }
  break;
  default:
    break;
  }
}