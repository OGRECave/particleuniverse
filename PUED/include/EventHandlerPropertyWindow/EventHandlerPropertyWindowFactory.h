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

#ifndef __PUED_EVENT_HANDLER_PROPERTY_FACTORY_H__
#define __PUED_EVENT_HANDLER_PROPERTY_FACTORY_H__

#include "wx/ogre/prerequisites.h"
#include "EventHandlerPropertyWindow/DoAffectorEventHandlerPropertyWindow.h"
#include "EventHandlerPropertyWindow/DoEnableComponentEventHandlerPropertyWindow.h"
#include "EventHandlerPropertyWindow/DoExpireEventHandlerPropertyWindow.h"
#include "EventHandlerPropertyWindow/DoFreezeEventHandlerPropertyWindow.h"
#include "EventHandlerPropertyWindow/DoPlacementParticleEventHandlerPropertyWindow.h"
#include "EventHandlerPropertyWindow/DoScaleEventHandlerPropertyWindow.h"
#include "EventHandlerPropertyWindow/DoStopSystemEventHandlerPropertyWindow.h"

/**	
*/
class EventHandlerPropertyWindowFactory
{
	public:
		/**
			Constructor / Destructor
		*/
		EventHandlerPropertyWindowFactory(void)
		{
			// Event Handlers
			CST_HANDLER_DO_AFFECTOR = ogre2wxTranslate(HANDLER_DO_AFFECTOR);
			CST_HANDLER_DO_ENABLE_COMPONENT = ogre2wxTranslate(HANDLER_DO_ENABLE_COMPONENT);
			CST_HANDLER_DO_EXPIRE = ogre2wxTranslate(HANDLER_DO_EXPIRE);
			CST_HANDLER_DO_FREEZE = ogre2wxTranslate(HANDLER_DO_FREEZE);
			CST_HANDLER_DO_PLACEMENT_PARTICLE = ogre2wxTranslate(HANDLER_DO_PLACEMENT_PARTICLE);
			CST_HANDLER_DO_SCALE = ogre2wxTranslate(HANDLER_DO_SCALE);
			CST_HANDLER_DO_STOP_SYSTEM = ogre2wxTranslate(HANDLER_DO_STOP_SYSTEM);
		};
		~EventHandlerPropertyWindowFactory(void){};

		/**
			Create a specific EventHandler Property Window
		*/
		EventHandlerPropertyWindow* createEventHandlerPropertyWindow(
			wxWindow* parent, 
			EditComponent* owner, 
			const Ogre::String& name,
			ComponentSubType subType)
		{
			if (subType == CST_UNDEFINED)
			{
				return new EventHandlerPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_HANDLER_DO_AFFECTOR)
			{
				return new DoAffectorEventHandlerPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_HANDLER_DO_ENABLE_COMPONENT)
			{
				return new DoEnableComponentEventHandlerPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_HANDLER_DO_EXPIRE)
			{
				return new DoExpireEventHandlerPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_HANDLER_DO_FREEZE)
			{
				return new DoFreezeEventHandlerPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_HANDLER_DO_PLACEMENT_PARTICLE)
			{
				return new DoPlacementParticleEventHandlerPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_HANDLER_DO_SCALE)
			{
				return new DoScaleEventHandlerPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_HANDLER_DO_STOP_SYSTEM)
			{
				return new DoStopSystemEventHandlerPropertyWindow(parent, owner, name);
			}
			else
			{
				return new EventHandlerPropertyWindow(parent, owner, name);
			}
			return 0;
		};

		/**
			Create a specific EventHandler Property Window and propagate the attributes from the EventHandlerPropertyWindow
		*/
		EventHandlerPropertyWindow* createEventHandlerPropertyWindow(ComponentSubType subType, EventHandlerPropertyWindow* eventHandlerPropertyWindow)
		{
			if (subType == CST_UNDEFINED)
			{
				return new EventHandlerPropertyWindow(eventHandlerPropertyWindow);
			}
			else if (subType == CST_HANDLER_DO_AFFECTOR)
			{
				return new DoAffectorEventHandlerPropertyWindow(eventHandlerPropertyWindow);
			}
			else if (subType == CST_HANDLER_DO_ENABLE_COMPONENT)
			{
				return new DoEnableComponentEventHandlerPropertyWindow(eventHandlerPropertyWindow);
			}
			else if (subType == CST_HANDLER_DO_EXPIRE)
			{
				return new DoExpireEventHandlerPropertyWindow(eventHandlerPropertyWindow);
			}
			else if (subType == CST_HANDLER_DO_FREEZE)
			{
				return new DoFreezeEventHandlerPropertyWindow(eventHandlerPropertyWindow);
			}
			else if (subType == CST_HANDLER_DO_PLACEMENT_PARTICLE)
			{
				return new DoPlacementParticleEventHandlerPropertyWindow(eventHandlerPropertyWindow);
			}
			else if (subType == CST_HANDLER_DO_SCALE)
			{
				return new DoScaleEventHandlerPropertyWindow(eventHandlerPropertyWindow);
			}
			else if (subType == CST_HANDLER_DO_STOP_SYSTEM)
			{
				return new DoStopSystemEventHandlerPropertyWindow(eventHandlerPropertyWindow);
			}
			else
			{
				return new EventHandlerPropertyWindow(eventHandlerPropertyWindow);
			}
			return 0;
		};
};

#endif

