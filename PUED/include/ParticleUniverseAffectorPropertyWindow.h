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

#ifndef __PUED_AFFECTOR_PROPERTY_WINDOW_H__
#define __PUED_AFFECTOR_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseAffector.h"

// Property names
static wxString PRNL_AFFECTOR_TYPE = wxT("");
static wxString PRNL_AFFECTOR_ENABLED = wxT("");
static wxString PRNL_AFFECTOR_POSITION = wxT("");
static wxString PRNL_AFFECTOR_MASS = wxT("");
static wxString PRNL_AFFECTOR_SPECIALISATION = wxT("");
static wxString PRNL_AFFECTOR_SPEC_DEFAULT = wxT("");
static wxString PRNL_AFFECTOR_SPEC_TT_INCREASE = wxT("");
static wxString PRNL_AFFECTOR_SPEC_TT_DECREASE = wxT("");

/**	
*/
class AffectorPropertyWindow : public PropertyWindow
{
	public:
		AffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);
		AffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow);
		~AffectorPropertyWindow(void){};

		/**
			Copy basic affector properties from a window to an affector
		*/
		virtual void copyAttributeToAffector(wxPGProperty* prop, wxString propertyName);

		/**
			Copy basic affector properties from an affector to a window
		*/
		virtual void copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector);

		/**
			Copy basic affector properties from another affector property window
		*/
		virtual void copyAttributesFromPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow);

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
		void replaceAffectorType(wxPGProperty* prop);

		/**
			Determine selected affector type (take internationalisation into account)
		*/
		const Ogre::String& getAffectorTypeByProperty(wxPGProperty* prop);

	protected:
		wxArrayString mTypes;
		wxArrayString mSpecialisation;

		/**
			Initialises all affector properties
		*/
		virtual void _initProperties(void);

		/**
			Validation
		*/
		bool validateTextureCoord(wxPGProperty* prop, ParticleUniverse::ParticleAffector* affector);
};

#endif

