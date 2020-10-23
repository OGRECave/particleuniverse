/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/VelocityMatchingAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
VelocityMatchingAffectorPropertyWindow::VelocityMatchingAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
VelocityMatchingAffectorPropertyWindow::VelocityMatchingAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
VelocityMatchingAffectorPropertyWindow::VelocityMatchingAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::VelocityMatchingAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void VelocityMatchingAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::VelocityMatchingAffector* affector = static_cast<ParticleUniverse::VelocityMatchingAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_VELOCITY_MATCHING_RADIUS)
	{
		// Radius: ParticleUniverse::Real
		affector->setRadius(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void VelocityMatchingAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::VelocityMatchingAffector* velocityMatchingAffector = static_cast<ParticleUniverse::VelocityMatchingAffector*>(affector);
	
	// Radius: ParticleUniverse::Real
	doSetDouble(PRNL_VELOCITY_MATCHING_RADIUS, velocityMatchingAffector->getRadius());
}
//-----------------------------------------------------------------------
void VelocityMatchingAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_VELOCITY_MATCHING_RADIUS = _("Velocity matching radius");

	mHelpHtml = wxT("AffectorVelocityMatching.html");

	// Radius: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_VELOCITY_MATCHING_RADIUS, PRNL_VELOCITY_MATCHING_RADIUS, ParticleUniverse::VelocityMatchingAffector::DEFAULT_RADIUS));
	SetPropertyEditor(PRNL_VELOCITY_MATCHING_RADIUS, wxPG_EDITOR(SpinCtrl));
}