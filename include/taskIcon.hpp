//! \file **********************************************************************
//! \brief Header TaskIcon.
//! 
//! - Compilateur : GCC,MinGW
//!
//! \author Antoine Maleyrie
//! \version 0.8
//! \date 10.12.12
//!
//! ****************************************************************************

#ifndef TASK_ICON_H
#define TASK_ICON_H

//WxWidgets
#include <wx/taskbar.h>

// *****************************************************************************
// Class TaskIcon
// *****************************************************************************

//! \brief Menu dans la zone de notification.
class TaskIcon : public wxTaskBarIcon
{
	public:
		TaskIcon(wxTaskBarIconType iconType=wxTBI_DEFAULT_TYPE);
		virtual ~TaskIcon();
		
	protected:
		virtual	wxMenu* CreatePopupMenu();

	private:
};

#endif //TASK_ICON_H
