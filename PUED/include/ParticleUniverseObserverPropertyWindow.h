/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_OBSERVER_PROPERTY_WINDOW_H__
#define __PUED_OBSERVER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseObserver.h"

// Property names
static wxString PRNL_OBSERVER_TYPE = wxT("");
static wxString PRNL_OBSERVER_ENABLED = wxT("");
static wxString PRNL_UNTIL_EVENT = wxT("");
static wxString PRNL_OBSERVE_INTERVAL = wxT("");

/**	
*/
class ObserverPropertyWindow : public PropertyWindow
{
	public:
		ObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);
		ObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow);
		~ObserverPropertyWindow(void){};

		/**
			Copy basic observer properties from a window to an observer
		*/
		virtual void copyAttributeToObserver(wxPGProperty* prop, wxString propertyName);

		/**
			Copy basic observer properties from an observer to a window
		*/
		virtual void copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer);

		/**
			Copy basic observer properties from another observer property window
		*/
		virtual void copyAttributesFromPropertyWindow(ObserverPropertyWindow* observerPropertyWindow);

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
		void replaceObserverType(wxPGProperty* prop);

		/**
			Updating parent
		@remarks
			This is not out-of-the-box in PU.
		*/
		void copyHandlersTo(ParticleUniverse::ParticleObserver* oldObserver, ParticleUniverse::ParticleObserver* newObserver);

		/**
			Determine selected observer type (take internationalisation into account)
		*/
		const Ogre::String& getObserverTypeByProperty(wxPGProperty* prop);


	protected:
		wxArrayString mTypes;
		wxArrayString mParticleTypes;

		/**
			Initialises all observer properties
		*/
		virtual void _initProperties(void);
};

#endif

