//! \file **************************************************************
//! \brief Source MenuIcon.
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 0.3
//! \date 10.12.12
//!
//! ********************************************************************

/*
*	Copyright © 2012-2013 - Antoine Maleyrie.
*/

#include "menuIcon.hpp"

#include <wx/event.h>
#include <wx/icon.h>
#include <wx/artprov.h>

#include "../icons/ft.xpm"

// *********************************************************************
// Class MenuIcon
// *********************************************************************

MenuIcon::MenuIcon()
{
	//Menu
	_menuTaskBarIcon = new wxMenu();
	
	//Menu Item
	_menuItemPreferences = new wxMenuItem(_menuTaskBarIcon, wxID_PREFERENCES, _("Preferences"), _("Open Preferences"), wxITEM_NORMAL);
	_menuTaskBarIcon->Append(_menuItemPreferences);
	
	_menuItemEnableShortcuts = new wxMenuItem(_menuTaskBarIcon, wxID_ANY, _("Enable Shortcuts"), _("Activate Shortcuts"), wxITEM_CHECK);
	_menuTaskBarIcon->Append(_menuItemEnableShortcuts);
	_menuItemEnableShortcuts->Check(true);
	
	_menuItemEnableActions = new wxMenuItem(_menuTaskBarIcon, wxID_ANY, _("Enable Actions"), _("Activate Actions"), wxITEM_CHECK);
	_menuTaskBarIcon->Append(_menuItemEnableActions);
	_menuItemEnableActions->Check(true);
	
	_menuItemAbout = new wxMenuItem(_menuTaskBarIcon, wxID_ABOUT, _("About"), _("About")+" "+PROJECT_NAME, wxITEM_NORMAL);
	_menuTaskBarIcon->Append(_menuItemAbout);
	
	_menuItemExit = new wxMenuItem(_menuTaskBarIcon, wxID_EXIT, _("Exit"), _("Exit")+" "+PROJECT_NAME, wxITEM_NORMAL);
	_menuTaskBarIcon->Append(_menuItemExit);
	
	//Task Icon
	_taskBarIcon = new wxTaskBarIcon;
	_taskBarIcon->SetIcon(wxIcon(ft_xpm), PROJECT_NAME);
	
	//bind l'événement
	_taskBarIcon->Bind(wxEVT_TASKBAR_LEFT_DCLICK, &MenuIcon::OnShow, this);
}

MenuIcon::~MenuIcon()
{
	_taskBarIcon->Unbind(wxEVT_TASKBAR_LEFT_DCLICK, &MenuIcon::OnShow, this);
	
	delete _taskBarIcon;
	delete _menuTaskBarIcon;
}

int MenuIcon::getIdMenuItemPreferences()
{
	return _menuItemPreferences->GetId();
}

int MenuIcon::getIdMenuItemEnableShortcuts()
{
	return _menuItemEnableShortcuts->GetId();
}

int MenuIcon::getIdMenuItemEnableActions()
{
	return _menuItemEnableActions->GetId();
}

int MenuIcon::getIdMenuItemAbout()
{
	return _menuItemAbout->GetId();
}

int MenuIcon::getIdMenuItemExit()
{
	return _menuItemExit->GetId();
}

void MenuIcon::OnShow(wxTaskBarIconEvent&)
{
	_taskBarIcon->PopupMenu(_menuTaskBarIcon);
}
