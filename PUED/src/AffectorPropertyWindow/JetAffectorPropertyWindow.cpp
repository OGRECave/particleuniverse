/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/JetAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
JetAffectorPropertyWindow::JetAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
JetAffectorPropertyWindow::JetAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
JetAffectorPropertyWindow::JetAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::JetAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void JetAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::JetAffector* affector = static_cast<ParticleUniverse::JetAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName.StartsWith(PRNL_ACCELERATION))
	{
		// Acceleration: DynamicAttribute
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_ACCELERATION, affector->getDynAcceleration());
		if (dynAttr)
		{
			affector->setDynAcceleration(dynAttr);
		}

		if (affector->_isMarkedForEmission())
		{
			_unprepare(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
		}
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void JetAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::JetAffector* jetAffector = static_cast<ParticleUniverse::JetAffector*>(affector);

	// Acceleration: DynamicAttribute
	doSetDynamicAttribute(PRNL_ACCELERATION, jetAffector->getDynAcceleration());
}
//-----------------------------------------------------------------------
void JetAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ACCELERATION = _("Acceleration");

	mHelpHtml = wxT("AffectorJet.html");

	// Acceleration: DynamicAttribute
	ParticleUniverse::DynamicAttributeFixed dynAttr;
	dynAttr.setValue(ParticleUniverse::JetAffector::DEFAULT_ACCELERATION);
	appendDynamicAttribute(PRNL_ACCELERATION, PRNL_ACCELERATION, dynAttr);
}