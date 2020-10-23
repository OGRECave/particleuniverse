/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEmitterPropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/ogre/utils.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
EmitterPropertyWindow::EmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) : PropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
EmitterPropertyWindow::EmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) : PropertyWindow(
	emitterPropertyWindow->GetParent(), 
	emitterPropertyWindow->getOwner(), 
	emitterPropertyWindow->getComponentName())
{
	_initProperties();
	copyAttributesFromPropertyWindow(emitterPropertyWindow);
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyAttributesFromPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow)
{
	Ogre::Vector3 v;
	Ogre::Quaternion q;
	wxColor c;

	// Name: String
	// Todo: Scan all connections of this emitter and replace the name if it is used in other components
	doSetString(PRNL_NAME, emitterPropertyWindow->doGetString(PRNL_NAME));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_EMITTER_TYPE);
	wxPGProperty* propFrom = emitterPropertyWindow->GetPropertyPtr(PRNL_EMITTER_TYPE);
	propTo->DoSetValue(propFrom->DoGetValue());

	// Enabled: Bool
	doSetBool(PRNL_EMITTER_ENABLED, emitterPropertyWindow->doGetBool(PRNL_EMITTER_ENABLED));

	// Position: Vector3
	doSetVector3(PRNL_EMITTER_POSITION, emitterPropertyWindow->doGetVector3(PRNL_EMITTER_POSITION, v));

	// Keep local: Bool
	doSetBool(PRNL_EMITTER_KEEP_LOCAL, emitterPropertyWindow->doGetBool(PRNL_EMITTER_KEEP_LOCAL));

	// Direction: Vector3
	doSetVector3(PRNL_EMITTER_DIRECTION, emitterPropertyWindow->doGetVector3(PRNL_EMITTER_DIRECTION, v));

	// Orientation: Quaternion
	doSetQuaternion(PRNL_EMITTER_ORIENTATION, emitterPropertyWindow->doGetQuaternion(PRNL_EMITTER_ORIENTATION, q));

	// Orientation range start: Quaternion
	doSetQuaternion(PRNL_EMITTER_ORIENTATION_START, emitterPropertyWindow->doGetQuaternion(PRNL_EMITTER_ORIENTATION_START, q));
	
	// Orientation range end: Quaternion
	doSetQuaternion(PRNL_EMITTER_ORIENTATION_END, emitterPropertyWindow->doGetQuaternion(PRNL_EMITTER_ORIENTATION_END, q));

	// Velocity: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_VELOCITY, emitterPropertyWindow);

	// Duration: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_DURATION, emitterPropertyWindow);

	// Repeat - Delay: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_REPEAT_DELAY, emitterPropertyWindow);

	// Emits: Is no property, because this is done by means of connections

	// Angle: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_ANGLE, emitterPropertyWindow);

	// Emission rate: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_EMISSION_RATE, emitterPropertyWindow);
	
	// Time to live: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_TIME_TO_LIVE, emitterPropertyWindow);

	// Mass: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_MASS, emitterPropertyWindow);	
	
	// Texture coordinate: Uint16
	doSetUint16(PRNL_EMITTER_TEXTURE_COORD, emitterPropertyWindow->doGetUint16(PRNL_EMITTER_TEXTURE_COORD));
	
	// Texture coordinate Start: Uint16
	doSetUint16(PRNL_EMITTER_TEXTURE_COORD_START, emitterPropertyWindow->doGetUint16(PRNL_EMITTER_TEXTURE_COORD_START));

	// Texture coordinate End: Uint16
	doSetUint16(PRNL_EMITTER_TEXTURE_COORD_END, emitterPropertyWindow->doGetUint16(PRNL_EMITTER_TEXTURE_COORD_END));

	// Colour: Colour attibute with alpha
	doSetColourWithAlpha(PRNL_EMITTER_COLOUR, emitterPropertyWindow->doGetColourWithAlpha(PRNL_EMITTER_COLOUR, c));

	// Colour range start: Colour attibute with alpha
	doSetColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_START, emitterPropertyWindow->doGetColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_START, c));

	// Colour range end: Colour attibute with alpha
	doSetColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_END, emitterPropertyWindow->doGetColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_END, c));

	// All particle dimensions: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_ALL_PARTICLE_DIM, emitterPropertyWindow);

	// Particle width: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_PARTICLE_WIDTH, emitterPropertyWindow);

	// Particle height: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_PARTICLE_HEIGHT, emitterPropertyWindow);

	// Particle depth: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_PARTICLE_DEPTH, emitterPropertyWindow);

	// Auto direction: Bool
	doSetBool(PRNL_EMITTER_AUTO_DIRECTION, emitterPropertyWindow->doGetBool(PRNL_EMITTER_AUTO_DIRECTION));

	// Force emission: Bool
	doSetBool(PRNL_EMITTER_FORCE_EMISSION, emitterPropertyWindow->doGetBool(PRNL_EMITTER_FORCE_EMISSION));
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(mOwner->getPUElement());
	if (!emitter)
		return;

	if (propertyName == PRNL_NAME)
	{
		// Name: String
		Ogre::String name = wx2ogre(prop->GetValueAsString());
		emitter->setName(name);
	}
	else if (propertyName == PRNL_EMITTER_TYPE)
	{
		// Type: List of types
		// This requires the emitter to be replaced.
		replaceEmitterType(prop);
	}
	else if (propertyName == PRNL_EMITTER_ENABLED)
	{
		// Enabled: Bool
		copyEnabledToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_POSITION + PRNL_X ||
		propertyName == PRNL_EMITTER_POSITION + PRNL_Y ||
		propertyName == PRNL_EMITTER_POSITION + PRNL_Z)
	{
		// Position: Vector3
		copyPositionToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_KEEP_LOCAL)
	{
		// Keep local: Bool
		copyKeepLocalToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_DIRECTION + PRNL_X ||
		propertyName == PRNL_EMITTER_DIRECTION + PRNL_Y ||
		propertyName == PRNL_EMITTER_DIRECTION + PRNL_Z)
	{
		// Direction: Vector3
		copyDirectionToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_ORIENTATION + PRNL_W ||
		propertyName == PRNL_EMITTER_ORIENTATION + PRNL_X ||
		propertyName == PRNL_EMITTER_ORIENTATION + PRNL_Y ||
		propertyName == PRNL_EMITTER_ORIENTATION + PRNL_Z)
	{
		// Orientation: Quaternion
		copyOrientationToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_ORIENTATION_START + PRNL_W ||
		propertyName == PRNL_EMITTER_ORIENTATION_START + PRNL_X ||
		propertyName == PRNL_EMITTER_ORIENTATION_START + PRNL_Y ||
		propertyName == PRNL_EMITTER_ORIENTATION_START + PRNL_Z)
	{
		// Orientation range start: Quaternion
		copyOrientationRangeStartToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_ORIENTATION_END + PRNL_W ||
		propertyName == PRNL_EMITTER_ORIENTATION_END + PRNL_X ||
		propertyName == PRNL_EMITTER_ORIENTATION_END + PRNL_Y ||
		propertyName == PRNL_EMITTER_ORIENTATION_END + PRNL_Z)
	{
		// Orientation range end: Quaternion
		copyOrientationRangeEndToEmitter(prop, emitter);
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_VELOCITY))
	{
		// Velocity type: Dynamic attibute
		copyVelocityToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_DURATION))
	{
		// Duration: Dynamic attibute
		copyDurationToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_REPEAT_DELAY))
	{
		// Repeat - Delay: Dynamic attibute
		copyRepeatDelayToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_ANGLE))
	{
		// Angle: Dynamic attibute
		copyAngleToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_EMISSION_RATE))
	{
		// Emission rate: Dynamic attibute
		copyEmissionRateToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_TIME_TO_LIVE))
	{
		// Time to live: Dynamic attibute
		copyTimeToLiveToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_MASS))
	{
		// Mass: Dynamic attibute
		copyMassToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName == PRNL_EMITTER_TEXTURE_COORD)
	{
		// Texture coordinate: Uint16
		copyTextureCoordinateToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_TEXTURE_COORD_START)
	{
		// Texture coordinate Start: Uint16
		copyTextureCoordinateStartToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_TEXTURE_COORD_END)
	{
		// Texture coordinate End: Uint16
		copyTextureCoordinateEndToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_COLOUR + PRNL_POST_RGB ||
		propertyName == PRNL_EMITTER_COLOUR + PRNL_POST_ALPHA)
	{
		// Colour: Colour attibute with alpha
		copyColourToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_COLOUR_RANGE_START + PRNL_POST_RGB ||
		propertyName == PRNL_EMITTER_COLOUR_RANGE_START + PRNL_POST_ALPHA)
	{
		// Colour range start: Colour attibute with alpha
		copyColourRangeStartToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_COLOUR_RANGE_END + PRNL_POST_RGB ||
		propertyName == PRNL_EMITTER_COLOUR_RANGE_END + PRNL_POST_ALPHA)
	{
		// Colour range end: Colour attibute with alpha
		copyColourRangeEndToEmitter(prop, emitter);
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_ALL_PARTICLE_DIM))
	{
		// All particle dimensions: Dynamic attibute
		copyAllParticleDimensionsToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_PARTICLE_WIDTH))
	{
		// Particle width: Dynamic attibute
		copyParticleWidthToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_PARTICLE_HEIGHT))
	{
		// Particle height: Dynamic attibute
		copyParticleHeightToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName.StartsWith(PRNL_EMITTER_PARTICLE_DEPTH))
	{
		// Particle depth: Dynamic attibute
		copyParticleDepthToEmitter(propertyName, prop, emitter);
		if (emitter->_isMarkedForEmission())
		{
			//_unprepareEmitters(emitter);
			_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
		}
	}
	else if (propertyName == PRNL_EMITTER_AUTO_DIRECTION)
	{
		// Auto direction: Bool
		copyAutoDirectionToEmitter(prop, emitter);
	}
	else if (propertyName == PRNL_EMITTER_FORCE_EMISSION)
	{
		// Force emission: Bool
		copyForceEmissionToEmitter(prop, emitter);
	}
	else
	{
		PropertyWindow::copyAttributeToComponent(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	// Name: Ogre::String
	doSetString(PRNL_NAME, ogre2wx(emitter->getName()));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_EMITTER_TYPE);
	wxString type = ogre2wxTranslate(emitter->getEmitterType());
	propTo->SetValueFromString(type);

	// Enabled: Bool
	doSetBool(PRNL_EMITTER_ENABLED, emitter->isEnabled());

	// Position: Vector3
	doSetVector3(PRNL_EMITTER_POSITION, emitter->position);

	// Keep local: Bool
	doSetBool(PRNL_EMITTER_KEEP_LOCAL, emitter->isKeepLocal());

	// Direction: Vector3
	doSetVector3(PRNL_EMITTER_DIRECTION, emitter->getOriginalParticleDirection());

	// Orientation: Quaternion
	doSetQuaternion(PRNL_EMITTER_ORIENTATION, emitter->getParticleOrientation());

	// Orientation range start: Quaternion
	doSetQuaternion(PRNL_EMITTER_ORIENTATION_START, emitter->getParticleOrientationRangeStart());
	
	// Orientation range end: Quaternion
	doSetQuaternion(PRNL_EMITTER_ORIENTATION_END, emitter->getParticleOrientationRangeEnd());

	// Velocity: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_VELOCITY, emitter->getDynVelocity());

	// Duration: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_DURATION, emitter->getDynDuration());

	// Repeat - Delay: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_REPEAT_DELAY, emitter->getDynRepeatDelay());

	// Emits: Is no property, because this is done by means of connections

	// Angle: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_ANGLE, emitter->getDynAngle());

	// Emission rate: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_EMISSION_RATE, emitter->getDynEmissionRate());
	
	// Time to live: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_TIME_TO_LIVE, emitter->getDynTotalTimeToLive());

	// Mass: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_MASS, emitter->getDynParticleMass());
	
	// Texture coordinate: Uint16
	doSetUint16(PRNL_EMITTER_TEXTURE_COORD, emitter->getParticleTextureCoords());
	
	// Texture coordinate Start: Uint16
	doSetUint16(PRNL_EMITTER_TEXTURE_COORD_START, emitter->getParticleTextureCoordsRangeStart());

	// Texture coordinate End: Uint16
	doSetUint16(PRNL_EMITTER_TEXTURE_COORD_END, emitter->getParticleTextureCoordsRangeEnd());

	// Colour: Colour attibute with alpha
	wxColor colour(255 * emitter->getParticleColour().r, 
		255 * emitter->getParticleColour().g, 
		255 * emitter->getParticleColour().b, 
		255 * emitter->getParticleColour().a);
	doSetColourWithAlpha(PRNL_EMITTER_COLOUR, colour);

	// Colour range start: Colour attibute with alpha
	wxColor colourStart(255 * emitter->getParticleColourRangeStart().r, 
		255 * emitter->getParticleColourRangeStart().g, 
		255 * emitter->getParticleColourRangeStart().b,
		255 * emitter->getParticleColourRangeStart().a);
	doSetColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_START, colourStart);

	// Colour range end: Colour attibute with alpha
	wxColor colourEnd(255 * emitter->getParticleColourRangeEnd().r, 
		255 * emitter->getParticleColourRangeEnd().g, 
		255 * emitter->getParticleColourRangeEnd().b,
		255 * emitter->getParticleColourRangeEnd().a);
	doSetColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_END, colourEnd);

	// All particle dimensions: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_ALL_PARTICLE_DIM, emitter->getDynParticleAllDimensions());

	// Particle width: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_PARTICLE_WIDTH, emitter->getDynParticleWidth());

	// Particle height: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_PARTICLE_HEIGHT, emitter->getDynParticleHeight());

	// Particle depth: Dynamic attibute
	doSetDynamicAttribute(PRNL_EMITTER_PARTICLE_DEPTH, emitter->getDynParticleDepth());

	// Auto direction: Bool
	doSetBool(PRNL_EMITTER_AUTO_DIRECTION, emitter->isAutoDirection());

	// Force emission: Bool
	doSetBool(PRNL_EMITTER_FORCE_EMISSION, emitter->isForceEmission());
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	CST_EMITTER_BOX = ogre2wxTranslate(EMITTER_BOX);
	CST_EMITTER_CIRCLE = ogre2wxTranslate(EMITTER_CIRCLE);
	CST_EMITTER_LINE = ogre2wxTranslate(EMITTER_LINE);
	CST_EMITTER_MESH_SURFACE = ogre2wxTranslate(EMITTER_MESH_SURFACE);
	CST_EMITTER_POINT = ogre2wxTranslate(EMITTER_POINT);
	CST_EMITTER_POSITION = ogre2wxTranslate(EMITTER_POSITION);
	CST_EMITTER_SLAVE = ogre2wxTranslate(EMITTER_SLAVE);
	CST_EMITTER_SPHERE = ogre2wxTranslate(EMITTER_SPHERE);
	CST_EMITTER_VERTEX = ogre2wxTranslate(EMITTER_VERTEX);
	PRNL_NAME = _("Name");
	PRNL_POST_RGB = _(" RGB");
	PRNL_POST_ALPHA = _(" Alpha");
	PRNL_W = _(".w");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_EMITTER_TYPE = _("Emitter type");
	PRNL_EMITTER_ENABLED = _("Enabled");
	PRNL_EMITTER_POSITION = _("Position");
	PRNL_EMITTER_KEEP_LOCAL = _("Keep local");
	PRNL_EMITTER_DIRECTION = _("Direction");
	PRNL_EMITTER_ORIENTATION = _("Orientation");
	PRNL_EMITTER_ORIENTATION_START = _("Orientation range start");
	PRNL_EMITTER_ORIENTATION_END = _("Orientation range end");
	PRNL_EMITTER_VELOCITY = _("Velocity");
	PRNL_EMITTER_DURATION = _("Duration");
	PRNL_EMITTER_REPEAT_DELAY = _("Repeat - Delay");
	PRNL_EMITTER_ANGLE = _("Angle");
	PRNL_EMITTER_EMISSION_RATE = _("Emission rate");
	PRNL_EMITTER_TIME_TO_LIVE = _("Time to live");
	PRNL_EMITTER_MASS = _("Mass");
	PRNL_EMITTER_TEXTURE_COORD = _("Texture coordinate");
	PRNL_EMITTER_TEXTURE_COORD_START = _("Texture coordinate start");
	PRNL_EMITTER_TEXTURE_COORD_END = _("Texture coordinate end");
	PRNL_EMITTER_COLOUR = _("Colour");
	PRNL_EMITTER_COLOUR_RANGE_START = _("Colour range start");
	PRNL_EMITTER_COLOUR_RANGE_END = _("Colour range end");
	PRNL_EMITTER_ALL_PARTICLE_DIM = _("All particle dimensions");
	PRNL_EMITTER_PARTICLE_WIDTH = _("Particle width");
	PRNL_EMITTER_PARTICLE_HEIGHT = _("Particle height");
	PRNL_EMITTER_PARTICLE_DEPTH = _("Particle depth");
	PRNL_EMITTER_AUTO_DIRECTION = _("Auto direction");
	PRNL_EMITTER_FORCE_EMISSION = _("Force emission");

	// Type: List of types (DO NOT CHANGE THE ORDER!
	// Internationalization means that the string values of this property are unusable to create an emitter)
	mTypes.Add(CST_EMITTER_BOX);
	mTypes.Add(CST_EMITTER_CIRCLE);
	mTypes.Add(CST_EMITTER_LINE);
	mTypes.Add(CST_EMITTER_MESH_SURFACE);
	mTypes.Add(CST_EMITTER_POINT);
	mTypes.Add(CST_EMITTER_POSITION);
	mTypes.Add(CST_EMITTER_SLAVE);
	mTypes.Add(CST_EMITTER_SPHERE);
	mTypes.Add(CST_EMITTER_VERTEX);
	wxPGId pid = Append(wxEnumProperty(PRNL_EMITTER_TYPE, PRNL_EMITTER_TYPE, mTypes));

	// Enabled: Bool
	SetBoolChoices (_("True"), _("False")); // Forces Internationalization
	Append(wxBoolProperty(PRNL_EMITTER_ENABLED, PRNL_EMITTER_ENABLED, ParticleUniverse::ParticleEmitter::DEFAULT_ENABLED));

	// Position: Vector3
	appendVector3(PRNL_EMITTER_POSITION, PRNL_EMITTER_POSITION, ParticleUniverse::ParticleEmitter::DEFAULT_POSITION);

	// Keep local: Bool
	Append(wxBoolProperty(PRNL_EMITTER_KEEP_LOCAL, PRNL_EMITTER_KEEP_LOCAL, ParticleUniverse::ParticleEmitter::DEFAULT_KEEP_LOCAL));

	// Direction: Vector3
	appendVector3(PRNL_EMITTER_DIRECTION, PRNL_EMITTER_DIRECTION, ParticleUniverse::ParticleEmitter::DEFAULT_DIRECTION);

	// Orientation: Quaternion
	appendQuaternion(PRNL_EMITTER_ORIENTATION, PRNL_EMITTER_ORIENTATION, ParticleUniverse::ParticleEmitter::DEFAULT_ORIENTATION);

	// Orientation range start: Quaternion
	appendQuaternion(PRNL_EMITTER_ORIENTATION_START, PRNL_EMITTER_ORIENTATION_START, ParticleUniverse::ParticleEmitter::DEFAULT_ORIENTATION_RANGE_START);

	// Orientation range end: Quaternion
	appendQuaternion(PRNL_EMITTER_ORIENTATION_END, PRNL_EMITTER_ORIENTATION_END, ParticleUniverse::ParticleEmitter::DEFAULT_ORIENTATION_RANGE_END);

	// Velocity: Dynamic attribute
	ParticleUniverse::DynamicAttributeFixed dynAttr;
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_VELOCITY);
	appendDynamicAttribute(PRNL_EMITTER_VELOCITY, PRNL_EMITTER_VELOCITY, dynAttr);

	// Duration: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_DURATION);
	appendDynamicAttribute(PRNL_EMITTER_DURATION, PRNL_EMITTER_DURATION, dynAttr);

	// Repeat - Delay: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_REPEAT_DELAY);
	appendDynamicAttribute(PRNL_EMITTER_REPEAT_DELAY, PRNL_EMITTER_REPEAT_DELAY, dynAttr);

	// Emits: Is no property, because this is done by means of connections

	// Angle: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_ANGLE);
	appendDynamicAttribute(PRNL_EMITTER_ANGLE, PRNL_EMITTER_ANGLE, dynAttr);

	// Emission rate: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_EMISSION_RATE);
	appendDynamicAttribute(PRNL_EMITTER_EMISSION_RATE, PRNL_EMITTER_EMISSION_RATE, dynAttr);

	// Time to live: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_TIME_TO_LIVE);
	appendDynamicAttribute(PRNL_EMITTER_TIME_TO_LIVE, PRNL_EMITTER_TIME_TO_LIVE, dynAttr);

	// Mass: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_MASS);
	appendDynamicAttribute(PRNL_EMITTER_MASS, PRNL_EMITTER_MASS, dynAttr);

	// Texture coordinate: Uint16
	Append(wxUIntProperty(PRNL_EMITTER_TEXTURE_COORD, PRNL_EMITTER_TEXTURE_COORD, ParticleUniverse::ParticleEmitter::DEFAULT_TEXTURE_COORDS));
	SetPropertyEditor(PRNL_EMITTER_TEXTURE_COORD, wxPG_EDITOR(SpinCtrl));

	// Texture coordinate Start: Uint16
	Append(wxUIntProperty(PRNL_EMITTER_TEXTURE_COORD_START, PRNL_EMITTER_TEXTURE_COORD_START, ParticleUniverse::ParticleEmitter::DEFAULT_TEXTURE_COORDS));
	SetPropertyEditor(PRNL_EMITTER_TEXTURE_COORD_START, wxPG_EDITOR(SpinCtrl));

	// Texture coordinate End: Uint16
	Append(wxUIntProperty(PRNL_EMITTER_TEXTURE_COORD_END, PRNL_EMITTER_TEXTURE_COORD_END, ParticleUniverse::ParticleEmitter::DEFAULT_TEXTURE_COORDS));
	SetPropertyEditor(PRNL_EMITTER_TEXTURE_COORD_END, wxPG_EDITOR(SpinCtrl));

	// Colour: Colour attibute with alpha
	appendColourWithAlphaProperty(PRNL_EMITTER_COLOUR, PRNL_EMITTER_COLOUR, wxColour(255, 255, 255, 255));

	// Colour range start: Colour attibute with alpha
	appendColourWithAlphaProperty(PRNL_EMITTER_COLOUR_RANGE_START, PRNL_EMITTER_COLOUR_RANGE_START, wxColour(0, 0, 0, 0));

	// Colour range end: Colour attibute with alpha
	appendColourWithAlphaProperty(PRNL_EMITTER_COLOUR_RANGE_END, PRNL_EMITTER_COLOUR_RANGE_END, wxColour(255, 255, 255, 255));

	// All particle dimensions: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_DIMENSIONS);
	appendDynamicAttribute(PRNL_EMITTER_ALL_PARTICLE_DIM, PRNL_EMITTER_ALL_PARTICLE_DIM, dynAttr);

	// Particle width: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_WIDTH);
	appendDynamicAttribute(PRNL_EMITTER_PARTICLE_WIDTH, PRNL_EMITTER_PARTICLE_WIDTH, dynAttr);

	// Particle height: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_HEIGHT);
	appendDynamicAttribute(PRNL_EMITTER_PARTICLE_HEIGHT, PRNL_EMITTER_PARTICLE_HEIGHT, dynAttr);

	// Particle depth: Dynamic attibute
	dynAttr.setValue(ParticleUniverse::ParticleEmitter::DEFAULT_DEPTH);
	appendDynamicAttribute(PRNL_EMITTER_PARTICLE_DEPTH, PRNL_EMITTER_PARTICLE_DEPTH, dynAttr);

	// Auto direction: Bool
	Append(wxBoolProperty(PRNL_EMITTER_AUTO_DIRECTION, PRNL_EMITTER_AUTO_DIRECTION, ParticleUniverse::ParticleEmitter::DEFAULT_AUTO_DIRECTION));

	// Force emission: Bool
	Append(wxBoolProperty(PRNL_EMITTER_FORCE_EMISSION, PRNL_EMITTER_FORCE_EMISSION, ParticleUniverse::ParticleEmitter::DEFAULT_FORCE_EMISSION));
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	wxPGProperty* prop = event.GetPropertyPtr();
	onParentPropertyChanged(event);
	copyAttributeToEmitter(prop, propertyName);
	ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(mOwner->getPUElement());
	if (emitter && emitter->_isMarkedForEmission() && emitter->getParentTechnique())
	{
		// Unprepare, to change a property of an emitted emitter
		//_unprepareEmitters(emitter);
		_unprepare(emitter, ParticleUniverse::Particle::PT_EMITTER, ParticleUniverse::Particle::PT_EMITTER);
	}
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::onParentPropertyChanged(wxPropertyGridEvent& event)
{
	// Perform additional validations.
	if (!_validatePropertyColourWithAlpha(event.GetPropertyPtr(), PRNL_EMITTER_COLOUR))
		return;
	if (!_validatePropertyColourWithAlpha(event.GetPropertyPtr(), PRNL_EMITTER_COLOUR_RANGE_START))
		return;
	if (!_validatePropertyColourWithAlpha(event.GetPropertyPtr(), PRNL_EMITTER_COLOUR_RANGE_END))
		return;

	// Allow only positive values of dynamic attributes
	if (!_validatePropertyDynamicAttribute(event.GetPropertyPtr(), PRNL_EMITTER_VELOCITY))
		return;
	if (!_validatePropertyDynamicAttribute(event.GetPropertyPtr(), PRNL_EMITTER_DURATION))
		return;
	if (!_validatePropertyDynamicAttribute(event.GetPropertyPtr(), PRNL_EMITTER_REPEAT_DELAY))
		return;
	if (!_validatePropertyDynamicAttribute(event.GetPropertyPtr(), PRNL_EMITTER_EMISSION_RATE))
		return;
	if (!_validatePropertyDynamicAttribute(event.GetPropertyPtr(), PRNL_EMITTER_TIME_TO_LIVE))
		return;
	if (!_validatePropertyDynamicAttribute(event.GetPropertyPtr(), PRNL_EMITTER_ALL_PARTICLE_DIM))
		return;
	if (!_validatePropertyDynamicAttribute(event.GetPropertyPtr(), PRNL_EMITTER_PARTICLE_WIDTH))
		return;
	if (!_validatePropertyDynamicAttribute(event.GetPropertyPtr(), PRNL_EMITTER_PARTICLE_HEIGHT))
		return;
	if (!_validatePropertyDynamicAttribute(event.GetPropertyPtr(), PRNL_EMITTER_PARTICLE_DEPTH))
		return;

	wxString propertyName = event.GetPropertyName();
	PropertyWindow::onPropertyChanged(event);

	if (propertyName == PRNL_EMITTER_TYPE)
	{
		// Replace this window by another one
		notifyDestroyUnnecessaryConnections();
		wxString subType = event.GetPropertyValueAsString();
		mOwner->createPropertyWindow(subType, this);
		mOwner->setCaption();
		getOwner()->refreshCanvas();
	}
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::notifyDestroyUnnecessaryConnections(void)
{
	// Delete unnecessary connections
	getOwner()->deleteConnection(CR_SLAVE, CRDIR_PRIMARY);
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::replaceEmitterType(wxPGProperty* prop)
{
	// Type: List of types
	Ogre::String type = getEmitterTypeByProperty(prop);
	if (type == Ogre::StringUtil::BLANK)
		return;

	ParticleUniverse::ParticleEmitter* oldEmitter = static_cast<ParticleUniverse::ParticleEmitter*>(mOwner->getPUElement());
	if (oldEmitter)
	{
		ParticleUniverse::ParticleTechnique* technique = oldEmitter->getParentTechnique();
		if (technique)
		{
			ParticleUniverse::ParticleEmitter* newEmitter = technique->createEmitter(type);
			oldEmitter->copyParentAttributesTo(newEmitter);
			bool wasStarted = false;
			ParticleUniverse::ParticleSystem* system = technique->getParentSystem();
			if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED)
			{
				wasStarted = true;
				system->stop();
			}
			technique->destroyEmitter(oldEmitter);
			mOwner->setPUElement(newEmitter);
			technique->_unprepareEmitters();
			if (wasStarted)
			{
				system->start();
			}
		}
		else
		{
			/** The old emitter didn't have a technique, so create a new emitter by means of the ParticleSystemManager itself and also delete
				the old emitter by means of the ParticleSystemManager
			*/
			ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleEmitter* newEmitter = particleSystemManager->createEmitter(type);
			oldEmitter->copyParentAttributesTo(newEmitter);
			particleSystemManager->destroyEmitter(oldEmitter);
			mOwner->setPUElement(newEmitter);
		}
	}
	else
	{
		// There is no old emitter. Create a new emitter by means of the ParticleSystemManager
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleEmitter* newEmitter = particleSystemManager->createEmitter(type);
		mOwner->setPUElement(newEmitter);
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyEnabledToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	emitter->_setOriginalEnabled(prop->DoGetValue().GetBool());
	emitter->setEnabled(prop->DoGetValue().GetBool());
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyPositionToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	Ogre::Vector3 v3;
	v3 = doGetVector3(PRNL_EMITTER_POSITION, v3);
	emitter->position = v3;
	emitter->originalPosition = v3;
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyKeepLocalToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	emitter->setKeepLocal(prop->DoGetValue().GetBool());
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyDirectionToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	Ogre::Vector3 v3;
	v3 = doGetVector3(PRNL_EMITTER_DIRECTION, v3);
	emitter->setParticleDirection(v3);
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyOrientationToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	Ogre::Quaternion q;
	q = doGetQuaternion(PRNL_EMITTER_ORIENTATION, q);
	emitter->setParticleOrientation(q);
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyOrientationRangeStartToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	Ogre::Quaternion q;
	q = doGetQuaternion(PRNL_EMITTER_ORIENTATION_START, q);
	emitter->setParticleOrientationRangeStart(q);
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyOrientationRangeEndToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	Ogre::Quaternion q;
	q = doGetQuaternion(PRNL_EMITTER_ORIENTATION_END, q);
	emitter->setParticleOrientationRangeEnd(q);
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyVelocityToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_VELOCITY, emitter->getDynVelocity());
	if (dynAttr)
	{
		emitter->setDynVelocity(dynAttr);
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyDurationToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_DURATION, emitter->getDynDuration());
	if (dynAttr)
	{
		emitter->setDynDuration(dynAttr);
	}
	if (emitter->getDynDuration()->getValue() == ParticleUniverse::ParticleEmitter::DEFAULT_DURATION)
	{
		emitter->setDynDurationSet(false); // Supressing writing this property the script and setting the emitter to the right state.
	}
	else
	{
		emitter->setDynDurationSet(true); // Force writing this property to the script and setting the emitter to the right state.
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyRepeatDelayToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_REPEAT_DELAY, emitter->getDynRepeatDelay());
	if (dynAttr)
	{
		emitter->setDynRepeatDelay(dynAttr);
	}
	emitter->setDynRepeatDelaySet(true); // Force writing to the script
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyAngleToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_ANGLE, emitter->getDynAngle());
	if (dynAttr)
	{
		emitter->setDynAngle(dynAttr);
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyEmissionRateToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_EMISSION_RATE, emitter->getDynEmissionRate());
	if (dynAttr)
	{
		emitter->setDynEmissionRate(dynAttr);
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyTimeToLiveToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_TIME_TO_LIVE, emitter->getDynTotalTimeToLive());
	if (dynAttr)
	{
		emitter->setDynTotalTimeToLive(dynAttr);
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyMassToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_MASS, emitter->getDynParticleMass());
	if (dynAttr)
	{
		emitter->setDynParticleMass(dynAttr);
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyTextureCoordinateToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	if (validateTextureCoord(prop, emitter))
	{
		emitter->setParticleTextureCoords(prop->DoGetValue().GetLong());
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyTextureCoordinateStartToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	if (validateTextureCoord(prop, emitter))
	{
		emitter->setParticleTextureCoordsRangeStart(prop->DoGetValue().GetLong());
	}
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyTextureCoordinateEndToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	if (validateTextureCoord(prop, emitter))
	{
		emitter->setParticleTextureCoordsRangeEnd(prop->DoGetValue().GetLong());
	}
}
//-----------------------------------------------------------------------
bool EmitterPropertyWindow::validateTextureCoord(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	if (!prop || !emitter)
		return false;

	ParticleUniverse::ParticleTechnique* technique = emitter->getParentTechnique();
	if (technique)
	{
		ParticleUniverse::ParticleRenderer* renderer = technique->getRenderer();
		if (renderer)
		{
			size_t max = renderer->getNumTextureCoords();
			size_t val = prop->DoGetValue().GetLong();
			if (val < 0 || val > max - 1)
			{
				Ogre::String ogreMax = Ogre::StringConverter::toString(max);
				wxString wxMax = ogre2wx(ogreMax);
				wxString message = _("Value must be less than ") + wxMax + _(" (but positive or 0)");
				return _setPropertyError(prop , message);
			}
		}
	}

	return true;
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyColourToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	wxColor c;
	c = doGetColourWithAlpha(PRNL_EMITTER_COLOUR, c);
	Ogre::ColourValue colour(((ParticleUniverse::Real)c.Red())/255.0f, 
		((ParticleUniverse::Real)c.Green())/255.0f, 
		((ParticleUniverse::Real)c.Blue())/255.0f, 
		((ParticleUniverse::Real)c.Alpha())/255.0f);
	emitter->setParticleColour(colour);
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyColourRangeStartToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	wxColor c;
	c = doGetColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_START, c);
	Ogre::ColourValue colour(((ParticleUniverse::Real)c.Red())/255.0f, 
		((ParticleUniverse::Real)c.Green())/255.0f, 
		((ParticleUniverse::Real)c.Blue())/255.0f, 
		((ParticleUniverse::Real)c.Alpha())/255.0f);
	emitter->setParticleColourRangeStart(colour);
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyColourRangeEndToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	wxColor c;
	c = doGetColourWithAlpha(PRNL_EMITTER_COLOUR_RANGE_END, c);
	Ogre::ColourValue colour(((ParticleUniverse::Real)c.Red())/255.0f, 
		((ParticleUniverse::Real)c.Green())/255.0f, 
		((ParticleUniverse::Real)c.Blue())/255.0f, 
		((ParticleUniverse::Real)c.Alpha())/255.0f);
	emitter->setParticleColourRangeEnd(colour);
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyAllParticleDimensionsToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_ALL_PARTICLE_DIM, emitter->getDynParticleAllDimensions());
	if (dynAttr)
	{
		emitter->setDynParticleAllDimensions(dynAttr);
	}
	emitter->setDynParticleAllDimensionsSet(true); // Force writing to the script
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyParticleWidthToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_PARTICLE_WIDTH, emitter->getDynParticleWidth());
	if (dynAttr)
	{
		emitter->setDynParticleWidth(dynAttr);
	}
	emitter->setDynParticleWidthSet(true); // Force writing to the script
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyParticleHeightToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_PARTICLE_HEIGHT, emitter->getDynParticleHeight());
	if (dynAttr)
	{
		emitter->setDynParticleHeight(dynAttr);
	}
	emitter->setDynParticleHeightSet(true); // Force writing to the script
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyParticleDepthToEmitter(wxString propertyName, wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_EMITTER_PARTICLE_DEPTH, emitter->getDynParticleDepth());
	if (dynAttr)
	{
		emitter->setDynParticleDepth(dynAttr);
	}
	emitter->setDynParticleDepthSet(true); // Force writing to the script
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyAutoDirectionToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	emitter->setAutoDirection(prop->DoGetValue().GetBool());
}
//-----------------------------------------------------------------------
void EmitterPropertyWindow::copyForceEmissionToEmitter(wxPGProperty* prop, ParticleUniverse::ParticleEmitter* emitter)
{
	emitter->setForceEmission(prop->DoGetValue().GetBool());
}
//-----------------------------------------------------------------------
const Ogre::String& EmitterPropertyWindow::getEmitterTypeByProperty(wxPGProperty* prop)
{
	int type = prop->DoGetValue().GetLong(); // The property must be a list (PRNL_EMITTER_TYPE)
	switch (type)
	{
		case 0:
			return EMITTER_BOX;
		break;
		case 1:
			return EMITTER_CIRCLE;
		break;
		case 2:
			return EMITTER_LINE;
		break;
		case 3:
			return EMITTER_MESH_SURFACE;
		break;
		case 4:
			return EMITTER_POINT;
		break;
		case 5:
			return EMITTER_POSITION;
		break;
		case 6:
			return EMITTER_SLAVE;
		break;
		case 7:
			return EMITTER_SPHERE;
		break;
		case 8:
			return EMITTER_VERTEX;
		break;
	}

	return Ogre::StringUtil::BLANK;
}