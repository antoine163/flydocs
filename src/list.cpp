//! \file **************************************************************
//! \brief Source List
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 0.14
//! \date 02.05.2013
//!
//! ********************************************************************

/*
*	Copyright © 2013 - Antoine Maleyrie.
*/

#include "list.hpp"

#include <wx/filefn.h> 
#include <vector>

//TEST
#include <iostream>

// *********************************************************************
// Class List
// *********************************************************************

List::List()
{
	_init = false;
}

List::~List()
{
}

bool List::init(	wxFileName const& fileName,
					wxString const& lgsrc,
					wxString const& lgto)
{
	_fileName = fileName;
	_lgsrc = lgsrc;
	_lgto = lgto;	
	_init = false;
	
	//Problème si les langes ne son pas renseigner et que le fichier existe.
	if(!wxFileExists(_fileName.GetFullPath()) &&
		(lgsrc.IsEmpty() || lgsrc.IsEmpty()))
	{
		clear();
		_init = false;
	}
	else
	{			
		//Création du fichier ou parse.
		_init = openFile();
		closeFile();
	}
	
	return _init;
}

bool List::clear()
{
	_firstLine->Clear();
	_knowledges->clear();
	_fileName->Clear();
	_lastModificationFile->ResetTime();
	_lgsrc->Clear();
	_lgto->Clear();
	_init = false;
}

void List::getlanguages(wxString* lgsrc, wxString* lgto)
{
	*lgsrc = _lgsrc;
	*lgto = _lgto;
}

int List::exist(wxString text)
{
	//Caractère en minuscule.
	text.MakeLower();
	
	//Ouverture du fichier.
	if(!openFile())
		return -1;
			
	//Le texte est existent ?
	if(getTextLine(text) != 0)
	{
		//Fermeture du fichier
		closeFile();
		
		return 1;
	}
	
	//Fermeture du fichier
	closeFile();
		
	return 0;
}

int List::save(	wxString const& text,
				wxString const& mainTranslate)
{	
	std::map<wxString, wxArrayString> empty;
	return save(text, mainTranslate, empty);
}
			
int List::save(	wxString text,
				wxString mainTranslate,
				std::map<wxString, wxArrayString> const& translations)
{		
	if(!openFile())
		return -1;
			
	//Le texte est existent ?
	if(getTextLine(text) != 0)
	{
		//Fermeture du fichier
		closeFile();
		
		return 0;
	}
	
	//Caractère en minuscule.
	text.MakeLower();
	mainTranslate.MakeLower();
	
	//Les ligne à ajouter au fichier
	std::vector<wxString> newLignes;
	
	//Il y a des traductions. 
	if(translations.size())
	{
		//Détermine le nombre de ligne.
		//Et ajout de nouvelle valeur a la première ligne au besoin.
		size_t nbLine = 0;
		size_t tmpNbLine = 0;
		for(auto it: translations)
		{
			tmpNbLine = it.second.GetCount();
			if(nbLine < tmpNbLine)
				nbLine = tmpNbLine;
				
			//On vérifie si le type du mot et connue dans la première ligne.
			if(_firstLine.Index(it.first, false) == wxNOT_FOUND)
			{
				//Si il n'existe pas on l'ajoute.
				_firstLine.Add(it.first);
			}
		}
		
		//Obtenir le nombre de colonne
		//(avec les nouvelles colonnes qui doive être ajouter au besoin).
		size_t nbColumn = _firstLine.GetCount();
		
		//Parcoure tout les lignes de la traduction.
		for(size_t l = 0; l < nbLine; l++)
		{
			//Création de la première ligne de la traduction.
			if(l == 0)
				newLignes.push_back("1,"+text+","+mainTranslate);
			else
				newLignes.push_back(",,");

			//Parcoure les colonnes.
			for(size_t c = 3; c < nbColumn; c++)
			{
				newLignes[l] << ',';

				if(translations.count(_firstLine[c]) > 0)
				{
					auto tr = translations.at(_firstLine[c]);
					if(l < tr.GetCount())
						newLignes[l] << tr[l];
				}
			}
		}
	}
	//Il y a que la traduction principale
	else
		newLignes.push_back("1,"+text+","+mainTranslate);

	//Réécriture de la première ligne du fichier.
	wxString addFirstLine;
	for(auto it : _firstLine)
	{
		addFirstLine << it << ',';
	}
	_file.RemoveLine(0);
	_file.InsertLine(addFirstLine, 0);
	
	//Ajout des nouvelles lignes au fichier.
	for(auto it: newLignes)
		_file.AddLine(it);

	//Écriture des données dans le fichier.
	if(!_file.Write())
	{
		//Fermeture du fichier
		closeFile();
		
		return -1;
	}
	
	//Fermeture du fichier
	closeFile();
	
	//On actualise les connaissance.
	_knowledges[KNOWLEDGE_UNKNOWN]++;
		
	return 1;
}

