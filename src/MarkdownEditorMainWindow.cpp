#include "MarkdownEditorMainWindow.h"

#include <cppmarkdown/extensions.h>

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/aboutdlg.h>

#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>

struct GeometrySettings : public wxTopLevelWindow::GeometrySerializer
{
public:
	GeometrySettings()
	{
	}

	bool SaveField(const wxString& name, int value) const override
	{
		this->props[name] = value;
		return true;
	}

	bool RestoreField(const wxString& name, int* value) override
	{
		auto it = props.find(name.ToStdString());
		if (it == props.end())
			return false;

		*value = it->second;

		return true;
	}

	static void writeGeometry(const GeometrySettings& settings)
	{
		wxFileName path(wxStandardPaths::Get().GetUserLocalDataDir(), "geometry");
		path.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		auto name = path.GetName();
		auto ppp = path.GetPath();
		auto asd = path.GetFullPath();

		std::ofstream ofs(asd.ToStdString());
		if (ofs.is_open())
		{
			for (auto pair : settings.props)
			{
				ofs << pair.second << "\n";
			}
		}
	}

	static GeometrySettings readGeometry(GeometrySettings defaults)
	{
		wxFileName path(wxStandardPaths::Get().GetUserLocalDataDir(), "geometry");
		auto asd = path.GetFullPath();

		std::ifstream ifs(asd.ToStdString());
		if (ifs.is_open())
		{
			for (auto& pair : defaults.props)
			{
				std::string line;
				if (std::getline(ifs, line))
					pair.second = std::stoi(line);
				else
					break;
			}
		}

		return defaults;
	}

private:
	mutable std::map<wxString, int> props;
};

MarkdownEditorMainWindow::MarkdownEditorMainWindow( wxWindow* parent )
	: MainWindow( parent )
	, timer(this, wxID_ANY)
{
	Markdown::registerStandardExtensions();

	this->Bind(wxEVT_MENU, &MarkdownEditorMainWindow::evtCommand, this);

	this->Bind(wxEVT_CLOSE_WINDOW, &MarkdownEditorMainWindow::handlerClose, this);

	this->m_toolBar1->Bind(wxEVT_TOOL, &MarkdownEditorMainWindow::evtCommand, this);

	auto resDir = wxFileName(wxStandardPaths::Get().GetExecutablePath());
	resDir.AppendDir("res");
	
	auto iconPath = wxFileName(resDir.GetPath(), "markdown.ico").GetFullPath();

	this->SetIcon(wxIcon(iconPath, wxBitmapType::wxBITMAP_TYPE_ICO));
	
	this->m_editor->Bind(wxEVT_TEXT, &MarkdownEditorMainWindow::handlerTextChanged, this);
	this->Bind(wxEVT_TIMER, &MarkdownEditorMainWindow::handlerTimerMarkdown, this, this->timer.GetId());

	this->m_splitter1->Bind(wxEVT_SPLITTER_DOUBLECLICKED, &MarkdownEditorMainWindow::handlerSplitterDoubleclick, this);

	this->enableEditor();

	GeometrySettings geometry;
	this->SaveGeometry(geometry);
	geometry = GeometrySettings::readGeometry(geometry);
	this->RestoreToGeometry(geometry);

	this->m_viewer->insertImageWidth();
}

// Utils

bool MarkdownEditorMainWindow::isCursorAtLineStart() const
{
	auto insertionPoint = this->m_editor->GetInsertionPoint();
	long x;
	long y;
	this->m_editor->PositionToXY(insertionPoint, &x, &y);
	return x == 0;
}

bool MarkdownEditorMainWindow::isCursorAtEnd() const
{
	auto insertionPoint = this->m_editor->GetInsertionPoint();
	this->m_editor->SetInsertionPointEnd();
	auto length = this->m_editor->GetInsertionPoint();
	this->m_editor->SetInsertionPoint(insertionPoint);
	long ix, iy;
	long x, y;
	this->m_editor->PositionToXY(insertionPoint, &ix, &iy);
	this->m_editor->PositionToXY(length, &x, &y);
	return ix == x && iy == y;
}

