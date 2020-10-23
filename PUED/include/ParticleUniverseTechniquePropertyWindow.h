/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_TECHNIQUE_PROPERTY_WINDOW_H__
#define __PUED_TECHNIQUE_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseTechnique.h"

// Property names
static wxString PRNL_TECHNIQUE_ENABLED = wxT("");
static wxString PRNL_TECHNIQUE_POSITION = wxT("");
static wxString PRNL_TECHNIQUE_KEEP_LOCAL = wxT("");
static wxString PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA = wxT("");
static wxString PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA = wxT("");
static wxString PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA = wxT("");
static wxString PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA = wxT("");
static wxString PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA = wxT("");
static wxString PRNL_TECHNIQUE_MATERIAL = wxT("");
static wxString PRNL_TECHNIQUE_LOD_INDEX = wxT("");
static wxString PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH = wxT("");
static wxString PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT = wxT("");
static wxString PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH = wxT("");
static wxString PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION = wxT("");
static wxString PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP = wxT("");
static wxString PRNL_TECHNIQUE_SPHASHING_SIZE = wxT("");
static wxString PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL = wxT("");
static wxString PRNL_TECHNIQUE_MAX_VELOCITY = wxT("");

/**	
*/
class TechniquePropertyWindow : public PropertyWindow
{
	public:
		TechniquePropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);
		~TechniquePropertyWindow(void){};

		/**
			Copy technique properties from a particle technique to a window
		*/
		virtual void copyAttributesFromTechnique(const ParticleUniverse::ParticleTechnique* technique);

		/**
			Copy technique properties from a window to a technique
		*/
		virtual void copyAttributeToTechnique(wxPGProperty* prop, wxString propertyName);

		/**
			See PropertyWindow
		*/
		virtual void onPropertyChanged(wxPropertyGridEvent& event);
		void onParentPropertyChanged(wxPropertyGridEvent& event);

		/**
			Updating functions
		*/
		void copyEnabledToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique);
		void copyPositionToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique);
		void copyKeepLocalToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique);
		void copyVisualParticleQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique);
		void copyEmittedEmitterQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique);
		void copyEmittedAffectorQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique);
		void copyEmittedTechniqueQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique);
		void copyEmittedSystemQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique);

	protected:
		/**
			Initialises all technique properties
		*/
		virtual void _initProperties(void);
};

#endif
