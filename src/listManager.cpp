//! \file **************************************************************
//! \brief Source Gestion des listes
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 0.12
//! \date 02.05.2013
//!
//! ********************************************************************

/*
*	Copyright © 2013 - Antoine Maleyrie.
*/

#include "listManager.hpp"

#include <wx/stdpaths.h>
#include <wx/utils.h> 
#include <wx/dir.h>

//TEST
#include <iostream>

// *********************************************************************
// Class ListManagerBase
// *********************************************************************

ListManagerBase::ListManagerBase()
{
}

ListManagerBase::~ListManagerBase()
{
}

bool ListManagerBase::createAndAddList(	wxString const &listName,
										wxString const &lgsrc,
										wxString const &lgto)
{
	//Création et initialisation d'une nouvelle liste.
	List* tmpList = new List();
	if(tmpList->init(getPath()+'/'+listName, lgsrc, lgto))
	{
		//Si l'init est ok, on l'ajout.
		if(add(listName, tmpList))
			return true;
	}

	delete tmpList;
	return false;
}

wxArrayString ListManagerBase::getNameLists()const
{
	wxArrayString tmpArrayString;
	
	//Parcoure des lites.
	for(auto it: _data)
	{
		//Ajout de la liste au wxArrayString.
		tmpArrayString.Add(it.first);
	}
	
	return tmpArrayString;
}

wxArrayString ListManagerBase::getNameListsByLanguages(	wxString const& lgsrc,
														wxString const& lgto)const
{
	wxArrayString tmpArrayString;
	wxString tmplgsrc;
	wxString tmplgto;
	
	//Parcoure des lites.
	for(auto it: _data)
	{
		//Récupère les langages de la liste.
		it.second->getlanguages(&tmplgsrc, &tmplgto);
		
		//Si les lange son équivalente.
		if(tmplgsrc == lgsrc && tmplgto == lgto)
		{
			//Ajout le nom de la liste au wxArrayString.
			tmpArrayString.Add(it.first);
		}
	}
	
	return tmpArrayString;
}

// *********************************************************************
// Class ListManager
// *********************************************************************

ListManager::ListManager()
{
}

ListManager::~ListManager()
{
}

void ListManager::load(wxFileConfig& fileConfig)
{
	wxString tmpNameList;
	long lIndex;
		
	//Avent de charger quoi que se soit, on supprime tout les listes.
	removeAll();
	
	//On positionne le path.
	fileConfig.SetPath("/ListManager");
	
	//On récupère la première list.
	if(!fileConfig.GetFirstGroup(tmpNameList, lIndex))
		return;
		
	do
	{
		//On positionne le path.
		fileConfig.SetPath(tmpNameList);
		
		//Récupération des paramètres de la liste.
		wxString tmplgsrc;
		wxString tmplgto;
		fileConfig.Read("lgsrc", &tmplgsrc);
		fileConfig.Read("lgto", &tmplgto);
		
		//Création et initialisation d'une nouvelle liste.
		createAndAddList(tmpNameList, tmplgsrc, tmplgto);
		
		//On positionne le path.
		fileConfig.SetPath("../");
		
	}//Puis toutes les autres.
	while(fileConfig.GetNextGroup(tmpNameList, lIndex));
	
	//On positionne le path a la racine.
	fileConfig.SetPath("/");
}

void ListManager::save(wxFileConfig& fileConfig)const
{
	//Parcoure tout les lites.
	for(auto it: _data)
	{
		//Crée un groupe pour cette liste.
		fileConfig.SetPath("/ListManager/"+it.first);
		
		//Sauvegarde des paramètres de la liste.
		wxString lgsrc;
		wxString lgto;
		it.second->getlanguages(&lgsrc, &lgto);
		fileConfig.Write("lgsrc", lgsrc);
		fileConfig.Write("lgto", lgto);
	}
}

wxString ListManager::getPath()
{
	return wxStandardPaths::Get().GetUserDataDir()+"/lists";
}

// *********************************************************************
// Class EditListManager
// *********************************************************************

EditListManager::EditListManager()
{
}

EditListManager::~EditListManager()
{
	//Destruction du dossier temporaire.
	wxDir::Remove(EditListManager::getPath(), wxPATH_RMDIR_RECURSIVE);
}

void EditListManager::init()
{	
	//Récupération de ListManager.
	ListManager* listManager = ListManager::getInstance();
	
	//Création du répertoire temporaire.
	wxDir::Make(getPath(),  wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		
	//Récupération des listes.
	auto lists = listManager->getData();
	
	wxString tmplgsrc;
	wxString tmplgto;
		
	//Copie de tout les listes.
	for(auto it : lists)
	{
		//Si le fichier de la liste existe.
		if(wxFileExists(listManager->getPath()+'/'+it.first))
		{
			//Copie la liste dans le dossier temporaire.
			wxCopyFile(	listManager->getPath()+'/'+it.first,
						getPath()+'/'+it.first);
		}
							
		//Récupération des langues
		it.second->getlanguages(&tmplgsrc, &tmplgto);
		
		//Création et initialisation d'une nouvelle liste.
		createAndAddList(it.first, tmplgsrc, tmplgto);
	}
}

void EditListManager::apply()
{	
	//Récupération de ListManager.
	ListManager* listManager = ListManager::getInstance();
	
	//Destruction du dossier des liste utilisateur.
	wxDir::Remove(listManager->getPath(), wxPATH_RMDIR_RECURSIVE);
	
	//Création du dossier des liste utilisateur.
	wxDir::Make(listManager->getPath(),  wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	
	//On supprime tout
	listManager->removeAll();
		
	//Copie de tout les listes.
	wxString tmplgsrc;
	wxString tmplgto;
	for(auto it : _data)
	{
		//Si le fichier de la liste existe.
		if(wxFileExists(getPath()+'/'+it.first))
		{
			//Copie la liste dans le dossier temporaire.
			wxCopyFile(	getPath()+'/'+it.first,
						listManager->getPath()+'/'+it.first);
		}
							
		//Récupération des langues
		it.second->getlanguages(&tmplgsrc, &tmplgto);
		
		//Création et initialisation d'une nouvelle liste.
		listManager->createAndAddList(it.first, tmplgsrc, tmplgto);
	}
}

wxString EditListManager::getPath()
{
	return wxStandardPaths::Get().GetTempDir()+"/"+PROJECT_NAME;
}
