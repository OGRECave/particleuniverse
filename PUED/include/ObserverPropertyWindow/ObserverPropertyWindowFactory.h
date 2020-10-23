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

#ifndef __PUED_OBSERVER_PROPERTY_FACTORY_H__
#define __PUED_OBSERVER_PROPERTY_FACTORY_H__

#include "wx/ogre/prerequisites.h"

#include "ParticleUniverseObserverPropertyWindow.h"

#include "ObserverPropertyWindow/ObserverPropertyWindowFactory.h"
#include "ObserverPropertyWindow/OnClearObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnCollisionObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnCountObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnEmissionObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnEventFlagObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnExpireObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnPositionObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnQuotaObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnRandomObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnTimeObserverPropertyWindow.h"
#include "ObserverPropertyWindow/OnVelocityObserverPropertyWindow.h"

/**	
*/
class ObserverPropertyWindowFactory
{
	public:
		/**
			Constructor / Destructor
		*/
		ObserverPropertyWindowFactory(void)
		{
			CST_OBSERVER_ON_CLEAR = ogre2wxTranslate(OBSERVER_ON_CLEAR);
			CST_OBSERVER_ON_COLLISION = ogre2wxTranslate(OBSERVER_ON_COLLISION);
			CST_OBSERVER_ON_COUNT = ogre2wxTranslate(OBSERVER_ON_COUNT);
			CST_OBSERVER_ON_EMISSION = ogre2wxTranslate(OBSERVER_ON_EMISSION);
			CST_OBSERVER_ON_EVENTFLAG = ogre2wxTranslate(OBSERVER_ON_EVENTFLAG);
			CST_OBSERVER_ON_EXPIRE = ogre2wxTranslate(OBSERVER_ON_EXPIRE);
			CST_OBSERVER_ON_POSITION = ogre2wxTranslate(OBSERVER_ON_POSITION);
			CST_OBSERVER_ON_QUOTA = ogre2wxTranslate(OBSERVER_ON_QUOTA);
			CST_OBSERVER_ON_RANDOM = ogre2wxTranslate(OBSERVER_ON_RANDOM);
			CST_OBSERVER_ON_TIME = ogre2wxTranslate(OBSERVER_ON_TIME);
			CST_OBSERVER_ON_VELOCITY = ogre2wxTranslate(OBSERVER_ON_VELOCITY);
		};
		~ObserverPropertyWindowFactory(void){};

		/**
			Create a specific Observer Property Window
		*/
		ObserverPropertyWindow* createObserverPropertyWindow(
			wxWindow* parent, 
			EditComponent* owner, 
			const Ogre::String& name,
			ComponentSubType subType)
		{
			if (subType == CST_UNDEFINED)
			{
				return new ObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_CLEAR)
			{
				return new OnClearObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_COLLISION)
			{
				return new OnCollisionObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_COUNT)
			{
				return new OnCountObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_EMISSION)
			{
				return new OnEmissionObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_EVENTFLAG)
			{
				return new OnEventFlagObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_EXPIRE)
			{
				return new OnExpireObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_POSITION)
			{
				return new OnPositionObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_QUOTA)
			{
				return new OnQuotaObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_RANDOM)
			{
				return new OnRandomObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_TIME)
			{
				return new OnTimeObserverPropertyWindow(parent, owner, name);
			}
			else if (subType == CST_OBSERVER_ON_VELOCITY)
			{
				return new OnVelocityObserverPropertyWindow(parent, owner, name);
			}
			else
			{
				return new ObserverPropertyWindow(parent, owner, name);
			}
			return 0;
		};

		/**
			Create a specific Observer Property Window and propagate the attributes from the ObserverPropertyWindow
		*/
		ObserverPropertyWindow* createObserverPropertyWindow(ComponentSubType subType, ObserverPropertyWindow* observerPropertyWindow)
		{
			if (subType == CST_UNDEFINED)
			{
				return new ObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_CLEAR)
			{
				return new OnClearObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_COLLISION)
			{
				return new OnCollisionObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_COUNT)
			{
				return new OnCountObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_EMISSION)
			{
				return new OnEmissionObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_EVENTFLAG)
			{
				return new OnEventFlagObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_EXPIRE)
			{
				return new OnExpireObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_POSITION)
			{
				return new OnPositionObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_QUOTA)
			{
				return new OnQuotaObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_RANDOM)
			{
				return new OnRandomObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_TIME)
			{
				return new OnTimeObserverPropertyWindow(observerPropertyWindow);
			}
			else if (subType == CST_OBSERVER_ON_VELOCITY)
			{
				return new OnVelocityObserverPropertyWindow(observerPropertyWindow);
			}
			else
			{
				return new ObserverPropertyWindow(observerPropertyWindow);
			}
			return 0;
		};
};

#endif

