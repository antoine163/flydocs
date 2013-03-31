//! \file **************************************************************
//! \brief Source Dialogue pour les préférences d'une action et sont
//! raccourci associer.
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 1.1
//! \date 02.01.2013
//!
//! ********************************************************************

/*
*	Copyright © 2013 - Antoine Maleyrie.
*/

#include "dialogActionPreferences.hpp"
#include "resource.hpp"
#include "actionManager.hpp"

//TEST
#include <iostream>

// *********************************************************************
// Class DialogActionPreferences
// *********************************************************************

DialogActionPreferences::DialogActionPreferences(wxWindow* parent)
: GuiDialogActionPreferences(parent)
{
	//Initialisation des variable
	_keyCtrlIsPressed = false;
	_keyAltIsPressed = false;
	_keyShiftIsPressed = false;
	_keySuperIsPressed = false;
	
	_shortKeyIsValide = false;
	
	//Ajout des actions dans les chois.
	std::vector<wxString> const& actions = Resource::getInstance()->getActions();	
	for(size_t i = 0; i<actions.size(); i++)
	{
		_choiceAction->Append(actions[i]);
	}
	//Sélectionner la une action par défaut.
	_choiceAction->SetSelection(0);
}

DialogActionPreferences::DialogActionPreferences(	wxWindow* parent,
													ShortcutKey const& inShortcutKey,
													Action const& inAct)
: GuiDialogActionPreferences(parent)
{
	//copie de l'action
	_action = ActionManager::newAction(inAct);
	
	//Initialisation des variable
	_keyCtrlIsPressed = false;
	_keyAltIsPressed = false;
	_keyShiftIsPressed = false;
	_keySuperIsPressed = false;
	
	_shortKeyIsValide = true;
	
	//Ajout des actions dans la liste des chois.
	std::vector<wxString> const& actions = Resource::getInstance()->getActions();	
	for(size_t i = 0; i<actions.size(); i++)
	{
		_choiceAction->Append(actions[i]);
	}
	//Sélectionner la bonne action.
	int n = _choiceAction->FindString(_action->getName());
	_choiceAction->SetSelection(n);
	
	//Affiche du raccourci.
	_textCtrlChortcut->SetValue(ShortcutKey::shortcutKeyToString(inShortcutKey));
	_textCtrlChortcut->SetForegroundColour(wxNullColour);
	
	//Affiche de la description de l'action
	_staticTextDescription->SetLabel(_action->getDescription());
	
	//Ajout du panne d'édition propre à l'action.
	_bSizerActPreference->Add(_action->getPanelPreferences(this), 1, wxEXPAND | wxALL, 5);

	//
	GetSizer()->Fit( this );
}

DialogActionPreferences::~DialogActionPreferences()
{
	delete _action;
}

void DialogActionPreferences::OnKeyDown(wxKeyEvent& event)
{
	//Mise a jour des touches modifier
	#if defined(__UNIX__)
	switch(event.GetRawKeyFlags())
	#elif defined(__WXMSW__)
	switch(event.GetRawKeyCode())
	#endif
	{
		case RAW_KEY_CODE_MODIFIER_CONTROL:
		_keyCtrlIsPressed = true;
		break;
		case RAW_KEY_CODE_MODIFIER_ALT:
		_keyAltIsPressed = true;
		break;
		case RAW_KEY_CODE_MODIFIER_SHIFT:
		_keyShiftIsPressed = true;
		break;
		case RAW_KEY_CODE_MODIFIER_SUPER:
		_keySuperIsPressed = true;
		break;
	}
	
	//Mise à jour du texte.
	_shortKeyIsValide = false;
	_textCtrlChortcut->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
	updateTextCtrlChortcut(event.GetUnicodeKey());
}

void DialogActionPreferences::OnKeyUp(wxKeyEvent& event)
{
	//Mise à jour des touches modifier
	#if defined(__UNIX__)
	switch(event.GetRawKeyFlags())
	#elif defined(__WXMSW__)
	switch(event.GetRawKeyCode())
	#endif
	{
		case RAW_KEY_CODE_MODIFIER_CONTROL:
		_keyCtrlIsPressed = false;
		break;
		case RAW_KEY_CODE_MODIFIER_ALT:
		_keyAltIsPressed = false;
		break;
		case RAW_KEY_CODE_MODIFIER_SHIFT:
		_keyShiftIsPressed = false;
		break;
		case RAW_KEY_CODE_MODIFIER_SUPER:
		_keySuperIsPressed = false;
		break;
	}
	
	//Mise à jour du texte.
	updateTextCtrlChortcut(event.GetUnicodeKey());
}

void DialogActionPreferences::updateTextCtrlChortcut(wxChar key)
{
	//Temps que le raccourci n'est pas valide, on met à jour le texte.
	if(!_shortKeyIsValide)
	{
		wxString strShortcut;

		if(_keyCtrlIsPressed)
			strShortcut << "ctrl";
		if(_keyAltIsPressed)
		{
			if(!strShortcut.IsEmpty())
				strShortcut << "+";
				
			strShortcut << "alt";
		}
		if(_keyShiftIsPressed)
		{
			if(!strShortcut.IsEmpty())
				strShortcut << "+";
				
			strShortcut << "shift";
		}
		if(_keySuperIsPressed)
		{
			if(!strShortcut.IsEmpty())
				strShortcut << "+";
				
			strShortcut << "super";
		}
			
		if(!strShortcut.IsEmpty() && key)
		{
			strShortcut << "+" << key;
			_shortKeyIsValide = true;
			_textCtrlChortcut->SetForegroundColour(wxNullColour);
			
			strShortcut.MakeLower();
			_textCtrlChortcut->SetValue(strShortcut);
		}
		else if(strShortcut.IsEmpty())
			_textCtrlChortcut->SetValue(_("Press a key modifier and a key"));
		else
		{
			strShortcut.MakeLower();
			_textCtrlChortcut->SetValue(strShortcut);
		}
	}
}

void DialogActionPreferences::OnLeftDown(wxMouseEvent&)
{
	_shortKeyIsValide = false;
	_textCtrlChortcut->SetForegroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
	_textCtrlChortcut->SetValue(_("Press a key modifier and a key"));
	_textCtrlChortcut->SetFocus();
}

void DialogActionPreferences::OnKillFocus(wxFocusEvent&)
{
	if(!_shortKeyIsValide)
		_textCtrlChortcut->SetValue(_("Click here"));
}

void DialogActionPreferences::OnChoiceAction(wxCommandEvent& event)
{
}