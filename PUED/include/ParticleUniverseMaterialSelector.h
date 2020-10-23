/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_MATERIAL_SELECTOR_H__
#define __PUED_MATERIAL_SELECTOR_H__

#include "ParticleUniversePropertyWindow.h"

/**	Selection dialog for materials
*/
class MaterialSelector : public wxSingleChoiceDialog
{
	public:
		// Constructor / Destructor
		MaterialSelector(wxWindow* parent, 
			const wxString& message, 
			const wxString& caption, 
			int n, 
			const wxString* choices, 
			long style = wxCHOICEDLG_STYLE, 
			const wxPoint& pos = wxDefaultPosition) : 
		wxSingleChoiceDialog(parent, message, caption, n, choices, 0, style, pos){};
		~MaterialSelector(void){};
};

/**	Class that creates a button property. After clicking a material can be selected
*/
class MaterialProperty : public wxStringPropertyClass
{
	public:
		MaterialProperty(
			const wxString& label, 
			const wxString& name);
		virtual ~MaterialProperty(void){};

		/**	
		*/
		virtual bool OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event);

		/**	Returns the selected materialname
		*/
		const Ogre::String& getMaterialName(void) const;

	protected:
		Ogre::String mMaterialName;
};

/**	Class that displays a material selection dialog
*/
class MaterialDialog
{
	public:
		MaterialDialog(void){};
		virtual ~MaterialDialog(void){};

		/**	Open the dialog
		*/
		const Ogre::String& openDialog (wxWindow* parent);

		/**	Returns the selected material name
		*/
		const Ogre::String& getMaterialName(void) const;

	protected:
		Ogre::String mMaterialName;
};

#endif
