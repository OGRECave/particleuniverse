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

#ifndef __PUED_ALIGN_BEHAVIOUR_PROPERTY_WINDOW_H__
#define __PUED_ALIGN_BEHAVIOUR_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseBehaviourPropertyWindow.h"
#include "ParticleBehaviours/ParticleUniverseSlaveBehaviour.h"

// Property names

/**	
*/
class SlaveBehaviourPropertyWindow : public BehaviourPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window and the owner component
		*/
		SlaveBehaviourPropertyWindow(wxWindow* parent, EditComponent* owner);

		/**
			Constructor: Pass an behaviour window; its settings will be copied.
		*/
		SlaveBehaviourPropertyWindow(BehaviourPropertyWindow* behaviourPropertyWindow);

		/**
			Constructor: Pass an behaviour; its settings will be copied.
		*/
		SlaveBehaviourPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::SlaveBehaviour* behaviour);

		/**
			Destructor
		*/
		~SlaveBehaviourPropertyWindow(void){};

		/**
			See BehaviourPropertyWindow
		*/
		virtual void copyAttributeToBehaviour(wxPGProperty* prop, wxString propertyName);
		
		/**
			See BehaviourPropertyWindow
		*/
		virtual void copyAttributesFromBehaviour(const ParticleUniverse::ParticleBehaviour* behaviour);

	protected:
		/**
			Initialises all box behaviour properties
		*/
		virtual void _initProperties(void);

};

#endif

