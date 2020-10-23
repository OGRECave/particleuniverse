/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/GeometryRotatorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
GeometryRotatorPropertyWindow::GeometryRotatorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
GeometryRotatorPropertyWindow::GeometryRotatorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
GeometryRotatorPropertyWindow::GeometryRotatorPropertyWindow(
	wxWindow* parent, 
	EditComponent* owner, 
	ParticleUniverse::GeometryRotator* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void GeometryRotatorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::GeometryRotator* affector = static_cast<ParticleUniverse::GeometryRotator*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_USE_OWN_ROTATION)
	{
		// Use own rotation speed: bool
		affector->setUseOwnRotationSpeed(prop->DoGetValue().GetBool());
	}
	else if (propertyName.StartsWith(PRNL_ROTATION_SPEED))
	{
		// Rotation speed: DynamicAttribute
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_ROTATION_SPEED, affector->getRotationSpeed());
		if (dynAttr)
		{
			affector->setRotationSpeed(dynAttr);
		}
		_unprepare(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
	}
	else if (propertyName == PRNL_ROTATION_AXIS + PRNL_X ||
		propertyName == PRNL_ROTATION_AXIS + PRNL_Y ||
		propertyName == PRNL_ROTATION_AXIS + PRNL_Z)
	{
		// Rotation axis: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_ROTATION_AXIS, v3);
		affector->setRotationAxis(v3);
		if (v3 == Ogre::Vector3::ZERO)
		{
			// Force to default state
			affector->resetRotationAxis();
		}
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void GeometryRotatorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::GeometryRotator* geometryRotator = static_cast<ParticleUniverse::GeometryRotator*>(affector);

	// Use own rotation speed: bool
	doSetBool(PRNL_USE_OWN_ROTATION, geometryRotator->useOwnRotationSpeed());

	// Rotation speed: DynamicAttribute
	doSetDynamicAttribute(PRNL_ROTATION_SPEED, geometryRotator->getRotationSpeed());

	// Rotation axis: Ogre::Vector3
	doSetVector3(PRNL_ROTATION_AXIS, geometryRotator->getRotationAxis());
}
//-----------------------------------------------------------------------
void GeometryRotatorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ROTATION_SPEED = _("Rotation speed");
	PRNL_ROTATION_AXIS = _("Rotation axis");
	PRNL_USE_OWN_ROTATION = _("Use own rotation");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");


	mHelpHtml = wxT("AffectorGeometryRotator.html");

	// Use own rotation speed: bool
	Append(wxBoolProperty(PRNL_USE_OWN_ROTATION, PRNL_USE_OWN_ROTATION, ParticleUniverse::GeometryRotator::DEFAULT_USE_OWN));

	// Rotation speed: DynamicAttribute
	ParticleUniverse::DynamicAttributeFixed dynAttr;
	dynAttr.setValue(ParticleUniverse::GeometryRotator::DEFAULT_ROTATION_SPEED);
	appendDynamicAttribute(PRNL_ROTATION_SPEED, PRNL_ROTATION_SPEED, dynAttr);

	// Rotation axis: Ogre::Vector3
	appendVector3(PRNL_ROTATION_AXIS, PRNL_ROTATION_AXIS, ParticleUniverse::GeometryRotator::DEFAULT_ROTATION_AXIS);
}