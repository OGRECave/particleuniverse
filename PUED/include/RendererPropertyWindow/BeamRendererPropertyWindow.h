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

#ifndef __PUED_BEAM_RENDERER_PROPERTY_WINDOW_H__
#define __PUED_BEAM_RENDERER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseRendererPropertyWindow.h"
#include "ParticleRenderers/ParticleUniverseBeamRenderer.h"

// Property names
static wxString PRNL_UPDATE_INTERVAL = wxT("");
static wxString PRNL_DEVIATION = wxT("");
static wxString PRNL_NUMBER_OF_SEGMENTS = wxT("");
static wxString PRNL_JUMP = wxT("");
static wxString PRNL_TEXTURE_DIRECTION = wxT("");
static wxString TEXTURE_DIRECTION_U = wxT("");
static wxString TEXTURE_DIRECTION_V = wxT("");

/**	
*/
class BeamRendererPropertyWindow : public RendererPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		BeamRendererPropertyWindow(wxWindow* parent, EditComponent* owner);

		/**
			Constructor: Pass a renderer window; its settings will be copied.
		*/
		BeamRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow);

		/**
			Constructor: Pass a renderer; its settings will be copied.
		*/
		BeamRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::BeamRenderer* renderer);

		/**
			Destructor
		*/
		~BeamRendererPropertyWindow(void){};

		/**
			See RendererPropertyWindow
		*/
		virtual void copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName);
		
		/**
			See RendererPropertyWindow
		*/
		virtual void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer);

	protected:
		wxArrayString mTextureDirection;

		/**
			Initialises all Beam renderer properties
		*/
		virtual void _initProperties(void);

};

#endif

