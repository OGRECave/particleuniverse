/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/ScaleVelocityAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
ScaleVelocityAffectorPropertyWindow::ScaleVelocityAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ScaleVelocityAffectorPropertyWindow::ScaleVelocityAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ScaleVelocityAffectorPropertyWindow::ScaleVelocityAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::ScaleVelocityAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void ScaleVelocityAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ScaleVelocityAffector* affector = static_cast<ParticleUniverse::ScaleVelocityAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName.StartsWith(PRNL_VELOCITY_SCALE))
	{
		// ScaleVelocity: DynamicAttribute
		affector->resetDynScaleVelocity(false);
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_VELOCITY_SCALE, affector->getDynScaleVelocity());
		if (dynAttr)
		{
			affector->setDynScaleVelocity(dynAttr);
		}
		dynAttr = affector->getDynScaleVelocity();
		if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && dynAttr->getValue() == ParticleUniverse::ScaleVelocityAffector::DEFAULT_VELOCITY_SCALE)
		{
			// Force default state
			affector->resetDynScaleVelocity(true);
		}

		if (affector->_isMarkedForEmission())
		{
			_unprepare(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
		}
	}
	else if (propertyName == PRNL_SINCE_START_SYSTEM)
	{
		// Since Start System: bool
		affector->setSinceStartSystem(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_STOP_AT_FLIP)
	{
		// Stop at flip: bool
		affector->setStopAtFlip(prop->DoGetValue().GetBool());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void ScaleVelocityAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::ScaleVelocityAffector* scaleVelocityAffector = static_cast<ParticleUniverse::ScaleVelocityAffector*>(affector);

	// ScaleVelocity: DynamicAttribute
	doSetDynamicAttribute(PRNL_VELOCITY_SCALE, scaleVelocityAffector->getDynScaleVelocity());

	// Since Start System: bool
	doSetBool(PRNL_SINCE_START_SYSTEM, scaleVelocityAffector->isSinceStartSystem());

	// Stop at flip: bool
	doSetBool(PRNL_STOP_AT_FLIP, scaleVelocityAffector->isStopAtFlip());
}
//-----------------------------------------------------------------------
void ScaleVelocityAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_VELOCITY_SCALE = _("Scale velocity");
	PRNL_STOP_AT_FLIP = _("Stop at flip");
	PRNL_SINCE_START_SYSTEM = _("Since start system");

	mHelpHtml = wxT("AffectorScaleVelocity.html");

	// ScaleVelocity: DynamicAttribute
	ParticleUniverse::DynamicAttributeFixed dynAttr;
	dynAttr.setValue(ParticleUniverse::ScaleVelocityAffector::DEFAULT_VELOCITY_SCALE);
	appendDynamicAttribute(PRNL_VELOCITY_SCALE, PRNL_VELOCITY_SCALE, dynAttr);

	// Since Start System: bool
	Append(wxBoolProperty(PRNL_SINCE_START_SYSTEM, PRNL_SINCE_START_SYSTEM, false));

	// Stop at flip: bool
	Append(wxBoolProperty(PRNL_STOP_AT_FLIP, PRNL_STOP_AT_FLIP, false));
}