void MarkdownEditorMainWindow::insertAndSelectText(const wxString& text, size_t begin, int length)
{
	auto insertionPoint = this->m_editor->GetInsertionPoint();

	if (length < 0)
		length = text.length() - std::abs(length) - begin;
	
	if (length > text.length())
		length = text.length();

	this->m_editor->WriteText(text);
	this->m_editor->SetSelection(insertionPoint + begin, insertionPoint + begin + length);
}

void MarkdownEditorMainWindow::insertAndJumpToNextLine(const wxString& text)
{
	bool atEnd = this->isCursorAtEnd();
	this->m_editor->WriteText(text);
	if (atEnd)
		this->m_editor->WriteText("\n");

	auto insertionPoint = this->m_editor->GetInsertionPoint();
	long x, y;
	this->m_editor->PositionToXY(insertionPoint, &x, &y);
	y += 1;
	x = this->m_editor->GetLineText(y).length();
	insertionPoint = this->m_editor->XYToPosition(x, y);

	this->m_editor->SetInsertionPoint(insertionPoint);
}

void MarkdownEditorMainWindow::insertNewLineIfNeeded()
{
	if (!this->isCursorAtLineStart())
		this->m_editor->WriteText("\n");
}

void MarkdownEditorMainWindow::enableEditor()
{
	this->edit->FindItem(ID_ENABLE_EDITOR)->Check(true);
	this->m_toolBar1->ToggleTool(ID_ENABLE_EDITOR, true);

	this->m_splitter1->SplitVertically(m_panel1, m_panel2);
}

void MarkdownEditorMainWindow::disableEditor()
{
	this->edit->FindItem(ID_ENABLE_EDITOR)->Check(false);
	this->m_toolBar1->ToggleTool(ID_ENABLE_EDITOR, false);
	
	this->m_splitter1->Unsplit(m_panel1);
}

void MarkdownEditorMainWindow::toggleEditor()
{
	if (m_panel1->IsShown())
		this->disableEditor();
	else
		this->enableEditor();
}

bool MarkdownEditorMainWindow::promptDocumentClose()
{
	if (this->textChanged)
	{
		int result = wxMessageBox("Do you want to save?", "Markdown editor", wxICON_QUESTION | wxYES_NO | wxCANCEL, this);
		switch (result)
		{
		case wxYES:
			this->promptSave();
			return true;
		case wxNO:
			return true;
		case wxCANCEL:
			return false;
		}
	}

	return true;
}

bool MarkdownEditorMainWindow::promptSave()
{
	if (this->currentFile.GetFullPath().empty())
		return this->promptSaveAs();
	else
	{
		this->save(this->currentFile.GetFullPath());
		return true;
	}
}

bool MarkdownEditorMainWindow::promptSaveAs()
{
	wxFileDialog saveDialog(this, "Markdown editor - save document", wxEmptyString, wxEmptyString, "Markdown files (*.md)|*.md|HyperText Markup Language (*.html)|*.html", wxFD_SAVE);
	int result = saveDialog.ShowModal();

	if (result == wxID_CANCEL)
		return false;

	this->save(saveDialog.GetPath());
	
	return true;
}

