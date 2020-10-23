/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2014 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_EXTERN_PROPERTY_WINDOW_H__
#define __PUED_EXTERN_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseExtern.h"

// Property names
static wxString PRNL_EXTERN_TYPE = wxT("");
static wxString PRNL_PHYSX_COLLISION_GROUP = wxT("");

/**	
*/
class ExternPropertyWindow : public PropertyWindow
{
	public:
		ExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);
		ExternPropertyWindow(ExternPropertyWindow* externPropertyWindow);
		~ExternPropertyWindow(void){};

		/**
			Copy basic extern properties from a window to an extern
		*/
		virtual void copyAttributeToExtern(wxPGProperty* prop, wxString propertyName);

		/**
			Copy basic extern properties from an extern to a window
		*/
		virtual void copyAttributesFromExtern(ParticleUniverse::Extern* externObject);

		/**
			Copy basic extern properties from another extern property window
		*/
		virtual void copyAttributesFromPropertyWindow(ExternPropertyWindow* externPropertyWindow);

		/**
			See PropertyWindow
		*/
		virtual void onPropertyChanged(wxPropertyGridEvent& event);

		/**
			Only handle parent properties if they are changed.
		*/
		void onParentPropertyChanged(wxPropertyGridEvent& event);

		/**
			Updating function
		*/
		void replaceExternType(wxPGProperty* prop);

		/**
			Determine selected extern type (take internationalisation into account)
		*/
		const Ogre::String& getExternTypeByProperty(wxPGProperty* prop);

	protected:
		wxArrayString mTypes;
		wxArrayString mSpecialisation;

		/**
			Initialises all extern properties
		*/
		virtual void _initProperties(void);
};

#endif

