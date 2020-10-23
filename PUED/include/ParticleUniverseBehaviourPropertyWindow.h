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

#ifndef __PUED_BEHAVIOUR_PROPERTY_WINDOW_H__
#define __PUED_BEHAVIOUR_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseBehaviour.h"

// Property names
static wxString PRNL_BEHAVIOUR_TYPE = wxT("");

/**	
*/
class BehaviourPropertyWindow : public PropertyWindow
{
	public:
		BehaviourPropertyWindow(wxWindow* parent, EditComponent* owner);
		BehaviourPropertyWindow(BehaviourPropertyWindow* behaviourPropertyWindow);
		~BehaviourPropertyWindow(void){};

		/**
			Copy basic behaviour properties from a window to an behaviour
		*/
		virtual void copyAttributeToBehaviour(wxPGProperty* prop, wxString propertyName);

		/**
			Copy basic behaviour properties from an behaviour to a window
		*/
		virtual void copyAttributesFromBehaviour(const ParticleUniverse::ParticleBehaviour* behaviour);

		/**
			Copy basic behaviour properties from another behaviour property window
		*/
		virtual void copyAttributesFromPropertyWindow(BehaviourPropertyWindow* behaviourPropertyWindow);

		/**
			Only handle parent properties if they are changed.
		*/
		void onParentPropertyChanged(wxPropertyGridEvent& event);

		/**
			See PropertyWindow
		*/
		virtual void onPropertyChanged(wxPropertyGridEvent& event);

		/**
			Updating function
		*/
		void replaceBehaviourType(wxPGProperty* prop);

		/**
			Determine selected behaviour type (take internationalisation into account)
		*/
		const Ogre::String& getBehaviourTypeByProperty(wxPGProperty* prop);

	protected:
		wxArrayString mTypes;

		/**
			Initialises all behaviour properties
		*/
		virtual void _initProperties(void);
};

#endif

