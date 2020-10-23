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

#ifndef __PUED_LINE_EMITTER_PROPERTY_WINDOW_H__
#define __PUED_LINE_EMITTER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseEmitterPropertyWindow.h"
#include "ParticleEmitters/ParticleUniverseLineEmitter.h"

// Property names
static wxString PRNL_LINE_END = wxT("");
static wxString PRNL_LINE_MAX_INCREMENT = wxT("");
static wxString PRNL_LINE_MIN_INCREMENT = wxT("");
static wxString PRNL_LINE_DEVIATION = wxT("");

/**	
*/
class LineEmitterPropertyWindow : public EmitterPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		LineEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);

		/**
			Constructor: Pass an emitter window; its settings will be copied.
		*/
		LineEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow);

		/**
			Constructor: Pass an emitter; its settings will be copied.
		*/
		LineEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::LineEmitter* emitter);

		/**
			Destructor
		*/
		~LineEmitterPropertyWindow(void){};

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
		void copyEndToLineEmitter(wxPGProperty* prop, ParticleUniverse::LineEmitter* lineEmitter);
		void copyMaxIncrementToLineEmitter(wxPGProperty* prop, ParticleUniverse::LineEmitter* lineEmitter);
		void copyMinIncrementToLineEmitter(wxPGProperty* prop, ParticleUniverse::LineEmitter* lineEmitter);
		void copyDeviationToLineEmitter(wxPGProperty* prop, ParticleUniverse::LineEmitter* lineEmitter);

	protected:
		/**
			Initialises all Line emitter properties
		*/
		virtual void _initProperties(void);

};

#endif

