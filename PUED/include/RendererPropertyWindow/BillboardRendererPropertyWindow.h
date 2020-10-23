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

#ifndef __PUED_BILLBOARD_RENDERER_PROPERTY_WINDOW_H__
#define __PUED_BILLBOARD_RENDERER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseRendererPropertyWindow.h"
#include "ParticleRenderers/ParticleUniverseBillboardRenderer.h"

// Property names
static wxString PRNL_BILLBOARD_TYPE;
static wxString BBT_POINT;
static wxString BBT_ORIENTED_COMMON;
static wxString BBT_ORIENTED_SELF;
static wxString BBT_PERPENDICULAR_COMMON;
static wxString BBT_PERPENDICULAR_SELF;
static wxString BBT_ORIENTED_SHAPE;
static wxString PRNL_ORIGIN;
static wxString ORG_TOP_LEFT;
static wxString ORG_TOP_CENTER;
static wxString ORG_TOP_RIGHT;
static wxString ORG_CENTER_LEFT;
static wxString ORG_CENTER_RIGHT;
static wxString ORG_CENTER;
static wxString ORG_BOTTOM_LEFT;
static wxString ORG_BOTTOM_RIGHT;
static wxString ORG_BOTTOM_CENTER;
static wxString PRNL_ROTATION_TYPE;
static wxString RT_VERTEX;
static wxString RT_TEXTURE_COORDINATES;
static wxString PRNL_COMMON_DIRECTION;
static wxString PRNL_UP_VECTOR;
static wxString PRNL_POINT_RENDERING;
static wxString PRNL_ACCURATE_FACING;
static wxString ACF_ON;
static wxString ACF_OFF;

/**	
*/
class BillboardRendererPropertyWindow : public RendererPropertyWindow
{
	public:
		/**
			Constructor: Pass a parent window, the owner component and name
		*/
		BillboardRendererPropertyWindow(wxWindow* parent, EditComponent* owner);

		/**
			Constructor: Pass a renderer window; its settings will be copied.
		*/
		BillboardRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow);

		/**
			Constructor: Pass a renderer; its settings will be copied.
		*/
		BillboardRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::BillboardRenderer* renderer);

		/**
			Destructor
		*/
		~BillboardRendererPropertyWindow(void){};

		/**
			See RendererPropertyWindow
		*/
		virtual void copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName);
		
		/**
			See RendererPropertyWindow
		*/
		virtual void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer);

	protected:
		wxArrayString mBillboardTypes;
		wxArrayString mOrigin;
		wxArrayString mRotationTypes;
		wxArrayString mAccurateFacing;

		/**
			Initialises all billboard renderer properties
		*/
		virtual void _initProperties(void);

};

#endif

