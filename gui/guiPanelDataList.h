///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 24 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __guiPanelDataList__
#define __guiPanelDataList__

#include <wx/intl.h>

#include <wx/dataview.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GuiPanelDataList
///////////////////////////////////////////////////////////////////////////////
class GuiPanelDataList : public wxPanel 
{
	private:
	
	protected:
		wxDataViewListCtrl* _listCtrl;
		wxButton* _buttonDelete;
		wxButton* _buttonPreferences;
		wxButton* _buttonAdd;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClickDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickPreferences( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickAdd( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GuiPanelDataList( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
		~GuiPanelDataList();
	
};

#endif //__guiPanelDataList__