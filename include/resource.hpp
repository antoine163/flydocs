//! \file **************************************************************
//! \brief Header ressource.
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 0.16
//! \date 30.03.2013
//!
//! ********************************************************************

/*
*	Copyright © 2013 - Antoine Maleyrie.
*/

#ifndef RESOURCE_H
#define RESOURCE_H

#include "singleton.hpp"

#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/buffer.h>
#include <wx/fileconf.h>
#include <map>

#if defined(__UNIX__)
#include <gst/gst.h>
#elif defined(__WXMSW__)
#endif

// *********************************************************************
// Class Resource
// *********************************************************************

//! \brief Cette classe offre plusieurs ressources et trucs plus ou mou utiles.
class Resource : public Singleton<Resource>
{	
	friend class Singleton<Resource>;
	
	public:
		//! \brief Obtenir la liste des langues avec et leur acronymes <acronyme, langue>.
		std::map<wxString, wxString> const& getLanguages()const;
		//! \brief Obtenir la langue en fonction de sont acronyme.
		wxString abbreviationToLanguage(wxString const& abbreviation)const;
		//! \brief Obtenir l'acronyme en fonction de sa langue.
		wxString languageToAbbreviation(wxString const& language)const;
		
		//! \brief Obtenir la liste des actions.
		std::map<wxString, wxString> const& getActions()const;
		//! \brief Obtenir le non de l'action en fonctionne de nom de type.
		wxString typeToAction(wxString const& actTypeName)const;
		//! \brief Obtenir le nom du type de l'action a partir de son nom.
		wxString actionsToType(wxString const& actionName)const;
		
		//! \brief Obtenir le text se trouvent dans la presse papier.
		//! Sous Unix la presse papier "Primary" est utiliser.
		//! Sous Windows une demande de copie dans la presse papier est
		//! demander à la fenêtre active avent de retourner sont contenue.
		//! Tous pour simuler la presse papier "Primary" des systèmes unix.
		static wxString getClipboard();
		
		//! \brief Obtenir la traduction d'un texte.
		//! \param translations
		//! \param text
		//! \param lgsrc
		//! \param lgto
		//! \return 
		static wxString getTranslations(
						std::map<wxString, wxArrayString>* translations,
						wxString const& text,
						wxString const& lgsrc,
						wxString const& lgto);
											
		void downloadFromUrl(wxMemoryBuffer* buffer, wxString const& sUrl);
		
		void Tts(wxString const& text, wxString const& lg);
		
		void setTtsVolume(double volume);
		double getTtsVolume();
		
		void setShowMenu(bool showMenu);
		bool getShowMenu();
		
		void setPowerOn(bool powerOn);
		bool getPowerOn();
		
		void load(wxFileConfig& fileConfig);
		void save(wxFileConfig& fileConfig)const;
		
	
	private:
		Resource();
		~Resource();
		
		//! \brief Liste des acronyme et langues <abbreviation, langue>.
		std::map<wxString, wxString> _languages;
		//! \brief Liste des nom d'action en fonction de leur nom de type <actionName, actTypeName>.
		std::map<wxString, wxString> _actions;
		
		bool _showMenu;
		bool _powerOn;
		double _ttsVolume;
		
		#if defined(__UNIX__)
			GstElement* _pipeline;
		#elif defined(__WXMSW__)
		#endif
};

#endif //RESOURCE_H
