//! \file **************************************************************
//! \brief Source Action de traduction.
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 0.13
//! \date 17.03.2013
//!
//! ********************************************************************

/*
*	Copyright © 2013 - Antoine Maleyrie.
*/

#include "action/actTranslation.hpp"
#include "resource.hpp"
#include "notification.hpp"

//TEST
#include <iostream>

// *********************************************************************
// Class PanelActTranslation
// *********************************************************************

PanelActTranslation::PanelActTranslation(wxWindow* parent, wxButton* buttonOK, ActTranslation * act)
: GuiPanelActTranslation(parent), _act(act), _buttonOK(buttonOK)
{
	std::map<wxString, wxString> const& languages = Resource::getInstance()->getLanguages();	
	
	//Ajout des langues.
	for(auto &it: languages)
	{
		_choiceLanguageSource->Append(it.second);
		_choiceLanguageOfTranslation->Append(it.second);
	}

	//Sélectionne les bonnes langues.
	int n = _choiceLanguageSource->FindString(languages.at(_act->_lgsrc));
	_choiceLanguageSource->SetSelection(n);
	n = _choiceLanguageOfTranslation->FindString(languages.at(_act->_lgto));
	_choiceLanguageOfTranslation->SetSelection(n);
	
	//Lier l'événement du bouton OK du wxWindow parent.
	_buttonOK->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &PanelActTranslation::OnOKButtonClick, this, _buttonOK->GetId());
}

PanelActTranslation::~PanelActTranslation()
{
	_buttonOK->Unbind(wxEVT_COMMAND_BUTTON_CLICKED, &PanelActTranslation::OnOKButtonClick, this, _buttonOK->GetId());
}

void PanelActTranslation::OnOKButtonClick(wxCommandEvent& event)
{
	//Affect le langage source.
	int n = _choiceLanguageSource->GetSelection();
	wxString s = _choiceLanguageSource->GetString(n);
	_act->_lgsrc = Resource::getInstance()->languageToAbbreviation(s);
	
	//Affect le langage de destination.
	n = _choiceLanguageOfTranslation->GetSelection();
	s = _choiceLanguageOfTranslation->GetString(n);
	_act->_lgto = Resource::getInstance()->languageToAbbreviation(s);
	
	//Propage l'événement.
	event.Skip();
}


// *********************************************************************
// Class ActTranslation
// *********************************************************************

//! \todo a implémenter avec les locales.
ActTranslation::ActTranslation()
: ActTranslation("en", "fr")
{
}

ActTranslation::ActTranslation(wxString const& lgsrc, wxString const& lgto)
: Action(_("Translation"), "ActTranslation",
_("Translation a text with google.")),
_lgsrc(lgsrc), _lgto(lgto)
{
}

ActTranslation::~ActTranslation()
{
}
#include <wx/msgdlg.h> 
void ActTranslation::execute()
{
	//On récupère le contenue de la presse papier.
	wxString clipboard = Resource::getClipboard();
	
	//La presse papier est t'elle vide ?
	if(clipboard.IsEmpty())
	{
		//Pas de texte à traduire
		Notification::getInstance()->notify(_("Translate clipboard"), _("Sorry, nothing at translate."));
		return;
	}

	//On récupère le texte traduit
	std::map<wxString, wxArrayString> translations;
	wxString mainTranslate = Resource::getTranslations(&translations, clipboard, _lgsrc, _lgto);
	//On vérifie si une traduction existe.
	if(mainTranslate.IsEmpty())
	{
		Notification::getInstance()->notify(_("Translate clipboard"), _("Sorry, not translation."));
		return;
	}
	
	//On incrément le compteur
	_counter[clipboard]++;
	
	//On mes en forme la traduction dans un wxString
	wxString trans;
	trans << "\n==> " << mainTranslate << " (" << _counter[clipboard] << ")";
	
	for(auto &it: translations)
	{		
		trans << "\n\n- " << it.first;
		for(auto &itt: it.second)
		{
			trans << "\n\t" << itt;
		}
	}
	
	//On affiche la traduction
	Notification::getInstance()->notify(
		wxString::Format(_("Clipboard translation in %s :"), Resource::getInstance()->abbreviationToLanguage(_lgto)), trans);
}

wxPanel* ActTranslation::getPanelPreferences(wxWindow* parent, wxButton* buttonOK)
{
	return new PanelActTranslation(parent, buttonOK, this);
}

void ActTranslation::actLoad(wxFileConfig & fileConfig)
{
	//On récupère les préférence.
	fileConfig.Read("lgsrc", &_lgsrc);
	fileConfig.Read("lgto", &_lgto);
}
		
void ActTranslation::actSave(wxFileConfig & fileConfig)const
{
	fileConfig.Write("lgsrc", _lgsrc);
	fileConfig.Write("lgto", _lgto);
}

wxString ActTranslation::getStringPreferences()const
{
	return 	Resource::getInstance()->abbreviationToLanguage(_lgsrc) +
			' ' + _("to") + ' ' +
			Resource::getInstance()->abbreviationToLanguage(_lgto);
}
