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

#ifndef __PUED_BEHAVIOUR_PROPERTY_FACTORY_H__
#define __PUED_BEHAVIOUR_PROPERTY_FACTORY_H__

#include "wx/ogre/prerequisites.h"
#include "BehaviourPropertyWindow/SlaveBehaviourPropertyWindow.h"

/**	
*/
class BehaviourPropertyWindowFactory
{
	public:
		/**
			Constructor / Destructor
		*/
		BehaviourPropertyWindowFactory(void)
		{
			// Behaviour
			CST_BEHAVIOUR_SLAVE = ogre2wxTranslate(BEHAVIOUR_SLAVE);
		};
		~BehaviourPropertyWindowFactory(void){};

		/**
			Create a specific Behaviour Property Window
		*/
		BehaviourPropertyWindow* createBehaviourPropertyWindow(
			wxWindow* parent, 
			EditComponent* owner, 
			const Ogre::String& name,
			ComponentSubType subType)
		{
			if (subType == CST_UNDEFINED)
			{
				return new BehaviourPropertyWindow(parent, owner);
			}
			else if (subType == CST_BEHAVIOUR_SLAVE)
			{
				return new SlaveBehaviourPropertyWindow(parent, owner);
			}
			else
			{
				return new BehaviourPropertyWindow(parent, owner);
			}
			return 0;
		};

		/**
			Create a specific Behaviour Property Window and propagate the attributes from the BehaviourPropertyWindow
		*/
		BehaviourPropertyWindow* createBehaviourPropertyWindow(ComponentSubType subType, BehaviourPropertyWindow* behaviourPropertyWindow)
		{
			if (subType == CST_UNDEFINED)
			{
				return new BehaviourPropertyWindow(behaviourPropertyWindow);
			}
			else if (subType == CST_BEHAVIOUR_SLAVE)
			{
				return new SlaveBehaviourPropertyWindow(behaviourPropertyWindow);
			}
			else
			{
				return new BehaviourPropertyWindow(behaviourPropertyWindow);
			}
			return 0;
		};
};

#endif

