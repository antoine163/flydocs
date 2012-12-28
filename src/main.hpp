//12/12/12

#ifndef MAIN_H
#define MAIN_H

#include <wx/app.h>

#include "shortcut.hpp"
#include "menuIcon.hpp"
#include "word.hpp"

class App : public wxApp
{
	public:		
		virtual bool OnInit();
		virtual int OnExit();
		
		//Menu Item Methode
		void creatMenuItem();
		void deleteMenuItem();
		//Menu Item Event Methode
		void OnPreferences(wxCommandEvent&);
		void OnEnable(wxCommandEvent&);
		//! \bug Ne quitte pas correctement.
		void OnExit(wxCommandEvent&);
		void OnShortcut(ShortcutEvent& event);

		//--
		wxString getClipboard()const;
		
		void translateClipBoard();	
		void soveTranslateClipBoard();
	
	private:
		Shortcut *_shortcut;
		MenuIcon *_menuIcon;
		Word _word;	
};

#endif //MAIN_H