bool MarkdownEditorMainWindow::promptOpen(wxString& outpath)
{
	wxFileDialog openDialog(this, "Markdown editor - open document", wxEmptyString, wxEmptyString, "Markdown files (*.md)|*.md", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	int result = openDialog.ShowModal();

	if (result == wxID_CANCEL)
		return false;

	outpath = openDialog.GetPath();

	return true;
}

bool MarkdownEditorMainWindow::promptOpen()
{
	wxString path;
	bool result = this->promptOpen(path);

	if (!result)
		return false;

	this->open(path);

	return true;
}

void MarkdownEditorMainWindow::newDocument()
{
	this->m_editor->Clear();
	this->setCurrentFilePath("");
	this->textChanged = false;
}

void MarkdownEditorMainWindow::save(const wxString& path)
{
	std::ofstream fs(path.ToStdString());
	if (!fs.is_open())
	{
		wxMessageBox("Cannot open file", "Markdown editor", wxICON_ERROR | wxOK);
		return;
	}

	this->setCurrentFilePath(path);
	wxFileName filename(path);
	auto ext = filename.GetExt();

	if (ext == "md")
		fs << this->m_editor->GetValue().ToStdString();
	else if (ext == "html")
		fs << this->m_viewer->getMarkdownDocument().getHtml();
}

void MarkdownEditorMainWindow::open(const wxString& path)
{
	std::ifstream fs(path.ToStdString());
	if (!fs.is_open())
	{
		wxMessageBox("Cannot open file", "Markdown editor", wxICON_ERROR | wxOK);
		return;
	}

	std::stringstream buf;
	buf << fs.rdbuf();

	this->m_editor->SetValue(buf.str());

	this->setCurrentFilePath(path);
	this->textChanged = false;
}

void MarkdownEditorMainWindow::exportAs()
{
	wxFileDialog exportDialog(this, "Markdown editor - export", wxEmptyString, wxEmptyString, "HTML files (*.html;*.htm)|*.html;*.htm", wxFD_SAVE);
	int result = exportDialog.ShowModal();

	if (result == wxID_CANCEL)
		return;

	wxFileName path = exportDialog.GetPath();

	if (path.GetExt() == "html" || path.GetExt() == "htm")
		this->exportAsHtml(path.GetFullPath());
}

void MarkdownEditorMainWindow::exportAsHtml(const wxString& path)
{
	std::ofstream fs(path.ToStdString());
	if (!fs.is_open())
	{
		wxMessageBox("Cannot open file", "Markdown editor", wxICON_ERROR | wxOK);
		return;
	}

	fs << this->m_viewer->getMarkdownDocument().getHtml();
}

void MarkdownEditorMainWindow::setCurrentFilePath(const wxString& path)
{
	this->currentFile = path;

	if (path.empty())
		this->SetTitle("Markdown editor");
	else
		this->SetTitle(path + " - Markdown editor");
}

// Events

void MarkdownEditorMainWindow::evtCommand(const wxCommandEvent& event)
{
	std::unordered_map<int, CommandHandler> evtmap = {
		{ID_NEW, &MarkdownEditorMainWindow::handlerNew},
		{ID_OPEN, &MarkdownEditorMainWindow::handlerOpen},
		{ID_SAVE, &MarkdownEditorMainWindow::handlerSave},
		{ID_SAVE_AS, &MarkdownEditorMainWindow::handlerSaveAs},
		{ID_EXPORT, &MarkdownEditorMainWindow::handlerExport},
		{ID_EXIT, &MarkdownEditorMainWindow::handlerExit},

		{ID_UNDO, &MarkdownEditorMainWindow::handlerUndo},
		{ID_REDO, &MarkdownEditorMainWindow::handlerRedo},
		{ID_CUT, &MarkdownEditorMainWindow::handlerCut},
		{ID_COPY, &MarkdownEditorMainWindow::handlerCopy},
		{ID_PASTE, &MarkdownEditorMainWindow::handlerPaste},
		{ID_ENABLE_EDITOR, &MarkdownEditorMainWindow::handlerEnableEditor},

		{ID_ABOUT, &MarkdownEditorMainWindow::handlerAbout},

		{ID_PARA, &MarkdownEditorMainWindow::handlerPara},
		{ID_H1, &MarkdownEditorMainWindow::handlerH1},
		{ID_H2, &MarkdownEditorMainWindow::handlerH2},
		{ID_H3, &MarkdownEditorMainWindow::handlerH3},
		{ID_H4, &MarkdownEditorMainWindow::handlerH4},
		{ID_H5, &MarkdownEditorMainWindow::handlerH5},
		{ID_H6, &MarkdownEditorMainWindow::handlerH6},
		{ID_LINE, &MarkdownEditorMainWindow::handlerLine},
		{ID_QUOTE, &MarkdownEditorMainWindow::handlerQuote},
		{ID_CODEBLOCK, &MarkdownEditorMainWindow::handlerCodeBlock},
		{ID_CODE, &MarkdownEditorMainWindow::handlerCode},
		{ID_LINK, &MarkdownEditorMainWindow::handlerLink},
		{ID_IMAGE, &MarkdownEditorMainWindow::handlerImage},
	};

	auto it = evtmap.find(event.GetId());
	if (it == evtmap.end())
		return;

	const auto& handler = it->second;
	(this->*handler)(event);
}

void MarkdownEditorMainWindow::handlerNew(const wxCommandEvent& event)
{
	if (this->promptDocumentClose())
		this->newDocument();
}

void MarkdownEditorMainWindow::handlerOpen(const wxCommandEvent& event)
{
	if (this->promptDocumentClose())
		this->promptOpen();
}

void MarkdownEditorMainWindow::handlerSave(const wxCommandEvent& event)
{
	this->promptSave();
}

void MarkdownEditorMainWindow::handlerSaveAs(const wxCommandEvent& event)
{
	this->promptSaveAs();
}

void MarkdownEditorMainWindow::handlerExport(const wxCommandEvent& event)
{
	this->exportAs();
}

void MarkdownEditorMainWindow::handlerExit(const wxCommandEvent& event)
{
	this->Close();
}

/////

void MarkdownEditorMainWindow::handlerUndo(const wxCommandEvent& event)
{
	m_editor->Undo();
}

void MarkdownEditorMainWindow::handlerRedo(const wxCommandEvent& event)
{
	m_editor->Redo();
}

void MarkdownEditorMainWindow::handlerCut(const wxCommandEvent& event)
{
	m_editor->Cut();
}

void MarkdownEditorMainWindow::handlerCopy(const wxCommandEvent& event)
{
	m_editor->Copy();
}

void MarkdownEditorMainWindow::handlerPaste(const wxCommandEvent& event)
{
	m_editor->Paste();
}

void MarkdownEditorMainWindow::handlerEnableEditor(const wxCommandEvent& event)
{
	if (event.GetId() == ID_ENABLE_EDITOR)
	{
		if (event.IsChecked())
			this->enableEditor();
		else
			this->disableEditor();
	}
}

/////

void MarkdownEditorMainWindow::handlerAbout(const wxCommandEvent& event)
{
	wxAboutDialogInfo aboutInfo;

	aboutInfo.SetName("scribbleMd");
	aboutInfo.SetVersion("0.1");
	aboutInfo.SetDescription(_("Simple open source Markdown editor."));
	aboutInfo.SetCopyright("(C) 2025");
	aboutInfo.SetWebSite("https://github.com/rwypior/scribblemd");
	aboutInfo.AddDeveloper("Robert Wypiór");

	wxAboutBox(aboutInfo, this);
}

/////

void MarkdownEditorMainWindow::handlerPara(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("New paragraph...");
}

void MarkdownEditorMainWindow::handlerH1(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("# Header1", 2);
}

void MarkdownEditorMainWindow::handlerH2(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("## Header2", 3);
}

void MarkdownEditorMainWindow::handlerH3(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("### Header3", 4);
}

void MarkdownEditorMainWindow::handlerH4(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("#### Header4", 5);
}

void MarkdownEditorMainWindow::handlerH5(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("##### Header5", 6);
}

void MarkdownEditorMainWindow::handlerH6(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("###### Header6", 7);
}

void MarkdownEditorMainWindow::handlerLine(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();	
	this->insertAndJumpToNextLine("-----");
}

void MarkdownEditorMainWindow::handlerQuote(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("> Quote...", 2);
}

void MarkdownEditorMainWindow::handlerCodeBlock(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("\tCode block...", 1);
}

void MarkdownEditorMainWindow::handlerCode(const wxCommandEvent& event)
{
	this->insertAndSelectText("`Inline code`", 1, -1);
}

void MarkdownEditorMainWindow::handlerLink(const wxCommandEvent& event)
{
	this->insertAndSelectText("[Title](http://url)", 1, 5);
}

void MarkdownEditorMainWindow::handlerImage(const wxCommandEvent& event)
{
	this->insertAndSelectText("![Alt](Path)", 7, 4);
}

void MarkdownEditorMainWindow::handlerTextChanged(const wxCommandEvent& event)
{
	this->textChanged = true;
	this->timer.StartOnce(500);
}

void MarkdownEditorMainWindow::handlerTimerMarkdown(const wxTimerEvent& event)
{
	m_viewer->setMarkdown(this->m_editor->GetValue());
}

void MarkdownEditorMainWindow::handlerSplitterDoubleclick(const wxSplitterEvent& event)
{
	m_splitter1->SetSashPosition(m_splitter1->GetClientSize().GetWidth() / 2);
}

void MarkdownEditorMainWindow::handlerClose(wxCloseEvent& event)
{
	if (this->promptDocumentClose())
	{
		GeometrySettings settings;
		this->SaveGeometry(settings);

		GeometrySettings::writeGeometry(settings);

		this->Destroy();
	}
}