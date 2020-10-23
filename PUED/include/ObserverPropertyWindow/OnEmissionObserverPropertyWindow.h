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

#ifndef __PUED_ON_EMISSION_OBSERVER_PROPERTY_WINDOW_H__
#define __PUED_ON_EMISSION_OBSERVER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseObserverPropertyWindow.h"
#include "ParticleObservers/ParticleUniverseOnEmissionObserver.h"

// Property names

/**	
*/
class OnEmissionObserverPropertyWindow : public ObserverPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		OnEmissionObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);

		/**
			Constructor: Pass an Observer window; its settings will be copied.
		*/
		OnEmissionObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow);

		/**
			Constructor: Pass an observer; its settings will be copied.
		*/
		OnEmissionObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnEmissionObserver* observer);

		/**
			Destructor
		*/
		~OnEmissionObserverPropertyWindow(void){};

		/**
			See ObserverPropertyWindow
		*/
		virtual void copyAttributeToObserver(wxPGProperty* prop, wxString propertyName);
		
		/**
			See ObserverPropertyWindow
		*/
		virtual void copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer);

	protected:
		/**
			Initialises all box observer properties
		*/
		virtual void _initProperties(void);

};

#endif