bool List::removeText(wxString const& text)
{
	if(!openFile())
		return false;
		
	//Récupère le numéro de la première ligne ou se trouve le texte.
	size_t textLine = getTextLine(text);
			
	//Le texte n'est pas existent ?
	if(textLine == 0)
	{
		//Fermeture du fichier
		closeFile();
		
		return false;
	}
	
	//Obtenir le nombre de ligne.
	size_t nbLine = _file.GetLineCount();

	//Suppression de tout les lignes constituent le texte.
	wxString line;
	while(1)
	{
		//Supprime la ligne.
		_file.RemoveLine(textLine);
		nbLine--;
		
		//On quitte la boucle si il n'y a plus de ligne.
		if(nbLine <= textLine)
			break;
		
		//Obtenir le texte de la ligne.
		line = _file.GetLine(textLine);
		
		//On quitte la boucle si on est arriver au texte suivent.
		if(!line.BeforeFirst(',').IsEmpty())
			break;
	}
	
	
	//Écriture des données dans le fichier.
	if(!_file.Write())
	{
		//Fermeture du fichier.
		closeFile();
		
		return false;
	}
	
	//Fermeture du fichier.
	closeFile();
	
	return true;
}

bool List::getText(	size_t index,
					Knowledge_e* knowledge,
					wxString* text,
					wxString* mainTranslate,
					std::map<wxString, wxArrayString>* translations)
{
	//On ouvre le fichier.
	if(!openFile())
		return false;
		
	long nb = -1;
	unsigned int i = 0;
	bool find = false;
	bool firstLineText = true;
	wxString level;
	
	//Lire tout le fichier ligne par ligne.
	_file.GetFirstLine();
	for(wxString line = _file.GetNextLine(); !_file.Eof(); line = _file.GetNextLine())
	{
		//Extraction de la connaissance.
		level = line.BeforeFirst(',');
		//Si il y à une connaissance.
		if(!level.IsEmpty())
		{
			//Si l'index a été trouvée précédemment.
			//Ceci veut dire que l'on a terminée.
			if(find)
				break;
			
			level.ToLong(&nb);
			
			if(knowledge == nullptr)
				i++;
			else if(*knowledge == KNOWLEDGE_ALL || (Knowledge_e)nb == *knowledge)
				i++;
			
			//Si on à trouver l'index.
			if(i == index)
			{
				if(knowledge != nullptr)
					*knowledge = (Knowledge_e)nb;
					
				find = true;
			}
		}
		
		//Si l'index a été trouvée précédemment.
		if(find)
		{
			size_t iColumn = 0;
			wxString afterFirstComma = line.AfterFirst(',');
			

			//On boucle tend que l'on a par parcourue tout les colonne de la ligne.
			while(!afterFirstComma.IsEmpty())
			{				
				iColumn++;
				
				switch(iColumn)
				{
					//Colonne texte
					case 1:					
						if(text != nullptr && firstLineText)
							*text = afterFirstComma.BeforeFirst(',');
					break;
					
					//Colonne traduction principale
					case 2:
						if(mainTranslate != nullptr && firstLineText)
							*mainTranslate = afterFirstComma.BeforeFirst(',');
					break;
					
					//Les autres traductions
					default:
						wxString beforeComma = afterFirstComma.BeforeFirst(',');
						
						if(translations != nullptr && !beforeComma.IsEmpty())
							(*translations)[_firstLine[iColumn]].Add(beforeComma);
					break;
				}
				
				//Suppression d'une colonne.
				afterFirstComma = afterFirstComma.AfterFirst(',');
			}
			
			firstLineText = false;
		}
	}
		
	//Fermeture du fichier
	closeFile();
	
	return find;
}

