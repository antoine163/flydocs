//! \file **********************************************************************
//! \brief Header ManCache
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 0.1
//! \date 19.11.2013
//!
//! ****************************************************************************

#ifndef MAN_CACHE_H
#define MAN_CACHE_H

//App
#include "manager.hpp"
#include "cache.hpp"

//WxWidgets
#include <wx/string.h>
#include <wx/language.h>

// *****************************************************************************
// Class ManCache
// *****************************************************************************

//! \ingroup manager
//! \brief ManCache
class ManCache : public Manager
{		
	DECLARE_MANAGER(ManCache);
	
	public:		
		virtual WinManager* newEditWindow(wxWindow* parent);
		
		Cache getCache(	wxLanguage const& lgsrc,
						wxLanguage const& lgto);
						
		Cache getCache(	wxString const& name);
		
		//! \brief Obtenir le nom de touts les caches.
		//! \return le nom de tout les caches.
		wxArrayString getNameCaches()const;
		
		void workToTmp(bool toTmp = true, bool apply = true);
		
	private:
		virtual void manLoad(wxFileConfig& fileConfig);
		virtual void manSave(wxFileConfig& fileConfig)const;
		
		bool _workInTmp;
};

#endif //MAN_CACHE_H
