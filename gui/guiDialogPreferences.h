///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 24 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __guiDialogPreferences__
#define __guiDialogPreferences__

#include <wx/intl.h>

#include <wx/listctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/tglbtn.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GuiPanelList
///////////////////////////////////////////////////////////////////////////////
class GuiPanelList : public wxPanel 
{
	private:
	
	protected:
		wxListCtrl* _listCtrl;
		wxButton* _buttonDelete;
		wxButton* _buttonPreferences;
		wxButton* _buttonAdd;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnListItemDeselected( wxListEvent& event ) { event.Skip(); }
		virtual void OnListItemRightClick( wxListEvent& event ) { event.Skip(); }
		virtual void OnListItemSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnButtonClickDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickPreferences( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickAdd( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GuiPanelList( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
		~GuiPanelList();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class GuiDialogPreferences
///////////////////////////////////////////////////////////////////////////////
class GuiDialogPreferences : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* _notebook;
		wxPanel* _panelSetting;
		wxStaticText* _staticTextGeneral;
		
		wxCheckBox* _checkBoxShowMenu;
		
		wxCheckBox* _checkBoxPowerOn;
		wxStaticText* _staticTextShutdown;
		wxToggleButton* _toggleBtnTurnOff;
		wxStdDialogButtonSizer* _sdbSizer;
		wxButton* _sdbSizerOK;
		wxButton* _sdbSizerApply;
		wxButton* _sdbSizerCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNotebookPageChanging( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnButtonClickApply( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickOK( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GuiDialogPreferences( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_DIALOG_STYLE|wxDIALOG_NO_PARENT );
		~GuiDialogPreferences();
	
};

#endif //__guiDialogPreferences__
