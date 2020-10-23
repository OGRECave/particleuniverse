/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_MATERIAL_LISTBOX_H__
#define __PUED_MATERIAL_LISTBOX_H__

#include "wx/ogre/prerequisites.h"

class MaterialTab;

/**	Material Listbox: Listbox with material names, used in the Material Tab
*/
class MaterialListBox : public wxListBox
{
	public:

		// Constructor / Destructor
		MaterialListBox(MaterialTab* materialTab,
			wxWindow* parent, 
			wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize);
		~MaterialListBox(void);

		/**	Fill listbox with all material names
		*/
		void loadMaterials(void);

		/**	Select a material
		*/
		void OnMaterialsClick(wxCommandEvent& event);

		/**	Add a material name
		*/
		void addMaterialName(wxString& materialName);

	protected:
		MaterialTab* mMaterialTab;
};

#endif
