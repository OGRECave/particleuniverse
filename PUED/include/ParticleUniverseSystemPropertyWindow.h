/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_SYSTEM_PROPERTY_WINDOW_H__
#define __PUED_SYSTEM_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseSystem.h"

// Property names
static wxString PRNL_SYSTEM_CATEGORY = wxT("");
static wxString PRNL_SYSTEM_KEEP_LOCAL = wxT("");
static wxString PRNL_SYSTEM_ITERATION_INTERVAL = wxT("");
static wxString PRNL_SYSTEM_FIXED_TIMEOUT = wxT("");
static wxString PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT = wxT("");
static wxString PRNL_SYSTEM_LOD_DISTANCES = wxT("");
static wxString PRNL_SYSTEM_SMOOTH_LOD = wxT("");
static wxString PRNL_SYSTEM_FAST_FORWARD = wxT("");
static wxString PRNL_SYSTEM_FAST_FORWARD_TIME = wxT("");
static wxString PRNL_SYSTEM_FAST_FORWARD_INTERVAL = wxT("");
static wxString PRNL_SYSTEM_MAIN_CAMERA_NAME = wxT("");
static wxString PRNL_SYSTEM_SCALE_VELOCITY = wxT("");
static wxString PRNL_SYSTEM_SCALE_TIME = wxT("");
static wxString PRNL_SYSTEM_SCALE = wxT("");
static wxString PRNL_SYSTEM_TIGHT_BOUNDING_BOX = wxT("");

class ParticleUniverseEditorFrame;
/**	
*/
class SystemPropertyWindow : public PropertyWindow
{
	public:
		SystemPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);
		~SystemPropertyWindow(void){};

		/**
			Copy basic system properties from a window to a system
		*/
		virtual void copyAttributeToSystem(wxPGProperty* prop, wxString propertyName);

		/**
			Copy system properties from a particle system to a window
		*/
		virtual void copyAttributesFromSystem(const ParticleUniverse::ParticleSystem* system);

		/**
			See PropertyWindow
		*/
		virtual void onPropertyChanged(wxPropertyGridEvent& event);

		/**
			Get/Set root frame
		*/
		ParticleUniverseEditorFrame* getRootFrame(void);
		void setRootFrame(ParticleUniverseEditorFrame* rootFrame);

	protected:
		/**
			Initialises all system properties
		*/
		virtual void _initProperties(void);

		/**
			Keep pointer to the root frame
		*/
		ParticleUniverseEditorFrame* mRootFrame;
};

#endif
