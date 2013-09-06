//! \file **************************************************************
//! \brief Source Dialogue générale des préférences.
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 3.8
//! \date 02.01.2013
//!
//! ********************************************************************

/*
*	Copyright © 2013 - Antoine Maleyrie.
*/

#include "dialogPreferences.hpp"
#include "dialogActionPreferences.hpp"
#include "dialogListAdd.hpp"
#include "shortcut.hpp"
#include "resource.hpp"
#include "actionManager.hpp"
#include "listManager.hpp"

#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include <wx/menu.h>

//TEST
#include <iostream>

// *********************************************************************
// Class PanelList
// *********************************************************************

PanelList::PanelList(wxWindow* parent, wxString name)
: GuiPanelList(parent), _name(name)
{
	#if wxCHECK_VERSION(2, 9, 5)
		_listCtrl->EnableAlternateRowColours();
	#endif
	
	//Construction du menu
	_menu = new wxMenu();
		
	_menuItemListAdd = new wxMenuItem(_menu,  wxID_ADD);
	_menu->Append(_menuItemListAdd);
	_menuItemListPreferences = new wxMenuItem(_menu, wxID_PREFERENCES);
	_menu->Append(_menuItemListPreferences);
	_menuItemListDelete = new wxMenuItem(_menu, wxID_DELETE);
	_menu->Append(_menuItemListDelete);
	
	_menuItemListPreferences->Enable(false);
	_menuItemListDelete->Enable(false);
	
	_menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &PanelList::OnButtonClickAdd, this, wxID_ADD);
	_menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &PanelList::OnButtonClickPreferences, this, wxID_PREFERENCES);
	_menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &PanelList::OnButtonClickDelete, this, wxID_DELETE);
}

PanelList::~PanelList()
{
	//Destruction du menu
	_menu->Unbind(wxEVT_COMMAND_MENU_SELECTED, &PanelList::OnButtonClickAdd, this, wxID_ADD);
	_menu->Unbind(wxEVT_COMMAND_MENU_SELECTED, &PanelList::OnButtonClickPreferences, this, wxID_PREFERENCES);
	_menu->Unbind(wxEVT_COMMAND_MENU_SELECTED, &PanelList::OnButtonClickDelete, this, wxID_DELETE);

	delete _menu;
}

