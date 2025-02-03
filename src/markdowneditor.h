///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include "wxmarkdownctrl/wxMarkdownCtrl.h"
#include <wx/scrolwin.h>
#include <wx/splitter.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_NEW 1000
#define ID_OPEN 1001
#define ID_SAVE 1002
#define ID_SAVE_AS 1003
#define ID_EXIT 1004
#define ID_UNDO 1005
#define ID_REDO 1006
#define ID_CUT 1007
#define ID_COPY 1008
#define ID_PASTE 1009
#define ID_ABOUT 1010
#define ID_PARA 1011
#define ID_H1 1012
#define ID_H2 1013
#define ID_H3 1014
#define ID_H4 1015
#define ID_H5 1016
#define ID_H6 1017
#define ID_LINE 1018
#define ID_QUOTE 1019
#define ID_CODEBLOCK 1020
#define ID_CODE 1021
#define ID_LINK 1022
#define ID_IMAGE 1023

///////////////////////////////////////////////////////////////////////////////
/// Class MainWindow
///////////////////////////////////////////////////////////////////////////////
class MainWindow : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar1;
		wxMenu* file;
		wxMenuItem* newfile;
		wxMenu* edit;
		wxMenu* help;
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* m_paragraph;
		wxToolBarToolBase* m_h1;
		wxToolBarToolBase* m_h2;
		wxToolBarToolBase* m_h3;
		wxToolBarToolBase* m_h4;
		wxToolBarToolBase* m_h5;
		wxToolBarToolBase* m_h6;
		wxToolBarToolBase* m_line;
		wxToolBarToolBase* m_blockquote;
		wxToolBarToolBase* m_codeblock;
		wxToolBarToolBase* m_code;
		wxToolBarToolBase* m_link;
		wxToolBarToolBase* m_image;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_editor;
		wxPanel* m_panel2;
		wxStaticText* m_staticText2;
		wxScrolledWindow* m_scrolledWindow1;
		MarkdownCtrl* m_viewer;

	public:

		MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Markdown editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 612,401 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MainWindow();

		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 0 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::m_splitter1OnIdle ), NULL, this );
		}

};

