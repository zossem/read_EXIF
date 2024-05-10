///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checkbox.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame
{
	private:

	protected:
		wxButton* m_button_load;
		wxButton* m_button_blur;
		wxButton* m_button_erode;
		wxCheckBox* m_checkBox_animation;
		wxNotebook* m_notebook;
		wxPanel* m_panel1;

		// Virtual event handlers, override them in your derived class
		virtual void button_load_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void bitton_blur_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void button_erode_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void checko_box_check( wxCommandEvent& event ) { event.Skip(); }
		virtual void panel_update( wxUpdateUIEvent& event ) { event.Skip(); }


	public:

		MyFrame1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 758,532 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame1();

};

