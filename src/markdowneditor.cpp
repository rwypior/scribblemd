///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "markdowneditor.h"

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	file = new wxMenu();
	newfile = new wxMenuItem( file, ID_NEW, wxString( wxT("New") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	file->Append( newfile );

	wxMenuItem* open;
	open = new wxMenuItem( file, ID_OPEN, wxString( wxT("Open...") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	file->Append( open );

	wxMenuItem* save;
	save = new wxMenuItem( file, ID_SAVE, wxString( wxT("Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	file->Append( save );

	wxMenuItem* saveAs;
	saveAs = new wxMenuItem( file, ID_SAVE_AS, wxString( wxT("Save as...") ) + wxT('\t') + wxT("Ctrl+Shift+S"), wxEmptyString, wxITEM_NORMAL );
	file->Append( saveAs );

	file->AppendSeparator();

	wxMenuItem* exit;
	exit = new wxMenuItem( file, ID_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( exit );

	m_menubar1->Append( file, wxT("File") );

	edit = new wxMenu();
	wxMenuItem* undo;
	undo = new wxMenuItem( edit, ID_UNDO, wxString( wxT("Undo") ) + wxT('\t') + wxT("Ctrl_Z"), wxEmptyString, wxITEM_NORMAL );
	edit->Append( undo );

	wxMenuItem* redo;
	redo = new wxMenuItem( edit, ID_REDO, wxString( wxT("Redo") ) + wxT('\t') + wxT("Ctrl+Y"), wxEmptyString, wxITEM_NORMAL );
	edit->Append( redo );

	edit->AppendSeparator();

	wxMenuItem* cut;
	cut = new wxMenuItem( edit, ID_CUT, wxString( wxT("Cut") ) + wxT('\t') + wxT("Ctrl+X"), wxEmptyString, wxITEM_NORMAL );
	edit->Append( cut );

	wxMenuItem* copy;
	copy = new wxMenuItem( edit, ID_COPY, wxString( wxT("Copy") ) + wxT('\t') + wxT("Ctrl+C"), wxEmptyString, wxITEM_NORMAL );
	edit->Append( copy );

	wxMenuItem* paste;
	paste = new wxMenuItem( edit, ID_PASTE, wxString( wxT("Paste") ) + wxT('\t') + wxT("Ctrl+V"), wxEmptyString, wxITEM_NORMAL );
	edit->Append( paste );

	m_menubar1->Append( edit, wxT("Edit") );

	help = new wxMenu();
	wxMenuItem* about;
	about = new wxMenuItem( help, ID_ABOUT, wxString( wxT("About") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	help->Append( about );

	m_menubar1->Append( help, wxT("Help") );

	this->SetMenuBar( m_menubar1 );

	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_paragraph = m_toolBar1->AddTool( ID_PARA, wxT("tool"), wxBitmap( wxT("res/para.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add paragraph"), wxEmptyString, NULL );

	m_h1 = m_toolBar1->AddTool( ID_H1, wxT("tool"), wxBitmap( wxT("res/h1.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add heading 1"), wxEmptyString, NULL );

	m_h2 = m_toolBar1->AddTool( ID_H2, wxT("tool"), wxBitmap( wxT("res/h2.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add heading 2"), wxEmptyString, NULL );

	m_h3 = m_toolBar1->AddTool( ID_H3, wxT("tool"), wxBitmap( wxT("res/h3.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add heading 3"), wxEmptyString, NULL );

	m_h4 = m_toolBar1->AddTool( ID_H4, wxT("tool"), wxBitmap( wxT("res/h4.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add heading 4"), wxEmptyString, NULL );

	m_h5 = m_toolBar1->AddTool( ID_H5, wxT("tool"), wxBitmap( wxT("res/h5.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add heading 5"), wxEmptyString, NULL );

	m_h6 = m_toolBar1->AddTool( ID_H6, wxT("tool"), wxBitmap( wxT("res/h6.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add heading 6"), wxEmptyString, NULL );

	m_line = m_toolBar1->AddTool( ID_LINE, wxT("tool"), wxBitmap( wxT("res/line.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add line"), wxEmptyString, NULL );

	m_blockquote = m_toolBar1->AddTool( ID_QUOTE, wxT("tool"), wxBitmap( wxT("res/quote.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add blockquote"), wxEmptyString, NULL );

	m_codeblock = m_toolBar1->AddTool( ID_CODEBLOCK, wxT("tool"), wxBitmap( wxT("res/codeblock.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add code block"), wxEmptyString, NULL );

	m_code = m_toolBar1->AddTool( ID_CODE, wxT("tool"), wxBitmap( wxT("res/code.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add inline code"), wxEmptyString, NULL );

	m_link = m_toolBar1->AddTool( ID_LINK, wxT("tool"), wxBitmap( wxT("res/link.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add link"), wxEmptyString, NULL );

	m_image = m_toolBar1->AddTool( ID_IMAGE, wxT("tool"), wxBitmap( wxT("res/image.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Add image"), wxEmptyString, NULL );

	m_toolBar1->Realize();

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->SetSashGravity( 0.5 );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::m_splitter1OnIdle ), NULL, this );
	m_splitter1->SetMinimumPaneSize( 100 );

	m_panel1 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_staticText1 = new wxStaticText( m_panel1, wxID_ANY, wxT("Source"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer4->Add( m_staticText1, 0, wxALL, 5 );

	m_editor = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer4->Add( m_editor, 1, wxALL|wxEXPAND, 5 );


	m_panel1->SetSizer( bSizer4 );
	m_panel1->Layout();
	bSizer4->Fit( m_panel1 );
	m_panel2 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_staticText2 = new wxStaticText( m_panel2, wxID_ANY, wxT("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer5->Add( m_staticText2, 0, wxALL, 5 );

	m_scrolledWindow1 = new wxScrolledWindow( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_viewer = new MarkdownCtrl( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_viewer, 1, wxALL|wxEXPAND, 5 );


	m_scrolledWindow1->SetSizer( bSizer6 );
	m_scrolledWindow1->Layout();
	bSizer6->Fit( m_scrolledWindow1 );
	bSizer5->Add( m_scrolledWindow1, 1, wxEXPAND | wxALL, 0 );


	m_panel2->SetSizer( bSizer5 );
	m_panel2->Layout();
	bSizer5->Fit( m_panel2 );
	m_splitter1->SplitVertically( m_panel1, m_panel2, 0 );
	bSizer1->Add( m_splitter1, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

MainWindow::~MainWindow()
{
}
