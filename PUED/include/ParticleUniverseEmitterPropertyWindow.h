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

#ifndef __PUED_EMITTER_PROPERTY_WINDOW_H__
#define __PUED_EMITTER_PROPERTY_WINDOW_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseEmitter.h"

// Property names
static wxString PRNL_EMITTER_TYPE = wxT("");
static wxString PRNL_EMITTER_ENABLED = wxT("");
static wxString PRNL_EMITTER_POSITION = wxT("");
static wxString PRNL_EMITTER_KEEP_LOCAL = wxT("");
static wxString PRNL_EMITTER_DIRECTION = wxT("");
static wxString PRNL_EMITTER_ORIENTATION = wxT("");
static wxString PRNL_EMITTER_ORIENTATION_START = wxT("");
static wxString PRNL_EMITTER_ORIENTATION_END = wxT("");
static wxString PRNL_EMITTER_VELOCITY = wxT("");
static wxString PRNL_EMITTER_DURATION = wxT("");
static wxString PRNL_EMITTER_REPEAT_DELAY = wxT("");
static wxString PRNL_EMITTER_ANGLE = wxT("");
static wxString PRNL_EMITTER_EMISSION_RATE = wxT("");
static wxString PRNL_EMITTER_TIME_TO_LIVE = wxT("");
static wxString PRNL_EMITTER_MASS = wxT("");
static wxString PRNL_EMITTER_TEXTURE_COORD = wxT("");
static wxString PRNL_EMITTER_TEXTURE_COORD_START = wxT("");
static wxString PRNL_EMITTER_TEXTURE_COORD_END = wxT("");
static wxString PRNL_EMITTER_COLOUR = wxT("");
static wxString PRNL_EMITTER_COLOUR_RANGE_START = wxT("");
static wxString PRNL_EMITTER_COLOUR_RANGE_END = wxT("");
static wxString PRNL_EMITTER_ALL_PARTICLE_DIM = wxT("");
static wxString PRNL_EMITTER_PARTICLE_WIDTH = wxT("");
static wxString PRNL_EMITTER_PARTICLE_HEIGHT = wxT("");
static wxString PRNL_EMITTER_PARTICLE_DEPTH = wxT("");
static wxString PRNL_EMITTER_AUTO_DIRECTION = wxT("");
static wxString PRNL_EMITTER_FORCE_EMISSION = wxT("");

/**	
*/
class EmitterPropertyWindow : public PropertyWindow
{
	public:
		EmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name);
		EmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow);
		~EmitterPropertyWindow(void){};

		/**
			Copy basic emitter properties from a window to an emitter
		*/
		virtual void copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName);

		/**
			Copy basic emitter properties from an emitter to a window
		*/
		virtual void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter);

		/**
			Copy basic emitter properties from another emitter property window
		*/
		virtual void copyAttributesFromPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow);

		/**
			See PropertyWindow
		*/
		virtual void onPropertyChanged(wxPropertyGridEvent& event);
		void onParentPropertyChanged(wxPropertyGridEvent& event);

		/**
			Validates the value of an entered texture coordinate.
		*/
		bool validateTextureCoord(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);

		/**
			Updating functions
		*/
		void replaceEmitterType(wxPGProperty* prop);
		void copyEnabledToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyPositionToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyKeepLocalToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyDirectionToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyOrientationToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyOrientationRangeStartToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyOrientationRangeEndToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyVelocityToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyDurationToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyRepeatDelayToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyAngleToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyEmissionRateToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyTimeToLiveToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyMassToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyTextureCoordinateToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyTextureCoordinateStartToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyTextureCoordinateEndToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyColourToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyColourRangeStartToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyColourRangeEndToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyAllParticleDimensionsToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyParticleWidthToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyParticleHeightToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyParticleDepthToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyAutoDirectionToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);
		void copyForceEmissionToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter);

		/**
			Determine selected emitter type (take internationalisation into account)
		*/
		const Ogre::String& getEmitterTypeByProperty(wxPGProperty* prop);

	protected:
		wxArrayString mTypes;
		wxArrayString mEmitsParticleTypes;

		/**
			Initialises all emitter properties
		*/
		virtual void _initProperties(void);

		/**
			Destroy connections that are not needed in case of a type-switch
		*/
		void notifyDestroyUnnecessaryConnections(void);
};

#endif

