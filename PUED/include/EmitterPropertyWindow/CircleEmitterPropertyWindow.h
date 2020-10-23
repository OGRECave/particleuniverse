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

#ifndef __PUED_CIRCLE_EMITTER_PROPERTY_WINDOW_H__
#define __PUED_CIRCLE_EMITTER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseEmitterPropertyWindow.h"
#include "ParticleEmitters/ParticleUniverseCircleEmitter.h"

// Property names
static wxString PRNL_CIRCLE_EMITTER_RADIUS = wxT("");
static wxString PRNL_CIRCLE_EMITTER_STEP = wxT("");
static wxString PRNL_CIRCLE_EMITTER_ANGLE = wxT("");
static wxString PRNL_CIRCLE_EMITTER_RANDOM = wxT("");
static wxString PRNL_CIRCLE_EMITTER_NORMAL = wxT("");

/**	
*/
class CircleEmitterPropertyWindow : public EmitterPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		CircleEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);

		/**
			Constructor: Pass an emitter window; its settings will be copied.
		*/
		CircleEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow);

		/**
			Constructor: Pass an emitter; its settings will be copied.
		*/
		CircleEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::CircleEmitter* emitter);

		/**
			Destructor
		*/
		~CircleEmitterPropertyWindow(void){};

		/**
			See EmitterPropertyWindow
		*/
		virtual void copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName);

		/**
			See EmitterPropertyWindow
		*/
		virtual void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter);

		/**
			Updating functions
		*/
		void copyAngleToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter);
		void copyNormalToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter);
		void copyRadiusToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter);
		void copyRandomToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter);
		void copyStepToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter);

	protected:
		/**
			Initialises all Circle emitter properties
		*/
		virtual void _initProperties(void);

};

#endif

