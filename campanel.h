// #include <opencv2/opencv.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/videoio.hpp>

//-----------------------------------------------------------------------------
// wxCamPanel
//-----------------------------------------------------------------------------
class wxCamPanel : public wxPanel
{
public:
  wxCamPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
  ~wxCamPanel();

  bool OpenCam(std::string camID);

protected:
  void OnPaint(wxPaintEvent& event);
  void OnTimer(wxTimerEvent& event);

  void Render(wxDC& dc);

private:
  //cv::VideoCapture cap;

  wxTimer   timer;
  wxBitmap  bmFrame;

  wxDECLARE_EVENT_TABLE();
};
