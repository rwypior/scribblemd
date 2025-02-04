#include "MarkdownEditorMainWindow.h"

#include <wx/wx.h>
#include <wx/cmdline.h>
#include <wx/stdpaths.h>

namespace
{
	const wxCmdLineEntryDesc g_cmdLineDesc[] =
	{
		{ wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },

		{ wxCMD_LINE_PARAM, "file", "file", "File to load",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_SWITCH, "edit", "edit", "Force editor when launched with parameter",
			wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_NONE }
	};
}

class ScribbleMd : public wxApp
{
public:
	ScribbleMd();

    virtual bool OnInit() override;
    void OnInitCmdLine(wxCmdLineParser& parser) override;
	bool OnCmdLineParsed(wxCmdLineParser& parser) override;

private:
	wxString initDocumentPath;
	bool forceEditor = false;
};

wxIMPLEMENT_APP(ScribbleMd);

ScribbleMd::ScribbleMd()
{
	SetVendorName("ScribbleMd");
	SetAppName("ScribbleMd");

	wxStandardPaths& sp = wxStandardPaths::Get();
	sp.UseAppInfo(wxStandardPaths::AppInfo_AppName);
}

bool ScribbleMd::OnInit()
{
	if (!wxApp::OnInit())
		return false;

    wxImage::AddHandler(new wxPNGHandler());

    MarkdownEditorMainWindow* window = new MarkdownEditorMainWindow(nullptr);
    window->Show();

	if (!this->initDocumentPath.empty())
	{
		window->open(this->initDocumentPath);

		if (!this->forceEditor)
			window->disableEditor();
	}

    return true;
}

void ScribbleMd::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc(g_cmdLineDesc);
    parser.SetSwitchChars(wxT("-"));
}

bool ScribbleMd::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (parser.GetParamCount() >= 1)
		this->initDocumentPath = parser.GetParam(0).ToStdString();

	this->forceEditor = parser.Found(wxT("edit"));

	return true;
}