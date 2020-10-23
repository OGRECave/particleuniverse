/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EventHandlerPropertyWindow/DoPlacementParticleEventHandlerPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
DoPlacementParticleEventHandlerPropertyWindow::DoPlacementParticleEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EventHandlerPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoPlacementParticleEventHandlerPropertyWindow::DoPlacementParticleEventHandlerPropertyWindow(EventHandlerPropertyWindow* eventHandlerPropertyWindow) :
	EventHandlerPropertyWindow(eventHandlerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
DoPlacementParticleEventHandlerPropertyWindow::DoPlacementParticleEventHandlerPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::DoPlacementParticleEventHandler* eventHandler) :
	EventHandlerPropertyWindow(parent, owner, eventHandler->getName())
{
	copyAttributesFromEventHandler(eventHandler);
}
//-----------------------------------------------------------------------
void DoPlacementParticleEventHandlerPropertyWindow::copyAttributeToEventHandler(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::DoPlacementParticleEventHandler* handler = static_cast<ParticleUniverse::DoPlacementParticleEventHandler*>(mOwner->getPUElement());
	if (!handler)
		return;

	if (propertyName == PRNL_NUMBER_OF_PARTICLES)
	{
		// Number of Particles: ParticleUniverse::Real
		handler->setNumberOfParticles(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_INHERIT_POSITION)
	{
		// Inherit Position: bool
		handler->setInheritPosition(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_INHERIT_DIRECTION)
	{
		// Inherit Direction: bool
		handler->setInheritDirection(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_INHERIT_ORIENTATION)
	{
		// Inherit Orientation: bool
		handler->setInheritOrientation(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_INHERIT_TIME_TO_LIVE)
	{
		// Inherit Time to live: bool
		handler->setInheritTimeToLive(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_INHERIT_MASS)
	{
		// Inherit Mass: bool
		handler->setInheritMass(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_INHERIT_TEXTURE_COORDINATE)
	{
		// Inherit Texture Coord: bool
		handler->setInheritTextureCoordinate(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_INHERIT_COLOUR)
	{
		// Inherit Colour: bool
		handler->setInheritColour(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_INHERIT_PARTICLE_WIDTH)
	{
		// Inherit Particle width: bool
		handler->setInheritParticleWidth(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_INHERIT_PARTICLE_HEIGHT)
	{
		// Inherit Particle height: bool
		handler->setInheritParticleHeight(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_INHERIT_PARTICLE_DEPTH)
	{
		// Inherit Particle Depth: bool
		handler->setInheritParticleDepth(prop->DoGetValue().GetBool());
	}
	else
	{
		// Update handler with another attribute
		EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void DoPlacementParticleEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler* eventHandler)
{
	EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

	// Copy properties from eventHandler to property window
	ParticleUniverse::DoPlacementParticleEventHandler* doPlacementParticleEventHandler = 
		static_cast<ParticleUniverse::DoPlacementParticleEventHandler*>(eventHandler);

	// Number of Particles: ParticleUniverse::Real
	doSetDouble(PRNL_NUMBER_OF_PARTICLES, doPlacementParticleEventHandler->getNumberOfParticles());

	// Inherit Position: bool
	doSetBool(PRNL_INHERIT_POSITION, doPlacementParticleEventHandler->isInheritPosition());

	// Inherit Direction: bool
	doSetBool(PRNL_INHERIT_DIRECTION, doPlacementParticleEventHandler->isInheritDirection());
	
	// Inherit Orientation: bool
	doSetBool(PRNL_INHERIT_ORIENTATION, doPlacementParticleEventHandler->isInheritOrientation());
	
	// Inherit Time to live: bool
	doSetBool(PRNL_INHERIT_TIME_TO_LIVE, doPlacementParticleEventHandler->isInheritTimeToLive());
	
	// Inherit Mass: bool
	doSetBool(PRNL_INHERIT_MASS, doPlacementParticleEventHandler->isInheritMass());
	
	// Inherit Texture Coord: bool
	doSetBool(PRNL_INHERIT_TEXTURE_COORDINATE, doPlacementParticleEventHandler->isInheritTextureCoordinate());
	
	// Inherit Colour: bool
	doSetBool(PRNL_INHERIT_COLOUR, doPlacementParticleEventHandler->isInheritColour());
	
	// Inherit Particle width: bool
	doSetBool(PRNL_INHERIT_PARTICLE_WIDTH, doPlacementParticleEventHandler->isInheritParticleWidth());
	
	// Inherit Particle height: bool
	doSetBool(PRNL_INHERIT_PARTICLE_HEIGHT, doPlacementParticleEventHandler->isInheritParticleHeight());
	
	// Inherit Particle Depth: bool
	doSetBool(PRNL_INHERIT_PARTICLE_DEPTH, doPlacementParticleEventHandler->isInheritParticleDepth());
}
//-----------------------------------------------------------------------
void DoPlacementParticleEventHandlerPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_NUMBER_OF_PARTICLES = _("Number of particles");
	PRNL_INHERIT_POSITION  = _("Inherit position");
	PRNL_INHERIT_DIRECTION  = _("Inherit direction");
	PRNL_INHERIT_ORIENTATION  = _("Inherit orientation");
	PRNL_INHERIT_TIME_TO_LIVE  = _("Inherit time to live");
	PRNL_INHERIT_MASS  = _("Inherit mass");
	PRNL_INHERIT_TEXTURE_COORDINATE  = _("Inherit texture coord");
	PRNL_INHERIT_COLOUR  = _("Inherit colour");
	PRNL_INHERIT_PARTICLE_WIDTH  = _("Inherit particle width");
	PRNL_INHERIT_PARTICLE_HEIGHT  = _("Inherit particle height");
	PRNL_INHERIT_PARTICLE_DEPTH  = _("Inherit particle depth");

	mHelpHtml = wxT("EventHandlerDoPlacementParticle.html");

	// Force Emitter: Is established by means of a connection

	// Number of Particles: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_NUMBER_OF_PARTICLES, 
		PRNL_NUMBER_OF_PARTICLES, 
		ParticleUniverse::DoPlacementParticleEventHandler::DEFAULT_NUMBER_OF_PARTICLES));
	SetPropertyEditor(PRNL_NUMBER_OF_PARTICLES, wxPG_EDITOR(SpinCtrl));

	// Inherit Position: bool
	Append(wxBoolProperty(PRNL_INHERIT_POSITION, PRNL_INHERIT_POSITION, true));

	// Inherit Direction: bool
	Append(wxBoolProperty(PRNL_INHERIT_DIRECTION, PRNL_INHERIT_DIRECTION, false));
	
	// Inherit Orientation: bool
	Append(wxBoolProperty(PRNL_INHERIT_ORIENTATION, PRNL_INHERIT_ORIENTATION, false));
	
	// Inherit Time to live: bool
	Append(wxBoolProperty(PRNL_INHERIT_TIME_TO_LIVE, PRNL_INHERIT_TIME_TO_LIVE, false));
	
	// Inherit Mass: bool
	Append(wxBoolProperty(PRNL_INHERIT_MASS, PRNL_INHERIT_MASS, false));
	
	// Inherit Texture Coord: bool
	Append(wxBoolProperty(PRNL_INHERIT_TEXTURE_COORDINATE, PRNL_INHERIT_TEXTURE_COORDINATE, false));
	
	// Inherit Colour: bool
	Append(wxBoolProperty(PRNL_INHERIT_COLOUR, PRNL_INHERIT_COLOUR, false));
	
	// Inherit Particle width: bool
	Append(wxBoolProperty(PRNL_INHERIT_PARTICLE_WIDTH, PRNL_INHERIT_PARTICLE_WIDTH, false));
	
	// Inherit Particle height: bool
	Append(wxBoolProperty(PRNL_INHERIT_PARTICLE_HEIGHT, PRNL_INHERIT_PARTICLE_HEIGHT, false));
	
	// Inherit Particle Depth: bool
	Append(wxBoolProperty(PRNL_INHERIT_PARTICLE_DEPTH, PRNL_INHERIT_PARTICLE_DEPTH, false));
}