//! \file **************************************************************
//! \brief Source Poins d'entrée de l'application.
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 1.1
//! \date 12.12.12
//!
//! ********************************************************************

/*
*	Copyright © 2012-2013 - Antoine Maleyrie.
*/

#include "main.hpp"
#include "resource.hpp"
#include "dialogPreferences.hpp"

#include <wx/aboutdlg.h>
#include <wx/fileconf.h>
#include <unistd.h>

//TEST
#include <iostream>
#include "action/actTranslation.hpp"
#include "shortcut.hpp"

// *********************************************************************
// Class App
// *********************************************************************

IMPLEMENT_APP(App);

bool App::OnInit()
{  	
	//Init général
	wxInitAllImageHandlers();
	SetExitOnFrameDelete(false);
	_menuIcon = nullptr;
	
	//Crée de l'instance de ActionManager;
	_actionManager = ActionManager::getInstance();
	
	//Chargement de la config
	wxFileConfig fileConfig(	PROJECT_NAME,
								wxEmptyString,
								wxGetUserHome()+"/."+PROJECT_NAME);
	bool showMenu = true;
	fileConfig.Read("show_menu", &showMenu);
	
	//Création du menu ou pas.
	if(showMenu)
		creatMenuItem();
	else
		deleteMenuItem();
		
	//Installation des raccourcis/actions
	_actionManager->load(fileConfig);
	
	
	wxCommandEvent te;
OnPreferences(te);
	return true;
}

int App::OnExit()
{	
	//Suppression du menu.
	deleteMenuItem();
	
	//Suppression du mangeur d'action.
	ActionManager::kill();
	_actionManager = nullptr;
	
	//Suppression des ressources.
	Resource::kill();

	return 0;
}

void App::creatMenuItem()
{
	if(!_menuIcon)
	{
		_menuIcon = new MenuIcon();
		
		Bind(wxEVT_COMMAND_MENU_SELECTED, &App::OnPreferences, this, _menuIcon->getIdMenuItemPreferences());
		Bind(wxEVT_COMMAND_MENU_SELECTED, &App::OnEnable, this, _menuIcon->getIdMenuItemEnable());
		Bind(wxEVT_COMMAND_MENU_SELECTED, &App::OnAbout, this, _menuIcon->getIdMenuItemAbout());
		Bind(wxEVT_COMMAND_MENU_SELECTED, &App::OnExit, this, _menuIcon->getIdMenuItemExit());
	}
}

void App::deleteMenuItem()
{
	if(_menuIcon)
	{
		Unbind(wxEVT_COMMAND_MENU_SELECTED, &App::OnPreferences, this, _menuIcon->getIdMenuItemPreferences());
		Unbind(wxEVT_COMMAND_MENU_SELECTED, &App::OnEnable, this, _menuIcon->getIdMenuItemEnable());
		Unbind(wxEVT_COMMAND_MENU_SELECTED, &App::OnAbout, this, _menuIcon->getIdMenuItemAbout());
		Unbind(wxEVT_COMMAND_MENU_SELECTED, &App::OnExit, this, _menuIcon->getIdMenuItemExit());
		
		delete _menuIcon;
		_menuIcon = nullptr;
	}
}

void App::OnPreferences(wxCommandEvent&)
{
	static DialogPreferences *dlg = nullptr;
	
	//On lance le dialog si il n'est pas déjà lancer.
	if(dlg == nullptr)
	{
		//Désactivation des raccourcis.
		_actionManager->enable(false);
		
		//Création du dialog.
		dlg = new DialogPreferences(_actionManager);
		
		//Affichage du dialog.
		if(dlg->ShowModal() == wxID_OK)
		{	
			//On vérifie si on doit quitter l'application ou pas.
			if(dlg->shutdownIsToggle())
				ExitMainLoop();
				
			//Vérification si on doit afficher ou pas l'icône dans la zone de notification.
			if(dlg->showIcon())
				creatMenuItem();
			else
				deleteMenuItem();
		}
		
		//On réactive les raccourcis
		_actionManager->enable(true);
		
		//Supprime le dialog
		delete dlg;
		dlg = nullptr;
	}	
	//Sinon on l'affiche au premier plan.
	else
		dlg->Raise();
}

void App::OnEnable(wxCommandEvent& event)
{
	_actionManager->enable(event.IsChecked());
}

void App::OnAbout(wxCommandEvent&)
{
		wxAboutDialogInfo info;

		info.SetName(PROJECT_NAME);
		info.SetVersion(PROJECT_VERSION);
		
		wxString msg;
		msg << _("This software using google translate to translate a word or sentence from your clipboard.");
		msg << _("\n\nBuild on ");
		#if defined(__UNIX__)
		msg << _("Unix ");
		#elif defined(__WXMSW__)
		msg << _("Windows ");
		#endif
		#ifdef __i386
		msg << _("in i386\n");
		#elif __amd64
		msg << _("in x86_64\n");
		#endif
		msg << _("Date : ") << __DATE__;
		
		info.SetDescription(msg);
		info.SetCopyright("(C) 2012-1013");
		info.SetWebSite("http://antoine163.github.com/flydocs/");
		info.AddDeveloper("Maleyrie Antoine <antoine.maleyrie@gmail.com>");
		info.AddDocWriter("Maleyrie Antoine <antoine.maleyrie@gmail.com>");
		
		wxAboutBox(info);
}

void App::OnExit(wxCommandEvent&)
{		
	ExitMainLoop();
}
