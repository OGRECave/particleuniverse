/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_RENDERER_PROPERTY_WINDOW_H__
#define __PUED_RENDERER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseRenderer.h"

// Property names
static wxString PRNL_RENDERER_TYPE = wxT("");
static wxString PRNL_RENDERER_RENDER_Q_GROUP = wxT("");
static wxString PRNL_RENDERER_SORTING = wxT("");
static wxString PRNL_RENDERER_TEXCOORDS_DEFINE = wxT("");
static wxString PRNL_RENDERER_TEXCOORDS_SET = wxT("");
static wxString PRNL_RENDERER_TEXCOORDS_ROWS = wxT("");
static wxString PRNL_RENDERER_TEXCOORDS_COLUMNS = wxT("");
static wxString PRNL_RENDERER_USE_SOFT_PARTICLES = wxT("");
static wxString PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER = wxT("");
static wxString PRNL_RENDERER_SOFT_PARTICLES_SCALE = wxT("");
static wxString PRNL_RENDERER_SOFT_PARTICLES_DELTA = wxT("");
static wxString PRNL_USE_VERTEX_COLOURS = wxT("");
static wxString PRNL_MAX_ELEMENTS = wxT("");

/**	
*/
class RendererPropertyWindow : public PropertyWindow
{
	public:
		RendererPropertyWindow(wxWindow* parent, EditComponent* owner);
		RendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow);
		~RendererPropertyWindow(void){};

		/**
			Copy basic renderer properties from a window to a renderer
		*/
		virtual void copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName);

		/**
			Copy basic renderer properties from an renderer to a window
		*/
		virtual void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer);

		/**
			Copy basic renderer properties from another renderer property window
		*/
		virtual void copyAttributesFromPropertyWindow(RendererPropertyWindow* rendererPropertyWindow);

		/**
			See PropertyWindow
		*/
		virtual void onPropertyChanged(wxPropertyGridEvent& event);

		/**
			Only handle parent properties if they are changed.
		*/
		void onParentPropertyChanged(wxPropertyGridEvent& event);

		/**
			Updating function
		*/
		void replaceRendererType(wxPGProperty* prop);

		/**
			Determine selected render type (take internationalisation into account)
		*/
		const Ogre::String& getRendererTypeByProperty(wxPGProperty* prop);


	protected:
		wxArrayString mTypes;

		/**
			Initialises all renderer properties
		*/
		virtual void _initProperties(void);
};

#endif