bool List::setKnowledge(wxString const& text, Knowledge_e knowledge)
{
	if(!openFile())
		return false;
		
	//Récupère le numéro de la première ligne ou se trouve le texte.
	size_t textLine = getTextLine(text);
			
	//Le texte n'est pas existent ?
	if(textLine == 0)
	{
		//Fermeture du fichier
		closeFile();
		
		return false;
	}

	//Récupéré la première ligne du texte.
	wxString line = _file.GetLine(textLine);
	
	//Création de la nouvelle ligne avec la connaissance.
	wxString newLing;
	newLing << (unsigned int)knowledge << ',';
	newLing << line.AfterFirst(',');
	
	//Remplace la ligne par la nouvelle.
	_file.RemoveLine(textLine);
	_file.InsertLine(newLing, textLine);
	
	//Écriture des données dans le fichier.
	if(!_file.Write())
	{
		//Fermeture du fichier.
		closeFile();
		
		return false;
	}
	
	//Fermeture du fichier.
	closeFile();
	
	return true;
}

size_t List::getNumberTextByKnowledge(Knowledge_e level)
{
	//On ouvre le fichier. Se qui aura pour effet d'analyser si il y a besoin.
	if(!openFile())
		return 0;
	//Fermeture du fichier
	closeFile();
	
	//Si le niveau de connaissance existe on retourne le nombre ...
	if(_knowledges.count(level) > 0)
		return _knowledges[level];
	
	return 0;
}

size_t List::getNumberText()
{
	size_t nb = 0;
	
	//On ouvre le fichier. Se qui aura pour effet d'analyser si il y a besoin.
	if(!openFile())
		return 0;
	//Fermeture du fichier
	closeFile();
	
	//Additionne le nombre de texte par connaissance.
	for(auto it: _knowledges)
		nb += it.second;
		
	return nb;
}

size_t List::getNumberKnowledge()
{
	//On ouvre le fichier. Se qui aura pour effet d'analyser si il y a besoin.
	if(!openFile())
		return 0;
	//Fermeture du fichier
	closeFile();
	
	return _knowledges.size();
}

void List::removeFile()
{
	//Récupération du non du fichier.
	wxString fileName = _fileName.GetFullPath();
	
	//Si le fichier est existent, on le supprime.
	if(wxFileExists(fileName))
		wxRemoveFile(fileName);
}

Knowledge_e List::stringToKnowledge(wxString const& knowledge)
{
	if(knowledge == "unknown")
		return KNOWLEDGE_UNKNOWN;
	if(knowledge == "little known")
		return KNOWLEDGE_LITTLE_KNOWN;
	if(knowledge == "known")
		return KNOWLEDGE_KNOWN;
	if(knowledge == "very known")
		return KNOWLEDGE_VERY_KNOWN;
		
	return KNOWLEDGE_ALL;
}

