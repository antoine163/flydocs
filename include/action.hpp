//04.01.2013
//v 0.4

#ifndef ACTION_H
#define ACTION_H

#include <wx/string.h>
#include <wx/fileconf.h>

//! \brief Class de base pour les actions.
//! \attention Dans les classes fille il faudra probablement prévoie un constructeur par recopie.
//! \attention Dans les classes fille il faudra prévoie une méthode de prototype -> static TypeDeAction load(wxFileConfig & fileConfig).
class Action
{
	public:		
		//! \brief Constructeur.
		Action();
		
		//! \brief destructeur.
		//! \todo à implémenter correctement avec la gestion mémoire de _actShortcutKey.
		virtual ~Action();
		
		//! \brief Permet d'exécuter l'action.
		virtual void execute()=0;
		
		//! \brief Permet de sauvegarder les préférences de l'action dans le wxFileConfig.
		virtual void sove(wxFileConfig & fileConfig)const=0;
		
		//! \brief Permet d'extraire les préférences de l'action au format string,
		//! dans le but des les affichées à l'utilisateur.
		virtual wxString getStringPreferences()const=0;
	
	private:
};

#endif //ACTION_H
