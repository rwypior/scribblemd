#include "MarkdownEditorMainWindow.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include <unordered_map>

MarkdownEditorMainWindow::MarkdownEditorMainWindow( wxWindow* parent )
	: MainWindow( parent )
	, timer(this, wxID_ANY)
{
	this->Bind(wxEVT_MENU, &MarkdownEditorMainWindow::evtCommand, this);
	this->m_toolBar1->Bind(wxEVT_TOOL, &MarkdownEditorMainWindow::evtCommand, this);

	auto resDir = wxFileName(wxStandardPaths::Get().GetExecutablePath());
	resDir.AppendDir("res");
	
	auto iconPath = wxFileName(resDir.GetPath(), "markdown.ico").GetFullPath();

	this->SetIcon(wxIcon(iconPath, wxBitmapType::wxBITMAP_TYPE_ICO));
	
	this->m_editor->Bind(wxEVT_TEXT, &MarkdownEditorMainWindow::handlerTextChanged, this);
	this->Bind(wxEVT_TIMER, &MarkdownEditorMainWindow::handlerTimerMarkdown, this, this->timer.GetId());
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

bool MarkdownEditorMainWindow::hasUnsavedChanges() const
{
	return true;
}

bool MarkdownEditorMainWindow::assureChangesAreSaved()
{
	if (this->hasUnsavedChanges())
	{
		//wxMessageBox("Do you want to save changes to the file?", "ScribbleMd", wxICON_QUESTION | wxbutton_yes);

	}

	return true;
}

// Events

void MarkdownEditorMainWindow::evtCommand(const wxCommandEvent& event)
{
	std::unordered_map<int, CommandHandler> evtmap = {
		{ID_NEW, &MarkdownEditorMainWindow::handlerNew},
		{ID_OPEN, &MarkdownEditorMainWindow::handlerOpen},
		{ID_SAVE, &MarkdownEditorMainWindow::handlerSave},
		{ID_SAVE_AS, &MarkdownEditorMainWindow::handlerSaveAs},
		{ID_EXIT, &MarkdownEditorMainWindow::handlerExit},

		{ID_UNDO, &MarkdownEditorMainWindow::handlerUndo},
		{ID_REDO, &MarkdownEditorMainWindow::handlerRedo},
		{ID_CUT, &MarkdownEditorMainWindow::handlerCut},
		{ID_COPY, &MarkdownEditorMainWindow::handlerCopy},
		{ID_PASTE, &MarkdownEditorMainWindow::handlerPaste},

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
	
}

void MarkdownEditorMainWindow::handlerOpen(const wxCommandEvent& event)
{

}

void MarkdownEditorMainWindow::handlerSave(const wxCommandEvent& event)
{

}

void MarkdownEditorMainWindow::handlerSaveAs(const wxCommandEvent& event)
{

}

void MarkdownEditorMainWindow::handlerExit(const wxCommandEvent& event)
{

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

/////

void MarkdownEditorMainWindow::handlerAbout(const wxCommandEvent& event)
{

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
	this->insertAndSelectText("* Header1", 2);
}

void MarkdownEditorMainWindow::handlerH2(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("** Header2", 3);
}

void MarkdownEditorMainWindow::handlerH3(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("*** Header3", 4);
}

void MarkdownEditorMainWindow::handlerH4(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("**** Header4", 5);
}

void MarkdownEditorMainWindow::handlerH5(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("***** Header5", 6);
}

void MarkdownEditorMainWindow::handlerH6(const wxCommandEvent& event)
{
	this->insertNewLineIfNeeded();
	this->insertAndSelectText("****** Header6", 7);
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
	this->timer.StartOnce(500);
}

void MarkdownEditorMainWindow::handlerTimerMarkdown(const wxTimerEvent& event)
{
	m_viewer->setMarkdown(this->m_editor->GetValue());
}