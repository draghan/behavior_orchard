///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Apr  5 2020)
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
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/panel.h>
#include <wx/ribbon/page.h>
#include <wx/ribbon/control.h>
#include <wx/ribbon/art.h>
#include <wx/ribbon/bar.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/stc/stc.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class GeneratedMainFrame
///////////////////////////////////////////////////////////////////////////////
class GeneratedMainFrame : public wxFrame
{
	private:

	protected:
		wxRibbonBar* m_ribbonBar1;
		wxRibbonPage* ribbon_page_general;
		wxRibbonPanel* ribbon_panel_file;
		wxRibbonButtonBar* m_ribbonButtonBar1;
		wxRibbonPanel* ribbon_panel_misc;
		wxRibbonButtonBar* m_ribbonButtonBar6;
		wxRibbonPage* ribbon_page_nodes;
		wxRibbonPanel* ribbon_panel_create;
		wxRibbonButtonBar* m_ribbonButtonBar21;
		wxRibbonPanel* ribbon_panel_edit;
		wxRibbonButtonBar* m_ribbonButtonBar611;
		wxRibbonPanel* ribbon_panel_goto;
		wxRibbonButtonBar* m_ribbonButtonBar32;
		wxRibbonPanel* ribbon_panel_show;
		wxRibbonButtonBar* m_ribbonButtonBar311;
		wxScrolledWindow* workspace;
		wxTextCtrl* m_textCtrl1;
		wxTextCtrl* m_textCtrl2;
		wxTextCtrl* m_textCtrl3;
		wxTextCtrl* m_textCtrl4;
		wxTextCtrl* m_textCtrl5;
		wxStyledTextCtrl* m_scintilla1;

		// Virtual event handlers, overide them in your derived class
		virtual void MainFrameOnClose( wxCloseEvent& event ) { event.Skip(); }


	public:

		GeneratedMainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Behavior Orchard"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~GeneratedMainFrame();

};

