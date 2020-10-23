/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_MEDIATYPE_COMBOBOX_H__
#define __PUED_MEDIATYPE_COMBOBOX_H__

#include "wx/ogre/prerequisites.h"


/**	This is an extension of the wxComboBox class
*/
class ConfigDialog;
class MediaTypeComboBox : public wxComboBox
{
	public:
		// Constructor / Destructor
		MediaTypeComboBox(wxWindow* parent,
			ConfigDialog* dialog, 
			wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize);
		~MediaTypeComboBox(void){};

		/**	Handle event
		*/
		void OnSelectEvent(wxCommandEvent& event);

	protected:
		ConfigDialog* mDialog;
};

#endif
