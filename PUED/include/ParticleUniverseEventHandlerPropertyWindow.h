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

#ifndef __PUED_EVENT_HANDLER_PROPERTY_WINDOW_H__
#define __PUED_EVENT_HANDLER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseEventHandler.h"

// Property names
static wxString PRNL_HANDLER_TYPE = wxT("");

/**	
*/
class EventHandlerPropertyWindow : public PropertyWindow
{
	public:
		EventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);
		EventHandlerPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow);
		~EventHandlerPropertyWindow(void){};

		/**
			Copy basic eventHandler properties from a window to an eventHandler
		*/
		virtual void copyAttributeToEventHandler(wxPGProperty* prop, wxString propertyName);

		/**
			Copy basic eventHandler properties from an eventHandler to a window
		*/
		virtual void copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler* eventHandler);

		/**
			Copy basic eventHandler properties from another eventHandler property window
		*/
		virtual void copyAttributesFromPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow);

		/**
			Replace event handler type window, depending on the new type.
		*/
		void replaceHandlerType(wxPGProperty* prop);

		/**
			See PropertyWindow
		*/
		virtual void onPropertyChanged(wxPropertyGridEvent& event);

		/**
			Only handle parent properties if they are changed.
		*/
		void onParentPropertyChanged(wxPropertyGridEvent& event);

		/**
			Determine selected event handler type (take internationalisation into account)
		*/
		const Ogre::String& getHandlerTypeByProperty(wxPGProperty* prop);

	protected:
		wxArrayString mTypes;

		/**
			Initialises all eventHandler properties
		*/
		virtual void _initProperties(void);

		/**
			Destroy connections that are not needed in case of a type-switch
		*/
		void notifyDestroyUnnecessaryConnections(void);
};

#endif

