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

#ifndef __PUED_GRAVITY_AFFECTOR_PROPERTY_WINDOW_H__
#define __PUED_GRAVITY_AFFECTOR_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseAffectorPropertyWindow.h"
#include "ParticleAffectors/ParticleUniverseGravityAffector.h"

// Property names
static wxString PRNL_GRAVITY = wxT("");

/**	
*/
class GravityAffectorPropertyWindow : public AffectorPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		GravityAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);

		/**
			Constructor: Pass an affector window; its settings will be copied.
		*/
		GravityAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow);

		/**
			Constructor: Pass an affector; its settings will be copied.
		*/
		GravityAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::GravityAffector* affector);

		/**
			Destructor
		*/
		~GravityAffectorPropertyWindow(void){};

		/**
			See AffectorPropertyWindow
		*/
		virtual void copyAttributeToAffector(wxPGProperty* prop, wxString propertyName);
		
		/**
			See AffectorPropertyWindow
		*/
		virtual void copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector);

	protected:
		/**
			Initialises all box affector properties
		*/
		virtual void _initProperties(void);

};

#endif

