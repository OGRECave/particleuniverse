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

#ifndef __PUED_LIGHT_RENDERER_PROPERTY_WINDOW_H__
#define __PUED_LIGHT_RENDERER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseRendererPropertyWindow.h"
#include "ParticleRenderers/ParticleUniverseLightRenderer.h"

// Property names
static wxString PRNL_LIGHT_TYPE;
static wxString LT_POINT;
static wxString LT_SPOT;
static wxString PRNL_ATT_RANGE;
static wxString PRNL_ATT_CONSTANT;
static wxString PRNL_ATT_LINEAR;
static wxString PRNL_ATT_QUADRATIC;
static wxString PRNL_SPOT_INNER_ANGLE;
static wxString PRNL_SPOT_OUTER_ANGLE;
static wxString PRNL_FALLOFF;
static wxString PRNL_POWER_SCALE;
static wxString PRNL_FLASH_FREQUENCY;
static wxString PRNL_FLASH_LENGTH;
static wxString PRNL_FLASH_RANDOM;

/**	
*/
class LightRendererPropertyWindow : public RendererPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		LightRendererPropertyWindow(wxWindow* parent, EditComponent* owner);

		/**
			Constructor: Pass a renderer window; its settings will be copied.
		*/
		LightRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow);

		/**
			Constructor: Pass a renderer; its settings will be copied.
		*/
		LightRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::LightRenderer* renderer);

		/**
			Destructor
		*/
		~LightRendererPropertyWindow(void){};

		/**
			See RendererPropertyWindow
		*/
		void _notifyStart(void);

		/**
			See RendererPropertyWindow
		*/
		virtual void copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName);
		
		/**
			See RendererPropertyWindow
		*/
		virtual void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer);

	protected:
		wxArrayString mLightTypes;

		/**
			Initialises all Light renderer properties
		*/
		virtual void _initProperties(void);

};

#endif

