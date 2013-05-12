//! \file **************************************************************
//! \brief Source Gestion des listes
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 0.7
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
		List* tmpList = new List();
		if(tmpList->init(getPath()+'/'+tmpNameList, tmplgsrc, tmplgto))
		{
			//Si l'init est ok, on l'ajout.
			add(tmpNameList, tmpList);
		}
		else
			delete tmpList;
		
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

//! \todo à modifier
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
	//Création du répertoire temporaire.
	wxDir::Make(EditListManager::getPath(),  wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		
	//Récupération des listes.
	auto lists = ListManager::getInstance()->getData();
	
	wxString tmplgsrc;
	wxString tmplgto;
		
	//Copie de tout les listes.
	for(auto it : lists)
	{
		//Si le fichier de la liste existe.
		if(wxFileExists(ListManager::getPath()+'/'+it.first))
		{
			//Copie la liste dans le dossier temporaire.
			wxCopyFile(	ListManager::getPath()+'/'+it.first,
						EditListManager::getPath()+'/'+it.first);
		}
							
		//Récupération des langues
		it.second->getlanguages(&tmplgsrc, &tmplgto);
		
		//Création et initialisation d'une nouvelle liste.
		List* tmpList = new List();
		if(tmpList->init(EditListManager::getPath()+'/'+it.first, tmplgsrc, tmplgto))
		{
			//Si l'init est ok, on l'ajout.
			add(it.first, tmpList);
		}
		else
			delete tmpList;
	}
}

void EditListManager::apply()
{	
	//Destruction du dossier des liste utilisateur.
	wxDir::Remove(ListManager::getPath(), wxPATH_RMDIR_RECURSIVE);
	
	//Création du dossier des liste utilisateur.
	wxDir::Make(ListManager::getPath(),  wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	
	wxString tmplgsrc;
	wxString tmplgto;
		
	//Copie de tout les listes.
	for(auto it : _data)
	{
		//Si le fichier de la liste existe.
		if(wxFileExists(EditListManager::getPath()+'/'+it.first))
		{
			//Copie la liste dans le dossier temporaire.
			wxCopyFile(	EditListManager::getPath()+'/'+it.first,
						ListManager::getPath()+'/'+it.first);
		}
							
		//Récupération des langues
		it.second->getlanguages(&tmplgsrc, &tmplgto);
		
		//Création et initialisation d'une nouvelle liste.
		List* tmpList = new List();
		if(tmpList->init(ListManager::getPath()+'/'+it.first, tmplgsrc, tmplgto))
		{
			//Si l'init est ok, on l'ajout.
			ListManager::getInstance()->add(it.first, tmpList);
		}
		else
			delete tmpList;
	}
}

wxString EditListManager::getPath()
{
	return wxStandardPaths::Get().GetTempDir()+"/"+PROJECT_NAME;
}

wxArrayString EditListManager::getNameLists()const
{
	wxArrayString tmpArrayString;
	
	//Parcoure des lites.
	//for(auto it: _data)
	//{
		////Ajout de la liste au wxArrayString.
		//tmpArrayString.Add(it.first);
	//}
	
	return tmpArrayString;
}

wxArrayString EditListManager::getNameListsByLanguages(	wxString const& lgsrc,
													wxString const& lgto)const
{
	wxArrayString tmpArrayString;
	
	////Parcoure des lites.
	//for(auto it: _lists)
	//{
		////Si les lange son équivalente.
		//if(it->_lgsrc == lgsrc && it->_lgto == lgto)
		//{
			////Ajout de la liste au wxArrayString.
			//tmpArrayString.Add(it->getName());
		//}
	//}
	
	return tmpArrayString;
}