void PanelList::addItem(wxArrayString const& item, bool select)
{
	//Vérifie si l'item est vide.
	if(item.GetCount() == 0)
		return;
		
	//Ajout l'item au débute.
	_listCtrl->InsertItem(0, item[0]);
	for(size_t i = 1; i < item.GetCount(); i++)
	{
		_listCtrl->SetItem(0, i, item[i]);
	}
	
	//Rend visible l'item.
	_listCtrl->EnsureVisible(0);
	
	//On sélectionne l'items au besoins.
	if(select)
		_listCtrl->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void PanelList::clear()
{
	_listCtrl->DeleteAllItems();
	_listItemSelected.clear();
	
	//On désactive les boutons
	_buttonDelete->Enable(false);
	_menuItemListDelete->Enable(false);
	_buttonPreferences->Enable(false);
	_menuItemListPreferences->Enable(false);
}

void PanelList::OnButtonClickDelete(wxCommandEvent&)
{
	wxMessageDialog *dlg = nullptr;
	
	//Création du dialog.
	if(_listItemSelected.size() > 1)
		dlg = new wxMessageDialog(this, wxString::Format(_("Do you want really delete this %ss ?"), _name), wxString::Format(_("Delete %ss"), _name), wxYES_NO|wxICON_QUESTION|wxCENTRE);
	else
		dlg = new wxMessageDialog(this, wxString::Format(_("Do you want really delete this %s ?"), _name), wxString::Format(_("Delete %s"), _name), wxYES_NO|wxICON_QUESTION|wxCENTRE);
    
    //Affichage du dialog
    if(dlg->ShowModal() == wxID_YES)
	{
		//Supprimer tous les items sélectionnés
		while(!_listItemSelected.empty())
		{	
			//L'item a supprimer.
			wxString tmpItem = _listItemSelected.back().GetText();
			
			//On cherche l'id de l'item.
			long idItem = _listCtrl->FindItem(-1, tmpItem);
			
			//On le supprime de la liste.
			_listCtrl->DeleteItem(idItem);
			_listItemSelected.pop_back();
			OnDeleteItem(tmpItem);
			
			//On désactive les boutons
			_buttonDelete->Enable(false);
			_menuItemListDelete->Enable(false);
			_buttonPreferences->Enable(false);
			_menuItemListPreferences->Enable(false);
		}
	}
	
    dlg->Destroy();
}

void PanelList::OnButtonClickPreferences(wxCommandEvent&)
{	
	//Préférence de l'item.
	wxArrayString tmpItem = OnPreferencesItem(_listItemSelected[0].GetText());
	
	//Mise à jour de l'item dans la liste.
	long idItem = _listItemSelected[0].GetId();
	for(size_t i = 0; i < tmpItem.GetCount(); i++)
	{
		_listCtrl->SetItem(idItem, i, tmpItem[i]);
	}
}

void PanelList::OnButtonClickAdd(wxCommandEvent&)
{
	//Ajout d'un nouveau item.
	wxArrayString newItem = OnAddItem();
	
	//tmpItem n'est pas vide ?
	if(newItem.GetCount() > 0)
	{
		//Désélectionne tout les items
		long tmpItem = -1;
		for(;;)
		{
			tmpItem = _listCtrl->GetNextItem(tmpItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if(tmpItem == -1)
				break;
				
			_listCtrl->SetItemState(tmpItem, 0, wxLIST_STATE_SELECTED);
		}
	
		//Ajout de l'item
		addItem(newItem);
	}
}

void PanelList::OnListItemRightClick(wxListEvent& event)
{
	//Propagation de l'événement.
	event.Skip();
	
	//Affichage du menu
	_listCtrl->PopupMenu(_menu);
}
		
void PanelList::OnListItemDeselected(wxListEvent& event)
{
	//Recherche et suppression de l'item désélectionner.
	for(size_t i = 0; i<_listItemSelected.size(); i++)
	{
		if(_listItemSelected[i] == event.GetItem())
		{
			_listItemSelected.erase(_listItemSelected.begin()+i);
			break;
		}
	}
	
	//Si rien n'est sélectionner on désactive les boutons delete.
	if(_listItemSelected.size() <= 0)
	{
		_buttonDelete->Enable(false);
		_menuItemListDelete->Enable(false);
	}
		
	//On désactive le bouton Préférence soft si il y a un seul item de sélectionner.
	if(_listItemSelected.size() != 1)
	{
		_buttonPreferences->Enable(false);
		_menuItemListPreferences->Enable(false);
	}
	else
	{
		_buttonPreferences->Enable();
		_menuItemListPreferences->Enable();
	}
}

void PanelList::OnListItemSelected(wxListEvent& event)
{
	_listItemSelected.push_back(event.GetItem());
	
	//Activation du bouton delete
	_buttonDelete->Enable();
	_menuItemListDelete->Enable();
	
	//On active le bouton préférence seulement si il y a qu'un item de sélectionner.
	if(_listItemSelected.size() == 1)
	{
		_buttonPreferences->Enable();
		_menuItemListPreferences->Enable();
	}
	else
	{
		_buttonPreferences->Enable(false);
		_menuItemListPreferences->Enable(false);
	}
}

// *********************************************************************
// Class PanelListActions
// *********************************************************************

PanelListActions::PanelListActions(wxWindow* parent)
: PanelList(parent, _("action"))
{
	_listCtrl->AppendColumn(_("Shortcut"), wxLIST_FORMAT_LEFT, 100);
	_listCtrl->AppendColumn(_("Action"), wxLIST_FORMAT_LEFT, 120);
	_listCtrl->AppendColumn(_("Preferences"), wxLIST_FORMAT_LEFT, 255);
}

PanelListActions::~PanelListActions()
{
}

void PanelListActions::update()
{
	//Vide la liste
	clear();
	
	//Rempli la liste.
	auto actions = EditActionManager::getInstance()->getData();
	for(auto it: actions)
	{
		//Récupération du raccourci
		wxString shortcut = ShortcutKey::shortcutKeyToString(it.first);
		
		//Préparation d'un wxArrayString pour l'ajout d'un item
		wxArrayString tmpItem;
		tmpItem.Add(shortcut);
		tmpItem.Add(it.second->getName());
		tmpItem.Add(it.second->getStringPreferences());
		
		//Ajout de l'item dans la liste
		addItem(tmpItem, false);
	}
}

bool PanelListActions::OnDeleteItem(wxString const& item)
{
	//Récupération de EditActionManager
	EditActionManager* editActionManager = EditActionManager::getInstance();
	
	//Récupération l'action à supprimer.
	Action* action = editActionManager->getValue(ShortcutKey::stringToShortcutKey(item));
	
	//Récupération du nom de la liste utiliser par cette action.
	wxString listUsed = action->getListNameUsed();
	
	//Suppression de l'action.
	editActionManager->remove(ShortcutKey::stringToShortcutKey(item));
	
	//Si une liste est utiliser par cette liste.
	if(!listUsed.IsEmpty())
	{
		//Récupération des raccourcis/actions utilisent la même liste que l'action à supprimer.
		std::vector<ShortcutKey> shortcuts = editActionManager->getShortcutUsedList(listUsed);
		
		//Si il n'y a pas d'autre raccourcis.
		if(shortcuts.size() == 0)
		{
			//Poser la question si on dois les supprimés la liste.
			wxMessageDialog dlg(this, wxString::Format(_("The list %s isn't used. You want delete the list?"), listUsed), _("Delete list"), wxYES_NO|wxICON_QUESTION|wxCENTRE);
			if(dlg.ShowModal() == wxID_YES)
			{
				//Suppression de la liste.
				EditListManager::getInstance()->getValue(listUsed)->removeFile();
				EditListManager::getInstance()->remove(listUsed);
			}
		}
	}
	
	return true;
}

wxArrayString PanelListActions::OnPreferencesItem(wxString const& item)
{
	//wxArrayString de retours.
	wxArrayString newItem;
	
	//Récupération du vieux raccourci.
	ShortcutKey oldShortcutKey = ShortcutKey::stringToShortcutKey(item);
	
	//Récupération de l'action.
	Action* oldAct = EditActionManager::getInstance()->getValue(oldShortcutKey);
	
	DialogActionPreferences dlg(this, item, oldAct);
	while(1)
	{
		//Montre le dialogue.
		if(dlg.ShowModal() == wxID_OK)
		{
			//Récupère le nouveau raccourci sélectionner.
			ShortcutKey newShortcutKey = ShortcutKey::stringToShortcutKey(dlg.getShortcut());
			
			//Si le raccourci a été modifier.
			if(oldShortcutKey != newShortcutKey)
			{				
				//Vérifie si le raccourci n'est pas déjà existent.
				if(EditActionManager::getInstance()->exist(newShortcutKey))
				{
					wxMessageDialog dlg(this, _("The shortcut already exist!"), _("Shortcut exist"), wxOK|wxICON_EXCLAMATION|wxCENTRE);
					dlg.ShowModal();
					
					continue;
				}
			}
			
			//On récupère la nouvelle action.
			Action* newAct = Action::newAction(dlg.getAction());
			
			//Libère la mémoire de l'ancienne action.
			EditActionManager::getInstance()->remove(oldShortcutKey);
			
			//Nouvelle action.
			EditActionManager::getInstance()->add(newShortcutKey, newAct);
			
			//Mise à jour de l'item.
			newItem.Add(ShortcutKey::shortcutKeyToString(newShortcutKey));
			newItem.Add(newAct->getName());
			newItem.Add(newAct->getStringPreferences());
		}
		break;
	}
	
	return newItem;
}

wxArrayString PanelListActions::OnAddItem()
{
	//wxArrayString de retour.
	wxArrayString newItem;
	
	DialogActionPreferences dlg(this);
	while(1)
	{
		//Montre le dialogue
		if(dlg.ShowModal() == wxID_OK)
		{
			//Récupère le nouveau raccourci sélectionner.
			ShortcutKey shortcutKey = ShortcutKey::stringToShortcutKey(dlg.getShortcut());
			
			//Vérifie si le raccourci n'est pas déjà existent.
			if(EditActionManager::getInstance()->exist(shortcutKey))
			{
				wxMessageDialog dlg(this, _("The shortcut already exist!"), _("Shortcut exist"), wxOK|wxICON_EXCLAMATION|wxCENTRE);
				dlg.ShowModal();
				
				continue;
			}
			
			//Nouvelle action
			Action* newAct = Action::newAction(dlg.getAction());
			EditActionManager::getInstance()->add(shortcutKey, newAct);
			
			//Un nouveau item
			newItem.Add(ShortcutKey::shortcutKeyToString(shortcutKey));
			newItem.Add(newAct->getName());
			newItem.Add(newAct->getStringPreferences());
		}
		break;
	}
	
	return newItem;
}

// *********************************************************************
// Class PanelListLists
// *********************************************************************

PanelListLists::PanelListLists(wxWindow* parent)
: PanelList(parent, _("list"))
{
	_listCtrl->AppendColumn(_("Name"), wxLIST_FORMAT_LEFT, 158);
	_listCtrl->AppendColumn(_("Language source"), wxLIST_FORMAT_LEFT, 158);
	_listCtrl->AppendColumn(_("Language of translation"), wxLIST_FORMAT_LEFT, 158);
}

PanelListLists::~PanelListLists()
{
}

void PanelListLists::update()
{
	//Vide la liste.
	clear();
	
	wxString tmplgsrc;
	wxString tmplgto;
	
	//Rempli la liste.
	auto lists = EditListManager::getInstance()->getData();
	for(auto it: lists)
	{		
		//Récupération des langues
		it.second->getlanguages(&tmplgsrc, &tmplgto);
		
		//Préparation d'un wxArrayString pour l'ajout d'un item.
		wxArrayString tmpItem;
		tmpItem.Add(it.first);
		tmpItem.Add(Resource::getInstance()->abbreviationToLanguage(tmplgsrc));
		tmpItem.Add(Resource::getInstance()->abbreviationToLanguage(tmplgto));
		
		//Ajout de l'item dans la liste.
		addItem(tmpItem, false);
	}
}

bool PanelListLists::OnDeleteItem(wxString const& item)
{
	auto shortcuts = EditActionManager::getInstance()->getShortcutUsedList(item);
	
	//Des action utilise cette liste ?
	if(shortcuts.size() != 0)
	{
		//Si il y a des action qui utilise cette liste,
		//On demande confirmation de pour supprimer aussi les action utiliser cette liste.
		wxMessageDialog dlg(this, _("Of actions use this list. Are you sure of want delete the actions and a list?"), _("Delete actions and list"), wxYES_NO|wxICON_QUESTION|wxCENTRE);
		if(dlg.ShowModal() == wxID_NO)
			return false;
	}
	
	//Suppression des actions.
	for(auto it: shortcuts)
		EditActionManager::getInstance()->remove(it);
	
	//Suppression de la liste.
	EditListManager::getInstance()->getValue(item)->removeFile();
	EditListManager::getInstance()->remove(item);
	
	return true;
}

//! \todo A implémenter.
wxArrayString PanelListLists::OnPreferencesItem(wxString const& item)
{
	wxMessageDialog dlg(this, _("Sorry, No implemented!"), _("No implemented"), wxOK|wxICON_INFORMATION|wxCENTRE);
	dlg.ShowModal();
	return wxArrayString();
}

wxArrayString PanelListLists::OnAddItem()
{	
	//wxArrayString de retours.
	wxArrayString newItem;
	
	DialogListAdd dlg(this);
	while(1)
	{
		//Montre le dialogue
		if(dlg.ShowModal() == wxID_OK)
		{
			//Récupère la nouvelle liste.
			wxString newList = dlg.getNamelist();
			
			//newList est vide ?
			if(newList.IsEmpty())
				break;
			
			//vérifie si la liste n'est pas déjà existent.
			if(EditListManager::getInstance()->exist(newList))
			{
				wxMessageDialog dlg(this, _("The list already exist!"), _("List exist"), wxOK|wxICON_EXCLAMATION|wxCENTRE);
				dlg.ShowModal();
				
				continue;
			}
			
			//Récupération des langue.
			wxString lgsrc;
			wxString lgto;
			dlg.getlanguages(&lgsrc, &lgto);
			
			//On crée une nouvelle liste.
			if(!EditListManager::getInstance()->createAndAddList(newList, lgsrc, lgto))
			{
				wxMessageDialog dlg(this, _("A problem is occured, the list can't add!"), _("List corrupt"), wxOK|wxICON_INFORMATION|wxCENTRE);
				dlg.ShowModal();
				
				continue;
			}
			
			//Un nouveau item
			newItem.Add(newList);
			newItem.Add(Resource::getInstance()->abbreviationToLanguage(lgsrc));
			newItem.Add(Resource::getInstance()->abbreviationToLanguage(lgto));
		
			//Une liste à ajouter;
			_newLists.Add(newList);
		}
		break;
	}
	
	return newItem;
}

// *********************************************************************
// Class DialogPreferences
// *********************************************************************

DialogPreferences::DialogPreferences()
: GuiDialogPreferences(nullptr)
{	
    //Magnifier 
    _staticTextGeneral->SetLabelMarkup("<b>"+_("General :")+"</b>");
    _staticTextVolumeTts->SetLabelMarkup("<b>"+_("Volume for text to speech (Say a text) :")+"</b>");
	_staticTextShutdown->SetLabelMarkup("<b>"+_("Shutdown this application :")+"</b>");
	
	//Initialise les valeurs
	_checkBoxShowMenu->SetValue(Resource::getInstance()->getShowMenu());
	_checkBoxPowerOn->SetValue(Resource::getInstance()->getPowerOn());
	_sliderTts->SetValue(Resource::getInstance()->getTtsVolume()*100);
	
	//init EditActionManager
	EditActionManager::getInstance()->init();
	//init EditListManager
	EditListManager::getInstance()->init();
	
	//Ajout du panel Action
	_PanelListActions = new PanelListActions(_notebook);
	_notebook->AddPage(_PanelListActions, _("Actions"), false);
	
	//Ajout du panel List
	_PanelListLists = new PanelListLists(_notebook);
	_notebook->AddPage(_PanelListLists, _("Lists"), false);
}

DialogPreferences::~DialogPreferences()
{
	//Destruction de EditActionManager
	EditActionManager::kill();
	//Destruction de EditListManager
	EditListManager::kill();
}

bool DialogPreferences::shutdownIsToggle()const
{
	return _toggleBtnTurnOff->GetValue();
}

void DialogPreferences::applyAndSave()
{
	//On ouvre le fichier de config.
	wxFileConfig fileConfig(	PROJECT_NAME,
								wxEmptyString,
								wxStandardPaths::Get().GetUserDataDir()+'/'+PROJECT_NAME);
	fileConfig.DeleteAll();
	
	//Affectation des valeurs dans les ressources.
	Resource::getInstance()->setShowMenu(_checkBoxShowMenu->GetValue());
	Resource::getInstance()->setPowerOn(_checkBoxPowerOn->GetValue());
	Resource::getInstance()->setTtsVolume(_sliderTts->GetValue()/100.);
	
	//On sauvegarde les ressources.
	Resource::getInstance()->save(fileConfig);
	
	//On sauvegarde EN PREMIER les listes
	EditListManager::getInstance()->apply();
	ListManager::getInstance()->save(fileConfig);
	
	//On sauvegarde EN DEUXIÈME les actions
	EditActionManager::getInstance()->apply();
	ActionManager::getInstance()->save(fileConfig);
}

void DialogPreferences::OnNotebookPageChanging(wxNotebookEvent& event)
{
	//Récupération du nom de la page.
	wxString notebookPage = _notebook->GetPageText(event.GetSelection());
	
	//Mise a jour de la page sélectionner
	if(notebookPage == _("Actions"))
		_PanelListActions->update();
	else if(notebookPage == _("Lists"))
		_PanelListLists->update();
}

void DialogPreferences::OnButtonClickOK(wxCommandEvent& event)
{	
	applyAndSave();
	event.Skip();
}

void DialogPreferences::OnButtonClickApply(wxCommandEvent& event)
{
	applyAndSave();
	event.Skip();
}
		