wxString List::knowledgeToString(Knowledge_e knowledge)
{
	if(knowledge == KNOWLEDGE_UNKNOWN)
		return "unknown";
	if(knowledge == KNOWLEDGE_LITTLE_KNOWN)
		return "little known";
	if(knowledge == KNOWLEDGE_KNOWN)
		return "known";
	if(knowledge == KNOWLEDGE_VERY_KNOWN)
		return "very known";
		
	return wxEmptyString;
}

bool List::openFile()
{
	//le non de fichier est valide ?
	if(!_fileName.IsOk())
		return false;

	//Si le fichier n'est pas déjà existent.
	if(!wxFileExists(_fileName.GetFullPath()))
	{
		//On le crée.
		if(!_file.Create(_fileName.GetFullPath()))
			return false;
		//On ajout la première ligne.
		_file.AddLine("knowledge,"+_lgsrc+','+_lgto);

		_firstLine.Add("knowledge");
		_firstLine.Add(_lgsrc);
		_firstLine.Add(_lgto);
		
		if(!_file.Write())
		{
			//Si erreur fermeture du fichier.
			closeFile();
			return false;
		}
	}
	//Le fichier existe.
	else
	{		
		//On ouvre le fichier.
		if(!_file.Open(_fileName.GetFullPath()))
			return false;
		
		// On parse le fichier si c'est la premier foi que'on l'ouvre ou si il y 
		// a besoin de le re parse. (éventuellement si le fichier a été
		// accéder par un autre programme par exemple)
		if(!_init || _lastModificationFile < _fileName.GetModificationTime())
		{
			if(!parseFirstLine())
				return false;
			parseKnowledge();
		}
	}
	
	return true;
}

void List::closeFile()
{
	_file.Close();
	
	//Mise à jour de la date de modification.
	if(_fileName.IsOk())
		_lastModificationFile = _fileName.GetModificationTime();
}

bool List::parseFirstLine()
{
	//On supprime l'analyse précédant.
	_firstLine.Clear();
	
	//On analyse la première ligne
	wxString firstLine = _file.GetFirstLine();
	wxString beforeComma;
	for(size_t i = 0; i<firstLine.Length(); i++)
	{
		if(firstLine[i] == ',')
		{
			_firstLine.Add(beforeComma);
			beforeComma.Clear();
		}
		else
			beforeComma << firstLine[i];
	}
	if(!beforeComma.IsEmpty())
		_firstLine.Add(beforeComma);
		
	//Si la premier ligne ne contient pas 3 colonnes il y a un problème.
	if(_firstLine.GetCount() >= 3)
		return false;
		
	//Au besoin on extrais les langes utilisés.
	if(_lgsrc.IsEmpty() && _lgto.IsEmpty())
	{
		_lgsrc = _firstLine[1];
		_lgto = _firstLine[2];
	}
	
	//Vérifie la cohérence entre les langes parser et celle de la class.
	if(_firstLine[1] != _lgsrc || _firstLine[2] != _lgto)
		return false;
	
	return true;
}

void List::parseKnowledge()
{
	//On supprime l'analyse précédant.
	_knowledges.clear();
	
	//Lire tout le fichier ligne par ligne.
	_file.GetFirstLine();
	long nb;
	wxString level;
	for(wxString line = _file.GetNextLine(); !_file.Eof(); line = _file.GetNextLine())
	{
		//Extraction de la connaissance.
		level = line.BeforeFirst(',');
		if(!level.IsEmpty())
		{
			level.ToLong(&nb);
			_knowledges[(Knowledge_e)nb]++;
		}
	}
}

size_t List::getTextLine(wxString text)
{
	//Caractère en minuscule.
	text.MakeLower();

	//On recherche si le texte existe (avent la deuxième ',').
	wxString line = _file.GetFirstLine();
	for(line = _file.GetNextLine(); !_file.Eof(); line = _file.GetNextLine())
	{		
		//Texte trouver ?
		if(line.AfterFirst(',').BeforeFirst(',') == text)
		{
			return _file.GetCurrentLine();
		}
	}

	return 0;
}
