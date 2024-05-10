///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUI.h"

///////////////////////////////////////////////////////////////////////////

MyFrame1::MyFrame1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	bSizer2->SetMinSize( wxSize( 200,400 ) );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_button_load = new wxButton( this, wxID_ANY, wxT("Wczytaj obrazek"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button_load, 0, wxALL, 5 );

	m_button_blur = new wxButton( this, wxID_ANY, wxT("Cenzura"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button_blur, 0, wxALL, 5 );

	m_button_erode = new wxButton( this, wxID_ANY, wxT("Erode"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button_erode, 0, wxALL, 5 );

	m_checkBox_animation = new wxCheckBox( this, wxID_ANY, wxT("Animacja"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_checkBox_animation, 0, wxALL, 5 );

	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	bSizer3->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );


	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );


	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );

	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer1->Add( m_panel1, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_button_load->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::button_load_click ), NULL, this );
	m_button_blur->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::bitton_blur_click ), NULL, this );
	m_button_erode->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::button_erode_click ), NULL, this );
	m_checkBox_animation->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::checko_box_check ), NULL, this );
	m_panel1->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MyFrame1::panel_update ), NULL, this );
}

MyFrame1::~MyFrame1()
{
	// Disconnect Events
	m_button_load->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::button_load_click ), NULL, this );
	m_button_blur->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::bitton_blur_click ), NULL, this );
	m_button_erode->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::button_erode_click ), NULL, this );
	m_checkBox_animation->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::checko_box_check ), NULL, this );
	m_panel1->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MyFrame1::panel_update ), NULL, this );

}
