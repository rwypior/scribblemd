#include "MarkdownEditorMainWindow.h"

#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler());

    MarkdownEditorMainWindow* window = new MarkdownEditorMainWindow(nullptr);
    window->Show();

    return true;
}
